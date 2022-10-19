#pragma once
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "rendering/Model.h"
#include "collision/CollisionBox.h"

class WorldEntity
{
private:

protected:
	uint32_t m_Id;

public:
	WorldEntity();

	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::vec3 m_Direction;

	bool b_UseCollision;

	std::vector<CollisionBox> v_CollisionBoxes;
	Model m_Model;


	void Delete();
	void AttachModel(Model& model, GLenum drawType);

	uint32_t GetId();
};