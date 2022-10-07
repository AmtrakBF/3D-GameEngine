#include "Actor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Actor::Actor(Model model)
	: trans(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f))
{
	AttachModel(model, GL_DYNAMIC_DRAW);
}

Actor::Actor()
	: trans(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f))
{
}

//! translates the object on cpu side
void Actor::Translate(glm::vec3 translation)
{
	m_Position += translation;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, translation);
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = trans * glm::vec4(m_Model.vertices[x].vertices, 1.0f);
	}
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
}

//! rotates the object on cpu side
void Actor::Rotate(float degrees, glm::vec3 rotationAxis)
{
	m_Rotation += rotationAxis * degrees;
	if (m_Rotation.x > 360)  m_Rotation.x -= 360; // X
	if (m_Rotation.y > 360)  m_Rotation.y -= 360; // Y
	if (m_Rotation.z > 360)  m_Rotation.z -= 360; // Z

	glm::vec3 pos = m_Position;

	Translate(-m_Position);
	trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(degrees), rotationAxis);

	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = trans * glm::vec4(m_Model.vertices[x].vertices, 1.0f);

		m_Model.vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(trans))) * m_Model.vertices[x].normals;
	}
	Translate(pos);
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());

	if (m_UseCollision) // NO WORKY
	{
		glm::vec3 posBuffer = m_CollisionPos;

		glm::mat4 translation = glm::mat4(1.0f);
		translation = glm::translate(translation, -m_CollisionPos);
		m_CollisionPos = translation * glm::vec4(m_CollisionPos, 1.0f);

		m_CollisionPos = trans * glm::vec4(m_CollisionPos, 1.0f);

		translation = glm::translate(translation, posBuffer);
		m_CollisionPos = translation * glm::vec4(posBuffer, 1.0f);
	}
}

//! scales the object on cpu side
void Actor::Scale(glm::vec3 scale)
{
	bool nonUniform = false;
	if (scale.x != scale.y && scale.y != scale.z)
		nonUniform = true;

	trans = glm::mat4(1.0f);
	trans = glm::scale(trans, scale);
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = trans * glm::vec4(m_Model.vertices[x].vertices, 1.0f);

		if (nonUniform)
			m_Model.vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(trans))) * m_Model.vertices[x].normals;
	}
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
}