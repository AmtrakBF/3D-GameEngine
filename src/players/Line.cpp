#include "Line.h"
#include "entities/World.h"

#include "events/EventSystem.h"
#include "collision/Collision.h"

#include <iostream>

#include "debug/Debug.h"

Line::Line(Model model, glm::vec3 position)
	: Actor(model, position)
{
	b_UseCollision = true;
}

void Line::GetInput(GLFWwindow* window)
{
	PlayerController::GetInput(window);

	if (m_LastKeyPress > 0.1f)
	{
		// Left Arrow -- Move Left 1 Unit
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS /*&& m_Position.x > -3*/)
			MoveModel({ -1.0f, 0.0f, 0.0f });
			//MoveModel({ -0.1f, 0.0f, 0.0f });

		// Right Arrow -- Move Right 1 Unit
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 1.0f, 0.0f, 0.0f });
			//MoveModel({ 0.1f, 0.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS /*&& m_Position.x > -3*/)
			MoveModel({ 0.0f, 1.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 0.0f, -1.0f, 0.0f });

		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 0.0f, 0.0f, -1.0f });
		
		// DEBUGGING
		// ----------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS /*&& m_Position.x < 3*/)
			MoveModel({ 0.0f, 0.0f, 1.0f });

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

/*
* // In the future I will add collisionEvent
* //! so this way only collisions can be passed through that event
* //!! no random events will be passed and it wont be so dangerous casting parameters
*/
void Line::HandleEvent(Event* event)
{
	WorldEntity* entity = static_cast<WorldEntity*>(event->Paramater());
	//WorldEntity* entity = (WorldEntity*)event->Paramater();

	if (entity)
	{
// 		m_Model.shader->use();
// 		m_Model.shader->SetVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

float Line::GetDirectionalTranslation(float direction, float collisionDistance, float distance)
{
	if (direction != 0)
	{
		int constant = (int)collisionDistance;
		if (distance < 0)
			constant = (int)-collisionDistance;
		return (collisionDistance - abs(distance)) * constant;
	}
	return 0.0f;
}
