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
	const glm::vec3 c_OriginCollisionDimensions;

public:
	struct CollisionData
	{
		glm::vec3 dimensions;
		glm::vec3 min, max;
	};

	CollisionBox(WorldEntity* entity, CollisionData data);
	~CollisionBox() {}

	glm::vec3 m_CollisionDirection;

	const inline glm::vec3 CollisionMin() const { return m_CollisionMin; }
	const inline glm::vec3 CollisionMax() const { return m_CollisionMax; }
	const inline glm::vec3 CollisionCenter() const { return m_CollisionCenter; }
	const inline glm::vec3 CollisionDimensions() const { return m_CollisionDimensions; }

	void Rotate(float degrees, glm::vec3 rotationAxis);
	void Translate(glm::vec3 position);
	void Scale(glm::vec3 scale);
};