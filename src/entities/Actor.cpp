#include "Actor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

Actor::Actor(Model model, glm::vec3 position)
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
	AttachModel(model, GL_DYNAMIC_DRAW);
}

Actor::Actor()
	: m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f))
{
}

//! translates the object on CPU side
void Actor::Translate(glm::vec3 translation)
{
	//! update position
	m_Position += translation;
	m_CollisionPosBottom += translation;
	m_CollisionPosTop += translation;


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

	RotateCollision();
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

void Actor::RotateCollision()
{
	//! Only rotates on z axis...
	float radian = (m_Rotation.z * 3.14159265359f) / 180.0f;

	m_CollisionPosTop -= m_Position;
	m_CollisionPosBottom -= m_Position;

	m_CollisionPosTop.x = ((5 * cos(radian)) + (-m_CollisionPosBottom.x)); // calculate X coord from cos
	m_CollisionPosTop.y = ((5 * sin(radian)) + (-m_CollisionPosBottom.y)); // calculate Y coord form sin

	m_CollisionPosBottom.x = ((-2 * cos(radian)) + m_CollisionPosBottom.x); // calculate X coord from cos
	m_CollisionPosBottom.y = ((-2 * sin(radian)) + m_CollisionPosBottom.y); // calculate Y coord form sin

	m_CollisionPosTop += m_Position;
	m_CollisionPosBottom += m_Position;
}
