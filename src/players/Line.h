#pragma once
#include "entities/actor.h"
#include "api/Shader.h"

class Line : public Actor
{
public:
	Line(Model model);

	virtual void GetInput(GLFWwindow* window) override;

private:
	void MoveModel(glm::vec3 location);
	void RotateModel(float rotation, glm::vec3 axis);

	float m_InputTimer = 0.0f;
	float m_LastKeyPress = 0.0f;
};
