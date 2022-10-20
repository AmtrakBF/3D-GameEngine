#include "glad/glad.h"
#include "glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "collision/Collision.h"

#include "events/EventSystem.h"

#include "rendering/Shader.h"
#include "rendering/Model.h"
#include "rendering/Camera.h"
#include "rendering/Renderer.h"

#include "entities/World.h"
#include "entities/Pawn.h"
#include "entities/Circle.h"

#include "players/Line.h"

#include "debug/Debug.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	// -------------------------------------------------------------------
	// INIT

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (!window)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Camera::Instance()->SetAspectRatio(glm::vec2(800, 600));

	float lastFrame = 0.0f;

	// -------------------------------------------------------------------
	// SHADERS
	Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shader.use();
	shader.SetVec4("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);

	glm::mat4 view = glm::mat4(1.0f);
	view = Camera::Instance()->GetView(); // position, target, and up -- no need to calculate right and above
	shader.SetMat4("view", view);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.SetMat4("projection", projection);

	Shader shaderCircle("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shaderCircle.use();
	shaderCircle.SetVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	shaderCircle.SetMat4("model", model);
	shaderCircle.SetMat4("view", view);
	shaderCircle.SetMat4("projection", projection);

	// -------------------------------------------------------------------
	// MODELS

	Model test("res/objects/testobj.obj", shader);
	Model cube("res/objects/Line.obj", shader);
	Model tangle("res/objects/Rectangle.obj", shader);
	cube.InitVertexArray(GL_STATIC_DRAW);
	Line line(cube);

 	Pawn staticLine(cube, { 7.0f, 0.0f, 0.0f });
	Pawn staticLine1(cube, { 12.0f, 0.0f, 0.0f });
	Pawn staticLine2(cube, { 0.0f, 8.0f, 0.0f });
	Pawn staticLine3(cube, { -7.0f, 0.0f, 0.0f });
	Pawn staticLine4(cube, { 5.0f, -8.0f, 0.0f });
	Pawn staticLine5(cube, { 0.0f, 0.0f, -5.0f });

	Circle circle({0.0f, 0.0f, 0.0f}, 4, 12, shaderCircle);

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
		//Collision::Instance()->UpdateCollisions();
		EventSystem::Instance()->ProcessEvents();

		shader.use();
		view = Camera::Instance()->GetView(); // position, target, and up -- no need to calculate right and above
		shader.SetMat4("view", view);

		projection = glm::perspective(glm::radians(Camera::Instance()->GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.SetMat4("projection", projection);

		line.GetInput(window);
		Renderer::Draw();

		shader.use();
		shader.SetVec4("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));	

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera::Instance()->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::Instance()->scroll_callback(window, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}