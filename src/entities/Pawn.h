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
	v_CollisionBoxes.push_back(CollisionBox{ this, {2, 5, 2} });

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, m_Position);
	for (int x = 0; x < m_Model.vertices.size(); x++)
	{
		m_Model.vertices[x].vertices = trans * glm::vec4(m_Model.vertices[x].vertices, 1.0f);
	}
	m_Model.VBO.UpdateBuffer(&m_Model.vertices[0], m_Model.GetSizeInBytes());
}
