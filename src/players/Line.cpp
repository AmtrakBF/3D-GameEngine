#include "Line.h"
#include "entities/World.h"
#include "collision/Collision.h"

Line::Line(Model model)
{
	AttachModel(model, GL_DYNAMIC_DRAW);
	SetCollision(2, 5, 2);
	m_UseCollision = true;
}

void Line::GetInput(GLFWwindow* window)
{
	PlayerController::GetInput(window);

	if (m_LastKeyPress > 0.2f)
	{
		// Left Arrow -- Move Left 1 Unit
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS /*&& m_Position.x > -3*/)
			MoveModel({ -1.0f, 0.0f, 0.0f });

		// Right Arrow -- Move Right 1 Unit
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 1.0f, 0.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS /*&& m_Position.x > -3*/)
			MoveModel({ 0.0f, 1.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 0.0f, -1.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 0.0f, 0.0f, -1.0f });

		// Space Bar -- Rotate Current Tile
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			RotateModel(90.0f, { 0.0f, 0.0f, 1.0f });

		// Escape -- Close Window || Needs relocation
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	m_LastKeyPress += World::DeltaTime();
}

void Line::MoveModel(glm::vec3 location)
{
	Translate(location);
	m_LastKeyPress = 0;
}

void Line::RotateModel(float rotation, glm::vec3 axis)
{
	Rotate(rotation, axis);
	m_LastKeyPress = 0;
}
