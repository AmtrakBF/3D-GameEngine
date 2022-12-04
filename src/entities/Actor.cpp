#include "Actor.h"

#include "rendering/Renderer.h"
#include "events/EventSystem.h"
#include "collision/Collision.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include "debug/Debug.h"


/*
* 
* Actors are movable (dynamic) objects in the game 
* 
* To achieve this, vertices are calculated on the CPU and then sent over to be GPU
* 
* I read that using GLProgram was slow so I figured instead of making a shader per object, you can move them via CPU
* Haven't done any benchmarking or anything but I can picture these functions being slower with more vertices
* 
* I also haven't done any research into any problems and tried to figure them out on my own. So to better improve this engine
* in the future, research will be required. Some of the methods used are probably not the fastest but just what I came up with
* 
*/

Actor::Actor(Model model, glm::vec3 position)
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
	AttachModel(model, GL_DYNAMIC_DRAW);
	Renderer::Actors.push_back(this);

	b_UseCollision = true;
	Translate(position);

	EventSystem::Instance()->RegisterClient("OnUpdate", this);
}

Actor::Actor()
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
	EventSystem::Instance()->RegisterClient("OnUpdate", this);
	Renderer::Actors.push_back(this);
}

Actor::~Actor()
{
	EventSystem::Instance()->UnregisterClient("OnUpdate", this);
	Renderer::Actors.remove(this);
}

//! translates the object on CPU side
void Actor::Translate(glm::vec3 translation)
{
	//! update m_Position && collisionBoxes && m_Direction
	TranslateCollisionData(translation);
	//! update entity Min and Max values (each entity is represented as a box, used for checking distance)
	UpdateEntityMinMax();
	//! translate entities that are attached to this object
	TranslateAttachedEntities(translation);

	glm::vec3 directionalTravel = Collision::Instance()->UpdateCollision(this);

	if (directionalTravel != glm::vec3(1.0f))
	{
		if (b_IsAttachedToEntity && directionalTravel != glm::vec3(0.0f))
		{
			m_EntityAttchedTo->Translate(directionalTravel);
		}
		else
		{
			TranslateCollisionData(directionalTravel);
			TranslateAttachedEntities(directionalTravel);
			translation += directionalTravel;
		}
	}

	//! set translationMatrix to default matrix and translate it by given parameter
	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::translate(m_TranslationMatrix, translation);

	//! loop through each pair of vertex positions and update with translation matrix
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
	}
	UpdateEntityMinMax();
	UpdatePosition();
	//! update the GPU buffer with updated vertex data
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

//! rotates the object on CPU side
void Actor::Rotate(float degrees, glm::vec3 rotationAxis, glm::vec3 offset /*= { 0.0f, 0.0f, 0.0f }*/)
{
	//! update member rotation and normalize around 360 degrees of rotation
	m_Rotation += rotationAxis * degrees;
	if (m_Rotation.x > 360)  m_Rotation.x -= 360; // X
	if (m_Rotation.y > 360)  m_Rotation.y -= 360; // Y
	if (m_Rotation.z > 360)  m_Rotation.z -= 360; // Z

	//! set temp position buffer
	glm::vec3 pos = m_Position;

	RotateAttachedEntities(degrees, rotationAxis);
	if (!b_IsAttachedToEntity && b_AttachedEntityRotationFailed)
	{
		RotateAttachedEntities(-degrees, rotationAxis);
		SetRotationFail(false);
		return;
	}


	//! translate to origin, and then perform rotation matrix
	Translate(-m_Position + offset);

	RotateCollisionData(pos, degrees, rotationAxis, offset);
	UpdateEntityMinMax();
	UpdatePosition();

	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::rotate(m_TranslationMatrix, glm::radians(degrees), rotationAxis);

	//! Check if collision is detected
	//! If so, rotate back to original position
	//! else continue on with regular translation
	glm::vec3 directionalTravel = Collision::Instance()->UpdateCollision(this);
	if (directionalTravel != glm::vec3(1.0f))
	{
		if (b_IsAttachedToEntity)
		{
			m_EntityAttchedTo->SetRotationFail(true);

			for (auto& i : v_CollisionBoxes)
				i.Translate(-pos);
			UpdateEntityMinMax();
			UpdatePosition();
		}
		else
		{
			for (auto& i : v_CollisionBoxes)
				i.Translate(-pos);
			RotateCollisionData(-pos, -degrees, rotationAxis, offset);
			UpdateEntityMinMax();
			UpdatePosition();

			m_TranslationMatrix = glm::mat4(1.0f);
 			m_TranslationMatrix = glm::rotate(m_TranslationMatrix, glm::radians(0.0f), rotationAxis);
		}
	}
	else
	{
		for (auto& i : v_CollisionBoxes)
			i.Translate(-pos);
		UpdateEntityMinMax();
		UpdatePosition();
	}
	
	//! loop through each pair of vertex positions and update with rotation matrix
	//! make sure normals are corrected for rotation
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
		m_Model.v_Vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.v_Vertices[x].normals;
	}

	//! translate back to previous location and apply new vertex data to GPU buffer
	if (b_IsAttachedToEntity)
		Translate(m_EntityAttchedTo->m_Position);
	else
		Translate(pos - offset);
	UpdateEntityMinMax();
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

