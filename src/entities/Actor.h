#pragma once
#include "entities/PlayerController.h"
#include "entities/WorldEntity.h"
#include "rendering/Model.h"
#include "events/Listener.h"

class Actor : public PlayerController, public WorldEntity, public Listener
{
public:
	Actor(Model model, glm::vec3 position);
	Actor();

	void Translate(glm::vec3 translation);
	void Rotate(float degrees, glm::vec3 rotationAxis);
	void Scale(glm::vec3 scale);

	void RotateCollision();

private:
	glm::mat4 m_TranslationMatrix, m_ViewMatrix, m_ProjectionMatrix;
};

/*
	needs model to be loaded and controlled
*/


