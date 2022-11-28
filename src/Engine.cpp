#include "Engine.h"

#include <iostream>

#include "events/EventSystem.h"
#include "rendering/Camera.h"
#include "rendering/Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Engine* Engine::Instance()
{
	static Engine instance;
	return &instance;
}

void Engine::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (!window)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void Engine::Run()
{
	float lastFrame = 0.0f;

	glEnable(GL_DEPTH_TEST);

	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	while (!glfwWindowShouldClose(window))
	{
		Camera::Instance()->ProcessInput(window);

		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;
		counter++;
		if (timeDiff >= 1.0f / 30.0)
		{
			std::string fps = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = fps + " FPS / " + ms + " ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = currentTime;
			counter = 0;
		}

		World::OnUpdate();
		Camera::Instance()->OnUpdate();
		Renderer::Clear();
		EventSystem::Instance()->SendEvent("OnUpdate", window);
		EventSystem::Instance()->ProcessEvents();
		Renderer::Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera::Instance()->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::Instance()->scroll_callback(window, xoffset, yoffset);
}