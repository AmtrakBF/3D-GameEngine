#pragma once
#include "entities/PlayerController.h"
#include "entities/WorldEntity.h"
#include "rendering/Model.h"
#include "events/Listener.h"

class Actor : public WorldEntity, public Listener
{
protected:
	std::vector<WorldEntity*> v_AttachedEntities;

	void RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis, glm::vec3 offset = { 0.0f, 0.0f, 0.0f }) override;

public:
	Actor(Model model, glm::vec3 position);
	Actor();
	~Actor();

	void Translate(glm::vec3 translation, bool checkForCollision = true) override;
	void Rotate(float degrees, glm::vec3 rotationAxis, bool checkForCollision = true, glm::vec3 offset = { 0.0f, 0.0f, 0.0f }) override;
	void Scale(glm::vec3 scale, bool checkForCollision = true) override;

	std::vector<WorldEntity*> GetNearbyObjects(glm::vec3 distance);
	std::vector<WorldEntity*> GetNearbyObjects_CollisionBox(glm::vec3 distance);
	glm::vec3 GetDistance(WorldEntity* entity);
	glm::vec3 GetCollisionDistance(WorldEntity* entity);

	void TranslateAttachedEntities(glm::vec3 translation, bool checkForCollision = true);
	void RotateAttachedEntities(float degrees, glm::vec3 rotationAxis, bool checkForCollision = true);

	//! Really should only be attaching Actors
	void AttachEntity(WorldEntity* entity, glm::vec3 positionOffset = { 0.0f, 0.0f, 0.0f });
	void DetachEntity(WorldEntity* entity, glm::vec3 positionOffset = { 0.0f, 0.0f, 0.0f });


private:

	glm::mat4 m_TranslationMatrix, m_ViewMatrix, m_ProjectionMatrix;
};

