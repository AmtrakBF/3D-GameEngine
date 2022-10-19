#pragma once
#include "glm/glm.hpp"

class CollisionBox
{
private:
	class WorldEntity* entity;

	glm::vec3 m_CollisionPos;
	glm::vec3 m_CollisionCenter;
	glm::vec3 m_CollisionMax;
	glm::vec3 m_CollisionMin;
	glm::vec3 m_CollisionDimensions;


public:
	CollisionBox(WorldEntity* entity, glm::vec3 dimensions, glm::vec3 position = { -1.0f, -1.0f, 0.0f });
	~CollisionBox() {}

	glm::vec3 m_CollisionDirection;

	inline glm::vec3 CollisionMin() const { return m_CollisionMin; }
	inline glm::vec3 CollisionMax() const { return m_CollisionMax; }
	inline glm::vec3 CollisionCenter() const { return m_CollisionCenter; }
	inline glm::vec3 CollisionDimensions() const { return m_CollisionDimensions; }

	void Rotate();
	void Update(glm::vec3 position);
};