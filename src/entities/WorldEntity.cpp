#include "entities/WorldEntity.h"
#include "rendering/Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation({ 0.0f, 0.0f, 90.0f }), m_Scale(0.0f), m_Direction(0.0f), b_UseCollision(false), m_Id(0), m_EntityAttchedTo(nullptr),
	m_TranslationMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f)), m_EntityMin(0.0f), m_EntityMax(0.0f), b_IsAttachedToEntity(false),
	b_AttachedEntityRotationFailed(false)
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

		//! -------------------------------------------------------------- POSSIBLE OPTIMIZATION -----------------------------------------------------
	for (const auto& i : m_Model.v_CollisonDimensions)
	{
		v_CollisionBoxes.push_back(CollisionBox{ this, i });
	}

	if (v_CollisionBoxes.empty())
		b_UseCollision = false;

	m_EntityMax = model.m_EntityMax;
	m_EntityMin = model.m_EntityMin;
	m_Position = GetPosition();

	Renderer::Entities.push_back(this);
	m_Id = Renderer::SetId();
}

void WorldEntity::UpdateEntityMinMax()
{
	if (v_CollisionBoxes.empty())
		return;

	glm::vec3 min = v_CollisionBoxes[0].CollisionMin(), max = v_CollisionBoxes[0].CollisionMax();

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
	m_EntityMax = max;
	m_EntityMin = min;
}

glm::vec3 WorldEntity::GetPosition()
{
	return { (m_EntityMax.x + m_EntityMin.x) / 2, (m_EntityMax.y + m_EntityMin.y) / 2, (m_EntityMax.z + m_EntityMin.z) / 2 };
}

void WorldEntity::SetPosition(glm::vec3 position)
{
	//! Super easy way to setPosition
	//! Can probably do mathematics to optimize
	Translate(-m_Position);
	Translate(position);
}

void WorldEntity::UpdatePosition()
{
	m_Position = GetPosition();
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
	UpdatePosition();
}

//! rotates the object on CPU side
void WorldEntity::Rotate(float degrees, glm::vec3 rotationAxis, glm::vec3 offset /*= { 0.0f, 0.0f, 0.0f }*/)
{
	//! update member rotation and normalize around 360 degrees of rotation
	m_Rotation += rotationAxis * degrees;
	if (m_Rotation.x > 360)  m_Rotation.x -= 360; // X
	if (m_Rotation.y > 360)  m_Rotation.y -= 360; // Y
	if (m_Rotation.z > 360)  m_Rotation.z -= 360; // Z

	//! set temp position buffer
	glm::vec3 pos = m_Position;

	//! translate to origin, and then perform rotation matrix
	Translate(-m_Position + offset);

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
	RotateCollisionData(pos, degrees, rotationAxis, offset);
	UpdateEntityMinMax();
	Translate(pos - offset);

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
	if (translation == glm::vec3(0.0f))
		return;

	for (auto& i : v_CollisionBoxes)
	{
		i.Translate(translation);
	}
	//if (!isRotation) ------------------------------------------- MAY CAUSE ISSUES -------------------------------------------
	m_Direction = glm::normalize(translation);
}

void WorldEntity::RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis, glm::vec3 offset)
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

void WorldEntity::SetRotationFail(bool condition)
{
	b_AttachedEntityRotationFailed = condition;
}
