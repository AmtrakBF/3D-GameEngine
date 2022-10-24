#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera* Camera::Instance()
{
	static Camera camera;
	return &camera;
}

void Camera::OnUpdate()
{
	cameraSpeed = 2.5f * World::DeltaTime();
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::GetFOV()
{
	return fov;
}

glm::vec3 Camera::GetPosition()
{
	return cameraPos;
}

void Camera::SetPosition(glm::vec3 position)
{
	cameraPos = position;
}

void Camera::SetAspectRatio(glm::vec2 aspectRatio)
{
	this->aspectRatio = aspectRatio.x / aspectRatio.y;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // initially set to true
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xOffset = (float)xpos - lastX;
	float yOffset = (float)ypos - lastY;
	lastX = (float)xpos;
	lastY = (float)ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch -= yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset * 2;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos.y += cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		cameraPos.y -= cameraSpeed;
}