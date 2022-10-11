#include "collision/Collision.h"
#include "glm/glm.hpp"

bool Collision::CheckCollision(WorldEntity& entity1, WorldEntity& entity2)
{
	bool X = false, Y = false, Z = false;

	if (entity1.m_CollisionPosTop.x >= entity2.m_CollisionPosBottom.x && entity1.m_CollisionPosBottom.x <= entity2.m_CollisionPosTop.x)
		X = true;
	if (entity1.m_CollisionPosTop.y >= entity2.m_CollisionPosBottom.y && entity1.m_CollisionPosBottom.y <= entity2.m_CollisionPosTop.y)
		Y = true;
	if (entity1.m_CollisionPosTop.z >= entity2.m_CollisionPosBottom.z && entity1.m_CollisionPosBottom.z <= entity2.m_CollisionPosTop.z)
		Z = true;

	if (X && Y && Z)
		return true;
	return false;
}
