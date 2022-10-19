#pragma once
#include "entities/WorldEntity.h"
#include "entities/Actor.h"

//! Need to make this a singleton instance

class Collision
{
public:

	bool CheckCollision(WorldEntity* entity1, WorldEntity* entity2);
	bool CheckCollision2D(WorldEntity& entity1, WorldEntity& entity2);

	static Collision* Instance();
	void UpdateCollisions();
	glm::vec3 UpdateCollision(Actor* actor);
	glm::vec3 GetCollisionDirection(glm::vec3 direction, CollisionBox* a, CollisionBox* b);
	const glm::vec3 c_CollisionDistance{ 1.0f };

private:
	//! Singleton, deny access to constructor/destructor
	Collision() {}
	~Collision() {}
	Collision(const Collision& collision) {}
	Collision& operator = (const Collision& collision) {}
};
