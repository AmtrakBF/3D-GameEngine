#pragma once
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "rendering/Model.h"

class WorldEntity
{
private:

protected:
	glm::vec3 m_CollisionLengths;
	uint32_t m_Id;

public:
	WorldEntity();

	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;


	bool m_IsStatic;
	bool m_UseCollision;

	glm::vec3 m_CollisionPos;
	glm::vec3 m_CollisionCenter;
	glm::vec3 m_CollisionMax;
	glm::vec3 m_CollisionMin;
	Model m_Model;


	void Delete();

	void AttachModel(Model& model, GLenum drawType);
	void SetCollision(float widthX, float heightY, float lengthZ);

	uint32_t GetId();
	glm::vec3 GetCollisionDimensions();
};