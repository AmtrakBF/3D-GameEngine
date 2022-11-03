#pragma once
#include "entities/actor.h"
#include "rendering/Shader.h"
#include "entities/PlayerController.h"

class Line : public Actor, public PlayerController
{
public:
	Line(Model& model, glm::vec3 position = { 0, 0, 0 });

	virtual void GetInput(GLFWwindow* window) override;

private:
	void MoveModel(glm::vec3 location);
	void RotateModel(float rotation, glm::vec3 axis);
	void HandleEvent(Event* event) override;

	float GetDirectionalTranslation(float direction, float collisionDistance, float distance);

	float m_InputTimer = 0.0f;
	float m_LastKeyPress = 0.0f;
};
