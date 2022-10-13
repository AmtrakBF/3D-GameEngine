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

Camera camera((float)800 / 600);

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

	float lastFrame = 0.0f;

	// -------------------------------------------------------------------
	// SHADERS
	Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	Shader shaderCircle("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shader.use();
	shader.SetVec4("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);

	glm::mat4 view = glm::mat4(1.0f);
	view = camera.GetView(); // position, target, and up -- no need to calculate right and above
	shader.SetMat4("view", view);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.SetMat4("projection", projection);

	shaderCircle.use();
	shaderCircle.SetVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	shaderCircle.SetMat4("model", model);
	shaderCircle.SetMat4("view", view);
	shaderCircle.SetMat4("projection", projection);

	// -------------------------------------------------------------------
	// MODELS
	std::vector<WorldEntity> Entities;

	Model cube("res/objects/rectangle.obj", shader);
	cube.InitVertexArray(GL_STATIC_DRAW);
	Line line(cube);
	EventSystem::Instance()->RegisterClient("test", &line);
	Entities.push_back(line);

 	Pawn staticLine(cube, { 3.0f, 0.0f, 0.0f });
 	Entities.push_back(staticLine);

	Circle circle({0.0f, 0.0f, 0.0f}, 4, 12, shaderCircle);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		camera.ProcessInput(window);

		EventSystem::Instance()->ProcessEvents();

		World::OnUpdate();
		camera.OnUpdate(World::DeltaTime());
		Renderer::Clear();

		shader.use();
		view = camera.GetView(); // position, target, and up -- no need to calculate right and above
		shader.SetMat4("view", view);

		projection = glm::perspective(glm::radians(camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.SetMat4("projection", projection);

		line.GetInput(window);
		//renderer.Draw(Entities);
		Renderer::Draw(staticLine.m_Model);
		Renderer::Draw(line.m_Model);

		shaderCircle.use();
		shaderCircle.SetMat4("view", view);
		shaderCircle.SetMat4("projection", projection);

		Renderer::Draw(circle);

		bool t1 = Collision::CheckCollision(line, staticLine);
		bool t2 = Collision::CheckCollision(staticLine, line);

	/*	std::cout << t1 << ", " << t2 << std::endl;*/
		shader.use();
		if (t1 || t2)
		{
			shader.SetVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else
		{
			shader.SetVec4("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		}

		std::cout << "TOP: " << line.m_CollisionMax.x << ", " << line.m_CollisionMax.y << ", " << line.m_CollisionMax.z << std::endl;
		std::cout << "BOTTOM: " << line.m_CollisionMin.x << ", " << line.m_CollisionMin.y << ", " << line.m_CollisionMin.z << std::endl;

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_callback(window, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}