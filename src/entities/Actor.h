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

	void Translate(glm::vec3 translation, bool isRotation = false);
	void Rotate(float degrees, glm::vec3 rotationAxis);
	void Scale(glm::vec3 scale);
	void UpdatePositionData(glm::vec3 translation, bool isRotation);

	std::vector<WorldEntity*> GetNearbyObjects(glm::vec3 distance);
	glm::vec3 GetDistance(WorldEntity* entity);

private:
	glm::mat4 m_TranslationMatrix, m_ViewMatrix, m_ProjectionMatrix;
};

/*
	needs model to be loaded and controlled
*/


