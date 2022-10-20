#pragma once
#include "entities/WorldEntity.h"
#include "glm/gtc/matrix_transform.hpp"

class Pawn : public WorldEntity
{
public:
	Pawn(Model model, glm::vec3 position);
};

Pawn::Pawn(Model model, glm::vec3 position)
{
	AttachModel(model, GL_STATIC_DRAW);
	m_Position = position;	

	b_UseCollision = true;

	for (const auto& i : m_Model.v_CollisonDimensions)
	{
		v_CollisionBoxes.push_back(CollisionBox{ this, i });
	}

	if (v_CollisionBoxes.empty())
		b_UseCollision = false;

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, m_Position);
	for (int x = 0; x < m_Model.v_Vertices.size(); x++)
	{
		m_Model.v_Vertices[x].vertices = trans * glm::vec4(m_Model.v_Vertices[x].vertices, 1.0f);
	}
	m_Model.VBO.UpdateBuffer(&m_Model.v_Vertices[0], m_Model.GetSizeInBytes());
}
