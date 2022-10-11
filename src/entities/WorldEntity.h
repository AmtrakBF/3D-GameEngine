#pragma once
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "rendering/Model.h"

class WorldEntity
{
private:
	glm::vec3 m_CollisionLengths;

public:
	WorldEntity();

	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	bool m_IsStatic;
	bool m_UseCollision;

	glm::vec3 m_CollisionPos;
	glm::vec3 m_CollisionPosTop;
	glm::vec3 m_CollisionPosBottom;
	Model m_Model;

	glm::vec3 GetCollisionLengths();

	void AttachModel(Model& model, GLenum drawType);
	void SetCollision(float widthX, float heightY, float lengthZ);
};