//! scales the object on CPU side
void Actor::Scale(glm::vec3 scale)
{
	Scale(scale);
}

void Actor::RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis, glm::vec3 offset)
{
	//! Rotate all collisions associated with object
	for (auto& i : v_CollisionBoxes)
	{
		i.Rotate(degrees, rotationAxis);
		i.Translate(translation - offset);
	}
}

std::vector<WorldEntity*> Actor::GetNearbyObjects(glm::vec3 distance)
{
	std::vector<WorldEntity*> nearby;

	if (!b_UseCollision)
		return nearby;

	for (const auto i : Renderer::Entities)
	{
		// Continue iteration if this object is "i"
		if (i->GetId() == m_Id)
			continue;

		glm::vec3 totalDistance = GetCollisionDistance(i);

		//! Check if all dimensions are within bounds of distance
		if (totalDistance.x <= distance.x && totalDistance.y <= distance.y && totalDistance.z <= distance.z)
			nearby.push_back(i);
	}

	return nearby;
}

std::vector<WorldEntity*> Actor::GetNearbyObjects_CollisionBox(glm::vec3 distance)
{
	std::vector<WorldEntity*> nearby;

	if (!b_UseCollision)
		return nearby;

	for (const auto i : Renderer::Entities)
	{
		// Continue iteration if this object is "i"
		if (i->GetId() == m_Id)
			continue;

		if (i->m_Model.b_IsCollision)
			continue;

		if (i == m_EntityAttchedTo)
			continue;

		if (m_EntityAttchedTo != nullptr && i->m_EntityAttchedTo == m_EntityAttchedTo)
			continue;

		bool isNotAttached = true;
		for (int x = 0; x < v_AttachedEntities.size(); x++)
		{
			if (v_AttachedEntities[x].m_Entity->GetId() == i->GetId())
				isNotAttached = false;
		}

		//! Make sure attached item isn't checked for collisions
		if (isNotAttached)
		{
			//! Get distance between two entities
			glm::vec3 totalDistance = GetCollisionDistance(i);

			//! Check if all dimensions are within bounds of distance
			if (totalDistance.x <= distance.x && totalDistance.y <= distance.y && totalDistance.z <= distance.z)
				nearby.push_back(i);
		}
	}

	return nearby;
}

glm::vec3 Actor::GetDistance(WorldEntity* entity)
{
	if (!entity->b_UseCollision || !b_UseCollision)
		return glm::vec3(1.0f);

	glm::vec3 distance = glm::abs(entity->m_Position) - abs(m_Position);

	//! Get CollisionDimensions
	glm::vec3 iDimensions = abs(entity->m_EntityMax - entity->m_EntityMin);
	glm::vec3 thisDimensions = abs(m_EntityMax - m_EntityMin);

	//! We are calculating the difference from CollisionCenter and half the size of the CollisionBox
	//!! Once we know that, we can then get the actual distance from Wall to Wall, not Center to Center
	distance.x -= (iDimensions.x / 2) + (thisDimensions.x / 2);
	distance.y -= (iDimensions.y / 2) + (thisDimensions.y / 2);
	distance.z -= (iDimensions.z / 2) + (thisDimensions.z / 2);

	return distance;
}

glm::vec3 Actor::GetCollisionDistance(WorldEntity* entity)
{
	if (!entity->b_UseCollision || !b_UseCollision)
		return glm::vec3(1.0f);

	glm::vec3 distance = glm::abs(entity->m_Position) - abs(m_Position);

	//! Get CollisionDimensions
	glm::vec3 iDimensions = abs(entity->m_EntityMax - entity->m_EntityMin);
	glm::vec3 thisDimensions = abs(m_EntityMax - m_EntityMin);

	//! We are calculating the difference from CollisionCenter and half the size of the CollisionBox
	//!! Once we know that, we can then get the actual distance from Wall to Wall, not Center to Center
	distance.x -= (iDimensions.x / 2) + (thisDimensions.x / 2);
	distance.y -= (iDimensions.y / 2) + (thisDimensions.y / 2);
	distance.z -= (iDimensions.z / 2) + (thisDimensions.z / 2);

	return distance;
}

void Actor::AttachEntity(WorldEntity* entity, glm::vec3 positionOffset /*= { 0.0f, 0.0f, 0.0f}*/)
{
	if (!entity)
		return;

	glm::vec3 offset(0.0f);
	offset = m_Position;
	if (positionOffset != glm::vec3{ 0.0f, 0.0f, 0.0f })
		offset += positionOffset;

	entity->b_IsAttachedToEntity = true;
	entity->m_EntityAttchedTo = this;
	entity->SetPosition(offset);
	v_AttachedEntities.push_back({ entity, offset });
}

void Actor::TranslateAttachedEntities(glm::vec3 translation)
{
	for (int x = 0; x < v_AttachedEntities.size(); x++)
	{
		v_AttachedEntities[x].m_Entity->Translate(translation);
	}
}

void Actor::RotateAttachedEntities(float degrees, glm::vec3 rotationAxis)
{
	for (int x = 0; x < v_AttachedEntities.size(); x++)
	{
		glm::vec3 offset = v_AttachedEntities[x].m_Entity->m_Position - m_Position;
		v_AttachedEntities[x].m_Entity->Rotate(degrees, rotationAxis, offset);
	}
}
