#pragma once
#include "glfw3.h"
#include "glm/glm.hpp"

class Camera {

public:
	Camera(float aspectRatio);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void OnUpdate(float deltaTime);

	glm::mat4 GetView();
	float GetFOV();
	glm::vec3 GetPosition();

	void ProcessInput(GLFWwindow* window);

	float aspectRatio;
private:
	bool firstMouse;
	glm::vec3 cameraPos, cameraFront, cameraUp;
	float lastX, lastY, yaw, pitch, fov;
	float deltaTime;
	float cameraSpeed;
};