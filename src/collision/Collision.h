#pragma once
#include "entities/WorldEntity.h"

struct BoxCollision
{
	glm::vec3 TopLeft;
	glm::vec3 TopRight;
	glm::vec3 BottomLeft;
	glm::vec3 BottomRight;
};

class Collision
{
public:
	static bool CheckCollision(WorldEntity* entity1, WorldEntity* entity2);
	static bool CheckCollision2D(WorldEntity& entity1, WorldEntity& entity2);

	static void UpdateCollisions();
	static void Rotate2DCollision(WorldEntity& entity1);

private:
};
