#pragma once
#include "entities/PlayerController.h"
#include "entities/WorldEntity.h"
#include "api/Model.h"

class Actor : public PlayerController, public WorldEntity
{
public:
	Actor(Model model);
	Actor();

	void Translate(glm::vec3 translation);
	void Rotate(float degrees, glm::vec3 rotationAxis);
	void Scale(glm::vec3 scale);

private:
	glm::mat4 trans, view, projection;
};

/*
	needs model to be loaded and controlled
*/


