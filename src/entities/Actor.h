#pragma once
#include "entities/PlayerController.h"
#include "entities/WorldEntity.h"
#include "rendering/Model.h"
#include "events/Listener.h"

class Actor : public WorldEntity, public Listener
{
public:
	Actor(Model model, glm::vec3 position);
	Actor();
	~Actor();

	void Translate(glm::vec3 translation) override;
	void Rotate(float degrees, glm::vec3 rotationAxis, glm::vec3 offset = { 0.0f, 0.0f, 0.0f }) override;
	void Scale(glm::vec3 scale) override;

	std::vector<WorldEntity*> GetNearbyObjects(glm::vec3 distance);
	std::vector<WorldEntity*> GetNearbyObjects_CollisionBox(glm::vec3 distance);
	glm::vec3 GetDistance(WorldEntity* entity);
	glm::vec3 GetCollisionDistance(WorldEntity* entity);

	void TranslateAttachedEntities(glm::vec3 translation);
	void RotateAttachedEntities(float degrees, glm::vec3 rotationAxis);

	//! Really should only be attaching Actors
	void AttachEntity(WorldEntity* entity, glm::vec3 positionOffset = { 0.0f, 0.0f, 0.0f });


protected:
	void RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis) override;

	struct AttachedEntity
	{
		WorldEntity* m_Entity;
		glm::vec3 m_Offset;
	};

private:
	glm::mat4 m_TranslationMatrix, m_ViewMatrix, m_ProjectionMatrix;
	std::vector<AttachedEntity> v_AttachedEntities;
};

