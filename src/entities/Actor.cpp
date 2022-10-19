#include "Actor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "rendering/Renderer.h"
#include "events/EventSystem.h"
#include "collision/Collision.h"

#include <iostream>
#include "debug/Debug.h"

Actor::Actor(Model model, glm::vec3 position)
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
	AttachModel(model, GL_DYNAMIC_DRAW);
	EventSystem::Instance()->RegisterClient("Collision", this);
	Renderer::Actors.push_back(this);

	m_Position = position;
	//! Need to implement feature to get collision box size from model?
	v_CollisionBoxes.push_back(CollisionBox{ this, {2, 5, 2} });

}

Actor::Actor()
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
	EventSystem::Instance()->RegisterClient("Collision", this);
	Renderer::Actors.push_back(this);
}

//! translates the object on CPU side
void Actor::Translate(glm::vec3 translation)
{
	//! update position
	UpdatePositionData(translation);
	glm::vec3 dir = Collision::Instance()->UpdateCollision(this);

	if (dir != glm::vec3(1.0f))
	{
		UpdatePositionData(dir);
		translation += dir;
	}

	//! set translationMatrix to default matrix and translate it by given parameter
	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::translate(m_TranslationMatrix, translation);

	//! loop through each pair of vertex positions and update with translation matrix
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.vertices[x].vertices, 1.0f);
	}
	//! update the GPU buffer with updated vertex data
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
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

	for (auto& i : v_CollisionBoxes)
	{
		i.Rotate();
	}

	//! loop through each pair of vertex positions and update with rotation matrix
	//! make sure normals are corrected for rotation
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.vertices[x].vertices, 1.0f);
		m_Model.vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.vertices[x].normals;
	}
	//! translate back to previous location and apply new vertex data to GPU buffer
	Translate(pos);
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
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
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.vertices[x].vertices, 1.0f);

		if (nonUniform)
			m_Model.vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.vertices[x].normals;
	}
	//! apply changed vertex data to GPU buffer
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
}

std::vector<WorldEntity*> Actor::GetNearbyObjects(glm::vec3 distance)
{
	std::vector<WorldEntity*> nearby;
	for (const auto i : Renderer::Entities)
	{
		// Continue iteration if this object is "i"
		if (i->GetId() == m_Id)
			continue;

		//! Get distance between two entities
		glm::vec3 totalDistance = GetDistance(i);

		//! Check if all dimensions are within bounds of distance
		if (totalDistance.x <= distance.x && totalDistance.y <= distance.y && totalDistance.z <= distance.z)
			nearby.push_back(i);
	}

	return nearby;
}

glm::vec3 Actor::GetDistance(WorldEntity* entity)
{
	//! --------------------------------------------------------------- DEBUG -----------------------------------------------
	//! need to add proper feature to loop through all collisions

	glm::vec3 distance = glm::abs(entity->v_CollisionBoxes[0].CollisionCenter() - v_CollisionBoxes[0].CollisionCenter());

	//! Get CollisionDimensions
	glm::vec3 iDimensions = entity->v_CollisionBoxes[0].CollisionDimensions();
	glm::vec3 thisDimensions = v_CollisionBoxes[0].CollisionDimensions();

	//! We are calculating the difference from CollisionCenter and half the size of the CollisionBox
	//!! Once we know that, we can then get the actual distance from Wall to Wall, not Center to Center
	distance.x -= (iDimensions.x / 2) + (thisDimensions.x / 2);
	distance.y -= (iDimensions.y / 2) + (thisDimensions.y / 2);
	distance.z -= (iDimensions.z / 2) + (thisDimensions.z / 2);

	return distance;
	return glm::vec3( 0.0f );
}

void Actor::UpdatePositionData(glm::vec3 translation)
{
	m_Position += translation;
	for (auto& i : v_CollisionBoxes)
	{
		i.Update(translation);
	}
	m_Direction = glm::normalize(translation);
}