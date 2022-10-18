#include "entities/WorldEntity.h"
#include "rendering/Renderer.h"

WorldEntity::WorldEntity()
	: m_Name(""), m_Position(0.0f), m_Rotation({0.0f, 0.0f, 90.0f}), m_Scale(0.0f), m_Direction(0.0f), b_UseCollision(false)
{

}
void WorldEntity::Delete()
{

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

uint32_t WorldEntity::GetId()
{
	return m_Id;
}
