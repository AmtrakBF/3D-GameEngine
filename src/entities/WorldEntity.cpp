#include "entities/WorldEntity.h"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation({0.0f, 0.0f, 90.0f}), m_Scale(0.0f), m_IsStatic(false), m_UseCollision(false), m_CollisionLengths(0.0f), m_CollisionPos(0.0f), m_CollisionPosBottom(0.0f), m_CollisionPosTop(0.0f)
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
}

void WorldEntity::SetCollision(float widthX, float heightY, float lengthZ)
{
	//! set collision box measurements
	m_CollisionLengths = {widthX, heightY, lengthZ };
	//! set collision position, subject to change
	m_CollisionPos = {-1.0f, -1.0f, 0};
}