#include "entities/WorldEntity.h"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation(0.0f), m_Scale(0.0f), m_IsStatic(false), m_UseCollision(false), m_CollisionLengths(0.0f), m_CollisionPos(0.0f)
{

}

glm::vec3 WorldEntity::GetCollisionLengths()
{
	return m_CollisionLengths;
}

void WorldEntity::AttachModel(Model& model, GLenum drawType)
{
	m_Model = model;
	m_Model.VAO.Generate();
	m_Model.InitVertexArray(drawType);
}

void WorldEntity::SetCollision(float widthX, float heightY, float lengthZ)
{
	m_CollisionLengths.x = widthX;
	m_CollisionLengths.y = heightY;
	m_CollisionLengths.z = lengthZ;

	m_CollisionPos = {-widthX/2, heightY, lengthZ};
}