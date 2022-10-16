#include "collision/Collision.h"
#include "glm/glm.hpp"

#include "rendering/Renderer.h"
#include "entities/Actor.h"
#include "events/EventSystem.h"

#include <iostream>

bool Collision::CheckCollision(WorldEntity* a, WorldEntity* b)
{

	// Can try to implement new collisions system using center of box
	// Need to rotate the center

	if (a && b)
	{
// 		glm::vec3 totalDistance = glm::abs(a->m_CollisionCenter - b->m_CollisionCenter);
// 
// 		glm::vec3 aDimensions = a->GetCollisionLengths();
// 		glm::vec3 bDimensions = b->GetCollisionLengths();
// 
// 		totalDistance.x -= (aDimensions.x / 2) + (bDimensions.x / 2);
// 		totalDistance.y -= (aDimensions.y / 2) + (bDimensions.y / 2);
// 		totalDistance.z -= (aDimensions.z / 2) + (bDimensions.z / 2);
// 
// 		if (totalDistance.x <= 0.0f && totalDistance.y <= 0.0f && totalDistance.z <= 0.0f)
// 			return true;

		return  a->m_CollisionMin.x <= b->m_CollisionMax.x &&	 // Obj A BottomLeft X is    LESS THAN      Obj B TopRight    X
				a->m_CollisionMax.x >= b->m_CollisionMin.x &&    // Obj A TopRight   X is    GREATER THAN   Obj B BottomLeft  X
				a->m_CollisionMin.y <= b->m_CollisionMax.y &&    // Obj A BottomLeft Y is    LESS THAN      Obj B TopRight    Y
				a->m_CollisionMax.y >= b->m_CollisionMin.y &&    // Obj A TopRight   Y is    GREATER THAN   Obj B BottomRight Y
				a->m_CollisionMin.z <= b->m_CollisionMax.z &&
				a->m_CollisionMax.z >= b->m_CollisionMin.z;
	}
	
	return false;

}


/*
* // Need to figure out why some collisions are not being detected
* //! I will rotate an object, min and max are the same but in one orientation
* //!! Collisions are not detected....
*/
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
// 			std::cout << "COLLISION ENTITY: " << entities[x]->m_CollisionCenter.x << ", " << entities[x]->m_CollisionCenter.y << ", " << entities[x]->m_CollisionCenter.z << std::endl;
// 			std::cout << "COLLISION TOP: " << entities[x]->m_CollisionMax.x << ", " << entities[x]->m_CollisionMax.y << ", " << entities[x]->m_CollisionMax.z << std::endl;
// 			std::cout << "COLLISION BOTTOM: " << entities[x]->m_CollisionMin.x << ", " << entities[x]->m_CollisionMin.y << ", " << entities[x]->m_CollisionMin.z << std::endl;
			if (CheckCollision(i, entities[x]))
			{
				EventSystem::Instance()->SendEvent("Collision", entities[x]);
			}
		}
	}
}
