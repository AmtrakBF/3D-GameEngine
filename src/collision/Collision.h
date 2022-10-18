#pragma once
#include "entities/WorldEntity.h"

//! Need to make this a singleton instance

class Collision
{
public:

	bool CheckCollision(WorldEntity* entity1, WorldEntity* entity2);
	bool CheckCollision2D(WorldEntity& entity1, WorldEntity& entity2);

	static Collision* Instance();
	void UpdateCollisions();
	void Rotate2DCollision(WorldEntity& entity1);
	const glm::vec3 c_CollisionDistance{ 1.0f };

private:
	//! Singleton, deny access to constructor/destructor
	Collision() {}
	~Collision() {}
	Collision(const Collision& rhs) {}
	Collision& operator = (const Collision& rhs) {}
};
