#pragma once
#include "glfw3.h"
#include "glm/glm.hpp"
#include "entities/World.h"

class Camera {

public:
	static Camera* Instance();

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
	Camera() : aspectRatio(1.33333f), firstMouse(true), cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f), 
		lastX(400.0f), lastY(300.0f), yaw(-90.0f), pitch(0.0f), fov(45.0f), cameraSpeed(2.5f * World::DeltaTime()) {};
	~Camera() {};
	Camera(const Camera& camera) : aspectRatio(1.33333f), firstMouse(true), cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f),
		lastX(400.0f), lastY(300.0f), yaw(-90.0f), pitch(0.0f), fov(45.0f), cameraSpeed(2.5f * World::DeltaTime()) {};
	Camera& operator = (const Camera& camera) {};

	float aspectRatio;
	bool firstMouse;
	glm::vec3 cameraPos, cameraFront, cameraUp;
	float lastX, lastY, yaw, pitch, fov;
	float cameraSpeed;
};