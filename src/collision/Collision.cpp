#include "collision/Collision.h"
#include "glm/glm.hpp"

#include "rendering/Renderer.h"
#include "entities/Actor.h"
#include "events/EventSystem.h"

Collision* Collision::Instance()
{
	static Collision instance;
	return &instance;
}

bool Collision::CheckCollision(WorldEntity* a, WorldEntity* b)
{
	if (a && b)
	{
		for (auto& i : a->v_CollisionBoxes)
		{
			for (auto& x : b->v_CollisionBoxes)
			{
				if (i.CollisionMin().x < x.CollisionMax().x &&	 // Obj A BottomLeft X is    LESS THAN      Obj B TopRight    X
					i.CollisionMax().x > x.CollisionMin().x &&    // Obj A TopRight   X is    GREATER THAN   Obj B BottomLeft  X
					i.CollisionMin().y < x.CollisionMax().y &&    // Obj A BottomLeft Y is    LESS THAN      Obj B TopRight    Y
					i.CollisionMax().y > x.CollisionMin().y &&    // Obj A TopRight   Y is    GREATER THAN   Obj B BottomRight Y
					i.CollisionMin().z < x.CollisionMax().z &&
					i.CollisionMax().z > x.CollisionMin().z)
					return true;
			}
		}
	}
	
	return false;

}

bool Collision::CheckCollision2D(WorldEntity& a, WorldEntity& b)
{
// 	return a.m_CollisionMin.x <= b.m_CollisionMax.x &&
// 		a.m_CollisionMax.x >= b.m_CollisionMin.x &&
// 		a.m_CollisionMin.y <= b.m_CollisionMax.y &&
// 		a.m_CollisionMax.y >= b.m_CollisionMin.y;
	return true;
}

void Collision::Rotate2DCollision(WorldEntity& entity1)
{

}

void Collision::UpdateCollisions()
{
	for (const auto i : Renderer::Actors)
	{
		std::vector<WorldEntity*> entities = i->GetNearbyObjects(c_CollisionDistance);
		for (int x = 0; x < entities.size(); x++)
		{
			//! check all nearby entities of the Actor i
			if (CheckCollision(i, entities[x]))
			{
				//! if CheckCollision() returns true
				//! SendEvent of type collision with params of collided object
				//! Needs to be sent on another thread I believe
				EventSystem::Instance()->SendEvent("Collision", entities[x]);
			}
		}
	}
}