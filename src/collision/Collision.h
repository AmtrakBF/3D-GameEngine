#pragma once
#include "entities/WorldEntity.h"

//! Need to make this a singleton instance

class Collision
{
public:
	static bool CheckCollision(WorldEntity* entity1, WorldEntity* entity2);
	static bool CheckCollision2D(WorldEntity& entity1, WorldEntity& entity2);

	static void UpdateCollisions();
	static void Rotate2DCollision(WorldEntity& entity1);
	//static void RotateCollision();

private:
};
