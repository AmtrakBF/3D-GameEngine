#include "entities/WorldEntity.h"
#include "rendering/Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation({0.0f, 0.0f, 90.0f}), m_Scale(0.0f), m_Direction(0.0f), b_UseCollision(false), m_Id(0),
	m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f)), m_CollisionMin(0.0f), m_CollisionMax(0.0f)
{

}

void WorldEntity::Delete()
{
	if (m_Model.b_IsCollision)
	{
		
	}
}

void WorldEntity::AttachModel(Model& model, GLenum drawType)
{
	//! copy model reference to member variable
	m_Model = model;
	//! generate VAO for copied model
	m_Model.VAO.Generate();
	//! initialize vertexArray for model
	m_Model.InitVertexArray(drawType);

	UpdateEntityMinMax();

	Renderer::Entities.push_back(this);
	m_Id = Renderer::SetId();
}

void WorldEntity::UpdateEntityMinMax()
{
	if (v_CollisionBoxes.empty())
		return;

	glm::vec3 min = v_CollisionBoxes[0].CollisionMin(), max = v_CollisionBoxes[0].CollisionMax();
	bool firstIt = true;

	//! Skip first index as we set first index to min and max values
	for (int x = 1; x < v_CollisionBoxes.size(); x++)
	{
		glm::vec3 tempMin = v_CollisionBoxes[x].CollisionMin();
		glm::vec3 tempMax = v_CollisionBoxes[x].CollisionMax();

		if (tempMin.x < min.x)
			min.x = tempMin.x;
		if (tempMin.y < min.y)
			min.y = tempMin.y;
		if (tempMin.z < min.z)
			min.z = tempMin.z;

		if (tempMax.x > max.x)
			max.x = tempMax.x;
		if (tempMax.y > max.y)
			max.y = tempMax.y;
		if (tempMax.z > max.z)
			max.z = tempMax.z;
	}
	m_CollisionMax = max;
	m_CollisionMin = min;
}

//! translates the object on CPU side
void WorldEntity::Translate(glm::vec3 translation)
{
	//! update position
	TranslateCollisionData(translation);
	UpdateEntityMinMax();

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
void WorldEntity::Rotate(float degrees, glm::vec3 rotationAxis)
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
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = m_TranslationMatrix * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
		m_Model.v_Vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(m_TranslationMatrix))) * m_Model.v_Vertices[x].normals;
	}

	//! translate back to previous location and apply new vertex data to GPU buffer
	RotateCollisionData(pos, degrees, rotationAxis);
	UpdateEntityMinMax();
	Translate(pos);

	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

//! scales the object on CPU side
void WorldEntity::Scale(glm::vec3 scale)
{
	//! check to see if parameters are uniform
	bool nonUniform = false;
	if (scale.x != scale.y && scale.y != scale.z)
		nonUniform = true;

	glm::vec3 pos = m_Position;

	Translate(-pos);

	ScaleCollisionData(scale);
	UpdateEntityMinMax();

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
	Translate(pos);
	//! apply changed vertex data to GPU buffer
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}

void WorldEntity::TranslateCollisionData(glm::vec3 translation)
{
	m_Position += translation;
	for (auto& i : v_CollisionBoxes)
	{
		i.Translate(translation);
	}
	//if (!isRotation) ------------------------------------------- MAY CAUSE ISSUES -------------------------------------------
	m_Direction = glm::normalize(translation);
}

void WorldEntity::RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis)
{
	//! Rotate all collisions associated with object
	for (auto& i : v_CollisionBoxes)
	{
		i.Rotate(degrees, rotationAxis);
	}
}

void WorldEntity::ScaleCollisionData(glm::vec3 scale)
{
	for (auto& i : v_CollisionBoxes)
	{
		i.Scale(scale);
	}
}

uint32_t WorldEntity::GetId()
{
	return m_Id;
}
