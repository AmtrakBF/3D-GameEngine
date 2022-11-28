#pragma once

#include "glad/glad.h"
#include "glfw3.h"

class Engine
{
private:
	Engine() {}
	~Engine() {};
	Engine(const Engine& debug) {}
	Engine& operator = (const Engine& debug) {}

public:
	static Engine* Instance();
	void Init();
	void Run();
	//void AddShader()

	GLFWwindow* window;
};