#include "collision/Collision.h"
#include "glm/glm.hpp"

#include "rendering/Renderer.h"
#include "entities/Actor.h"
#include "events/EventSystem.h"

bool Collision::CheckCollision(WorldEntity* a, WorldEntity* b)
{
	if (a && b)
	{
		return  a->m_CollisionMin.x <= b->m_CollisionMax.x &&	 // Obj A BottomLeft X is    LESS THAN      Obj B TopRight    X
				a->m_CollisionMax.x >= b->m_CollisionMin.x &&    // Obj A TopRight   X is    GREATER THAN   Obj B BottomLeft  X
				a->m_CollisionMin.y <= b->m_CollisionMax.y &&    // Obj A BottomLeft Y is    LESS THAN      Obj B TopRight    Y
				a->m_CollisionMax.y >= b->m_CollisionMin.y &&    // Obj A TopRight   Y is    GREATER THAN   Obj B BottomRight Y
				a->m_CollisionMin.z <= b->m_CollisionMax.z &&
				a->m_CollisionMax.z >= b->m_CollisionMin.z;
	}
	
	return false;

}

bool Collision::CheckCollision2D(WorldEntity& a, WorldEntity& b)
{
	return a.m_CollisionMin.x <= b.m_CollisionMax.x &&
		a.m_CollisionMax.x >= b.m_CollisionMin.x &&
		a.m_CollisionMin.y <= b.m_CollisionMax.y &&
		a.m_CollisionMax.y >= b.m_CollisionMin.y;
}

void Collision::Rotate2DCollision(WorldEntity& entity1)
{

}

void Collision::UpdateCollisions()
{
	for (const auto i : Renderer::Actors)
	{
		std::vector<WorldEntity*> entities = i->GetNearbyObjects(glm::vec3{1.0f});
		for (int x = 0; x < entities.size(); x++)
		{
			//! check all nearby entities of the Actor i
			if (CheckCollision(i, entities[x]))
			{
				//! if CheckCollision() returns true
				//! SendEvent of type collision with params of collided object
				EventSystem::Instance()->SendEvent("Collision", entities[x]);
			}
		}
	}
}