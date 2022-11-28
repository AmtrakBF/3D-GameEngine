#pragma once
#include "glfw3.h"
#include "glm/glm.hpp"
#include "entities/World.h"

class Camera {

public:
	Camera();

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void OnUpdate();

	glm::mat4 GetView();
	float GetFOV();
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);

	void SetAspectRatio(glm::vec2 aspectRatio);

	void ProcessInput(GLFWwindow* window);

private:

	float aspectRatio;
	bool firstMouse;
	glm::vec3 cameraPos, cameraFront, cameraUp;
	float lastX, lastY, yaw, pitch, fov;
	float cameraSpeed;
};