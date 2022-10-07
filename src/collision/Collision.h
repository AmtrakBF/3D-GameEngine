#pragma once
#include "entities/WorldEntity.h"

class Collision
{
public:
	static bool CheckCollision(WorldEntity& entity1, WorldEntity& entity2);
};
