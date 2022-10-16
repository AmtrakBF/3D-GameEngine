#include "Line.h"
#include "entities/World.h"
#include "collision/Collision.h"

#include "events/EventSystem.h"

#include <iostream>

Line::Line(Model model, glm::vec3 position)
{
	AttachModel(model, GL_DYNAMIC_DRAW);
	SetCollision(2, 5, 2);

	m_Position = position;

	m_CollisionMin = m_CollisionPos + m_Position;
	m_CollisionMax = m_CollisionPos + m_Position + GetCollisionLengths();
	m_CollisionCenter += m_CollisionMin;

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

	m_CollisionMax -= m_Position;
	m_CollisionMin -= m_Position;

	m_CollisionMax.x = ((GetCollisionLengths().y * cos(radian)) + (-m_CollisionPos.x)); // calculate X coord from cos
	m_CollisionMax.y = ((GetCollisionLengths().y * sin(radian)) + (-m_CollisionPos.y)); // calculate Y coord form sin

	m_CollisionMin.x = (((-GetCollisionLengths().x) * cos(radian)) + m_CollisionPos.x); // calculate X coord from cos
	m_CollisionMin.y = (((-GetCollisionLengths().x) * sin(radian)) + m_CollisionPos.y); // calculate Y coord form sin

	m_CollisionPos.x = (((-GetCollisionLengths().x) * cos(radian)) + m_CollisionPos.x); // calculate X coord from cos
	m_CollisionPos.y = (((-GetCollisionLengths().x) * sin(radian)) + m_CollisionPos.y); // calculate Y coord form sin

	m_CollisionMax += m_Position;
	m_CollisionMin += m_Position;

	if (m_CollisionMin.x > m_CollisionMax.x)
	{
		float temp = m_CollisionMax.x;
		m_CollisionMax.x = m_CollisionMin.x;
		m_CollisionMin.x = temp;
	}

	if (m_CollisionMin.y > m_CollisionMax.y)
	{
		float temp = m_CollisionMax.y;
		m_CollisionMax.y = m_CollisionMin.y;
		m_CollisionMin.y = temp;
	}

	glm::vec3 Lengths = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };
	//Lengths -= m_Position;
	m_CollisionCenter = Lengths;
}


/*
* // In the future I will add collisionEvent
* //! so this way only collisions can be passed through that event
* //!! no random events will be passed and it wont be so dangerous casting parameters
*/
void Line::HandleEvent(Event* event)
{
	WorldEntity* entity = (WorldEntity*)event->Paramater();

	if (entity)
	{
		// figure out way to go back to position before
		m_Model.shader->use();
		m_Model.shader->SetVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		//Translate({ -1.0f, -1.0f, 0.0f });
	}
}
