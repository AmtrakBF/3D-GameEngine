#pragma once
#include "glfw3.h"

class World
{
private:
	static float deltaTime;
	static float lastFrame;
public:
	static void OnUpdate();
	static float DeltaTime();
};
