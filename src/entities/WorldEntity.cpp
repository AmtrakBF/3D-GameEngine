#include "entities/WorldEntity.h"
#include "rendering/Renderer.h"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation({0.0f, 0.0f, 90.0f}), m_Scale(0.0f), m_IsStatic(false), m_UseCollision(false), m_CollisionLengths(0.0f), 
	m_CollisionPos(0.0f), m_CollisionMin(0.0f), m_CollisionMax(0.0f), m_Id(0.0f), m_CollisionCenter(0.0f)
{

}

WorldEntity::WorldEntity(void* entity)
{

}

glm::vec3 WorldEntity::GetCollisionLengths()
{
	return m_CollisionLengths;
}

void WorldEntity::AttachModel(Model& model, GLenum drawType)
{
	//! copy model reference to member variable
	m_Model = model;
	//! generate VAO for copied model
	m_Model.VAO.Generate();
	//! initialize vertexArray for model
	m_Model.InitVertexArray(drawType);

	Renderer::Entities.push_back(this);
	m_Id = Renderer::SetId();
}

void WorldEntity::SetCollision(float widthX, float heightY, float lengthZ)
{
	//! set collision box measurements
	m_CollisionLengths = {widthX, heightY, lengthZ };

	glm::vec3 lengths = { widthX / 2, heightY / 2, lengthZ / 2 };

	m_CollisionCenter = { widthX/2, heightY/2, lengthZ/2 };

	//! set collision position, subject to change
	m_CollisionPos = {-1.0f, -1.0f, 0};
}

uint32_t WorldEntity::GetId()
{
	return m_Id;
}
