#include "Line.h"
#include "entities/World.h"
#include "collision/Collision.h"

#include "events/EventSystem.h"

Line::Line(Model model, glm::vec3 position)
	: Actor(model, position)
{
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
		{
			MoveModel({ -1.0f, 0.0f, 0.0f });
		}

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
	RotateCollision();
	m_LastKeyPress = 0;
}

/*
* // Doesn't really belong here
* //! Need to do more maths to figure out a system to rotate in more than one direction
* //!! But for now this works well enough as I only need it to rotate one direction for tetris
*/
void Line::RotateCollision()
{
	//! Only rotates on z axis...
	float radian = (m_Rotation.z * 3.14159265359f) / 180.0f;

	//! Set position to origin 
	m_CollisionMax -= m_Position;
	m_CollisionMin -= m_Position;

	//! Objects default transform width * cos(degree to rotate in radians) + opposite(CollisionPosition.x)
	m_CollisionMax.x = glm::round(((5 * cos(radian)) + (-m_CollisionPos.x))); 
	//! Objects default transform width * sin(degree to rotate in radians) + opposite(CollisionPosition.y)
	m_CollisionMax.y = glm::round(((5 * sin(radian)) + (-m_CollisionPos.y)));

	//! Objects default transform opposite(Height) * cos(degree to rotate in radians) + CollisionPosition.y
	m_CollisionMin.x = glm::round((((-2) * cos(radian)) + m_CollisionPos.x));
	//! Objects default transform opposite(Height) * sin(degree to rotate in radians) + CollisionPosition.y
	m_CollisionMin.y = glm::round((((-2) * sin(radian)) + m_CollisionPos.y));

	//! Rotates the collision position for proper alignment of Min and Max 
	m_CollisionPos.x = glm::round((((-2) * cos(radian)) + m_CollisionPos.x));
	m_CollisionPos.y = glm::round((((-2) * sin(radian)) + m_CollisionPos.y));

	//! Max is always top right
	//! Min is always bottom left

	//! if Min.x is > Max.x
	//! Swap around so the Max is actually the Max
	if (m_CollisionMin.x > m_CollisionMax.x)
	{
		float temp = m_CollisionMax.x;
		m_CollisionMax.x = m_CollisionMin.x;
		m_CollisionMin.x = temp;
	}

	//! if Min.y is > Max.y
	//! Swap around so the Max is actually the Max
	if (m_CollisionMin.y > m_CollisionMax.y)
	{
		float temp = m_CollisionMax.y;
		m_CollisionMax.y = m_CollisionMin.y;
		m_CollisionMin.y = temp;
	}

	//! Calculate new collision lengths with rotation
	m_CollisionLengths =  glm::vec3(abs(m_CollisionMax.x) + abs(m_CollisionMin.x), abs(m_CollisionMax.y) + abs(m_CollisionMin.y), abs(m_CollisionMax.z) + abs(m_CollisionMin.z));

	//! Set position back to position
	m_CollisionMax += m_Position;
	m_CollisionMin += m_Position;

	//! Calculate center of collision
	m_CollisionCenter = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };
}

#include <iostream>

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
		// figure out way to go back to position before
		m_Model.shader->use();
		m_Model.shader->SetVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		entity->Delete();
		//Translate({ -1.0f, -1.0f, 0.0f });
	}
}
