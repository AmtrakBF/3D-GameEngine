#include "Actor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "rendering/Renderer.h"
#include "events/EventSystem.h"
#include "collision/Collision.h"

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
	m_Position = position;

	//! -------------------------------------------------------------- POSSIBLE OPTIMIZATION -----------------------------------------------------
	for (const auto& i : m_Model.v_CollisonDimensions)
	{
		v_CollisionBoxes.push_back(CollisionBox{ this, i });
	}

	if (v_CollisionBoxes.empty())
		b_UseCollision = false;

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
void Actor::Translate(glm::vec3 translation, bool isRotation) // (0, 1, 0)
{
	//! update position
	UpdatePositionData(translation, isRotation);
	glm::vec3 directionalTravel = Collision::Instance()->UpdateCollision(this);

	if (directionalTravel != glm::vec3(1.0f))
	{
		UpdatePositionData(directionalTravel, isRotation);
		translation += directionalTravel;
	}

	//! set translationMatrix to default matrix and translate it by given parameter
	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::translate(m_TranslationMatrix, translation);

	//! loop through each pair of vertex positions and update with translation matrix
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
	}
	//! update the GPU buffer with updated vertex data
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

//! rotates the object on CPU side
void Actor::Rotate(float degrees, glm::vec3 rotationAxis)
{
	//! update member rotation and normalize around 360 degrees of rotation
	m_Rotation += rotationAxis * degrees;
	if (m_Rotation.x > 360)  m_Rotation.x -= 360; // X
	if (m_Rotation.y > 360)  m_Rotation.y -= 360; // Y
	if (m_Rotation.z > 360)  m_Rotation.z -= 360; // Z

	//! set temp position buffer
	glm::vec3 pos = m_Position;

	//! translate to origin, and then perform rotation matrix
	Translate(-m_Position);

	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::rotate(m_TranslationMatrix, glm::radians(degrees), rotationAxis);

	//! Rotate all collisions associated with object
	for (auto& i : v_CollisionBoxes)
	{
		i.Rotate(degrees, rotationAxis);
		i.Translate(pos);
	}

	//! Check if collision is detected
	//! If so, rotate back to original position
	//! else continue on with regular translation
	glm::vec3 directionalTravel = Collision::Instance()->UpdateCollision(this);
	if (directionalTravel != glm::vec3(1.0f))
	{
 		m_TranslationMatrix = glm::mat4(1.0f);
		m_TranslationMatrix = glm::rotate(m_TranslationMatrix, glm::radians(0.0f), rotationAxis);

		for (auto& i : v_CollisionBoxes)
		{
			i.Translate(-pos);
 			i.Rotate(-degrees, rotationAxis);
		}
	}
	else
		for (auto& i : v_CollisionBoxes)
			i.Translate(-pos);
	
	//! loop through each pair of vertex positions and update with rotation matrix
	//! make sure normals are corrected for rotation
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
		m_Model.v_Vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.v_Vertices[x].normals;
	}

	//! translate back to previous location and apply new vertex data to GPU buffer
	Translate(pos);

	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

//! scales the object on CPU side
void Actor::Scale(glm::vec3 scale)
{
	//! check to see if parameters are uniform
	bool nonUniform = false;
	if (scale.x != scale.y && scale.y != scale.z)
		nonUniform = true;

	//! set translation matrix to default and scale by parameters
	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::scale(m_TranslationMatrix, scale);

	//! loop through each pair of vertices and apply scale, correct normals if scaling is non uniform
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);

		if (nonUniform)
			m_Model.v_Vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.v_Vertices[x].normals;
	}
	//! apply changed vertex data to GPU buffer
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
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

		//! ------------------------------------------------------------ Optimize by using GetDistance(); -------------------------------------
		//! Get distance between two entities
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

		//! Get distance between two entities
		glm::vec3 totalDistance = GetCollisionDistance(i);

		//! Check if all dimensions are within bounds of distance
		if (totalDistance.x <= distance.x && totalDistance.y <= distance.y && totalDistance.z <= distance.z)
			nearby.push_back(i);
	}

	return nearby;
}

glm::vec3 Actor::GetDistance(WorldEntity* entity)
{
	if (!entity->b_UseCollision || !b_UseCollision)
		return glm::vec3(0.0f);

	glm::vec3 center = { m_Model.Dimensions().x / 2, m_Model.Dimensions().y / 2, m_Model.Dimensions().z / 2 };
	center += m_Position;
	glm::vec3 entityCenter = { entity->m_Model.Dimensions().x / 2, entity->m_Model.Dimensions().y / 2, entity->m_Model.Dimensions().z / 2 };
	entityCenter += entity->m_Position;

	//! Distance from center to center
	glm::vec3 distance = glm::abs(entityCenter - center);

	//! We are calculating the difference from the center of model and half the size of the other model
	//!! Once we know that, we can then get the actual distance from Wall to Wall, not Center to Center
	distance.x -= (entity->m_Model.Dimensions().x / 2) + (m_Model.Dimensions().x / 2);
	distance.y -= (entity->m_Model.Dimensions().y / 2) + (m_Model.Dimensions().y / 2);
	distance.z -= (entity->m_Model.Dimensions().z / 2) + (m_Model.Dimensions().z / 2);

	return distance;
}

glm::vec3 Actor::GetCollisionDistance(WorldEntity* entity)
{
	if (!entity->b_UseCollision || !b_UseCollision)
		return glm::vec3(0.0f);

	glm::vec3 distance(0.0f);
	glm::vec3 tempDistance(0.0f);

	for (const auto& thisBox : v_CollisionBoxes)
	{
		for (const auto& entityBox : entity->v_CollisionBoxes)
		{
			tempDistance = glm::abs(entityBox.CollisionCenter() - thisBox.CollisionCenter());

			//! Get CollisionDimensions
			glm::vec3 iDimensions = entityBox.CollisionDimensions();
			glm::vec3 thisDimensions = thisBox.CollisionDimensions();

			//! We are calculating the difference from CollisionCenter and half the size of the CollisionBox
			//!! Once we know that, we can then get the actual distance from Wall to Wall, not Center to Center
			tempDistance.x -= (iDimensions.x / 2) + (thisDimensions.x / 2);
			tempDistance.y -= (iDimensions.y / 2) + (thisDimensions.y / 2);
			tempDistance.z -= (iDimensions.z / 2) + (thisDimensions.z / 2);

			if (tempDistance.x < distance.x && tempDistance.y < distance.y && tempDistance.z < distance.z)
				distance = tempDistance;
		}
	}

	return distance;
}

void Actor::UpdatePositionData(glm::vec3 translation, bool isRotation)
{
	m_Position += translation;
	for (auto& i : v_CollisionBoxes)
	{
		i.Translate(translation);
	}
	if (!isRotation)
		m_Direction = glm::normalize(translation);
}