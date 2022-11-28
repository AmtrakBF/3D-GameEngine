#include "collision/Collision.h"
#include "glm/glm.hpp"

#include "rendering/Renderer.h"
#include "events/EventSystem.h"

#include "debug/Debug.h"

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
				if (i.CollisionMin().x < x.CollisionMax().x &&	  // Obj A BottomLeft X is    LESS THAN      Obj B TopRight    X
					i.CollisionMax().x > x.CollisionMin().x &&    // Obj A TopRight   X is    GREATER THAN   Obj B BottomLeft  X
					i.CollisionMin().y < x.CollisionMax().y &&    // Obj A BottomLeft Y is    LESS THAN      Obj B TopRight    Y
					i.CollisionMax().y > x.CollisionMin().y &&    // Obj A TopRight   Y is    GREATER THAN   Obj B BottomRight Y
					i.CollisionMin().z < x.CollisionMax().z &&    // Obj A BottomLeft Z is	  LESS THAN		 OBJ B TopRight    Z
					i.CollisionMax().z > x.CollisionMin().z)
				{
					//! -------------------------------------------------------------------------------------------- DEBUG ------------------------------------------------------------------------
					//! why don't we use the actor collisions box direction instead of the other object?
					//! may cause issues further down with multiple actors
					x.m_CollisionDirection = GetCollisionDirection(a->m_Direction ,&i, &x);
					return true;
				}
				x.m_CollisionDirection = glm::vec3(0.0f);
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

glm::vec3 Collision::GetCollisionDirection(glm::vec3 direction, CollisionBox* a, CollisionBox* b)
{
	glm::vec3 outDirection(0.0f);

	if (direction.x > 0.0f)
	{
		//! LEFT
		if (a->CollisionMax().x > b->CollisionMin().x && a->CollisionMin().x < b->CollisionMin().x)
			outDirection.x = b->CollisionMin().x - a->CollisionMax().x;
	}
	if (direction.x < 0.0f)
	{
		//! RIGHT
		if (a->CollisionMin().x < b->CollisionMax().x && a->CollisionMax().x > b->CollisionMax().x)
			outDirection.x = b->CollisionMax().x - a->CollisionMin().x;
	}
	if (direction.y < 0.0f)
	{
		//! TOP
		if (a->CollisionMin().y < b->CollisionMax().y && a->CollisionMax().y > b->CollisionMax().y)
			outDirection.y = b->CollisionMax().y - a->CollisionMin().y;
	}
	if (direction.y > 0.0f)
	{
		//! BOTTOM
		if (a->CollisionMax().y > b->CollisionMin().y && a->CollisionMin().y < b->CollisionMin().y)
			outDirection.y = b->CollisionMin().y - a->CollisionMax().y;
	}

	if (direction.z < 0.0f)
	{
		//! FRONT
		if (a->CollisionMin().z < b->CollisionMax().z && a->CollisionMax().z > b->CollisionMax().z)
			outDirection.z = b->CollisionMax().z - a->CollisionMin().z;
	}
	if (direction.z > 0.0f)
	{
		//! BACK
		if (a->CollisionMax().z > b->CollisionMin().z && a->CollisionMin().z < b->CollisionMin().z)
			outDirection.z = b->CollisionMin().z - a->CollisionMax().z;
	}

	return outDirection;
}

void Collision::UpdateCollisions()
{
	for (const auto i : Renderer::Actors)
	{
		if (!i->b_UseCollision)
			continue;

		std::vector<WorldEntity*> entities = i->GetNearbyObjects_CollisionBox(c_CollisionDistance);
		for (int x = 0; x < entities.size(); x++)
		{
			//! check all nearby entities of the Actor i
			if (entities[x]->b_UseCollision && CheckCollision(i, entities[x]))
			{
				//! if CheckCollision() returns true
				//! SendEvent of type collision with params of collided object
				EventSystem::Instance()->SendEvent("Collision", entities[x]);
			}
		}
	}
}

glm::vec3 Collision::UpdateCollision(Actor* actor)
{
	if (!actor->b_UseCollision)
		return { 1.0f, 1.0f, 1.0f };

	std::vector<WorldEntity*> entities = actor->GetNearbyObjects_CollisionBox(c_CollisionDistance);
	for (int x = 0; x < entities.size(); x++)
	{
		//! check all nearby entities of the Actor i
		if (entities[x]->b_UseCollision && CheckCollision(actor, entities[x]))
		{
			EventSystem::Instance()->SendEvent("Collision", entities[x]);
			return entities[x]->v_CollisionBoxes[0].m_CollisionDirection;
		}
	}
	return { 1.0f, 1.0f, 1.0f };
}
