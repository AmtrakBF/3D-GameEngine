#include "collision/Collision.h"
#include "glm/glm.hpp"

bool Collision::CheckCollision(WorldEntity& entity1, WorldEntity& entity2)
{
	glm::vec3 entity1Left = entity1.m_CollisionPos + entity1.m_Position;
	glm::vec3 entity1Right = entity1.m_CollisionPos + entity1.m_Position + entity1.GetCollisionLengths();

	glm::vec3 entity2Left = entity2.m_CollisionPos + entity2.m_Position;
	glm::vec3 entity2Right = entity2.m_CollisionPos + entity2.m_Position + entity2.GetCollisionLengths();

	bool X = false, Y = false, Z = false;

	//! check left and right of object on x axis
	if (entity1Right.x <= entity2Left.x && entity1Left.x >= entity2Right.x || entity1Right.x >= entity2Left.x && entity1Left.x <= entity2Right.x)
		X = true;
	if (entity1Right.y <= entity2Left.y && entity1Left.y >= entity2Right.y || entity1Right.y >= entity2Left.y && entity1Left.y <= entity2Right.y)
		Y = true;
	if (entity1Right.z <= entity2Left.z && entity1Left.z >= entity2Right.z || entity1Right.z >= entity2Left.z && entity1Left.z <= entity2Right.z)
		Z = true;

	if (X && Y && Z)
		return true;
	return false;
}
