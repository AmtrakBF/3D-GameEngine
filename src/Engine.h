#pragma once

#include "glad/glad.h"
#include "glfw3.h"

class Engine
{
private:
	Engine() : window(nullptr), screenHeight(600), screenWidth(800) {}
	~Engine() {};
	Engine(const Engine& debug) : window(nullptr), screenHeight(600), screenWidth(800) {}
	Engine& operator = (const Engine& debug) {}

public:
	static Engine* Instance();
	void Init();
	void Run();
	void SetScreenResolution(int width, int height);
	//void AddShader()

	int screenHeight, screenWidth;

	GLFWwindow* window;
};