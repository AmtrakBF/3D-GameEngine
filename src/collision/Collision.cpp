#include "collision/Collision.h"
#include "glm/glm.hpp"

bool Collision::CheckCollision(WorldEntity& a, WorldEntity& b)
{
	return a.m_CollisionMin.x <= b.m_CollisionMax.x &&
		a.m_CollisionMax.x >= b.m_CollisionMin.x &&
		a.m_CollisionMin.y <= b.m_CollisionMax.y &&
		a.m_CollisionMax.y >= b.m_CollisionMax.y &&
		a.m_CollisionMin.z <= b.m_CollisionMax.z &&
		a.m_CollisionMax.z >= b.m_CollisionMin.z;
}
