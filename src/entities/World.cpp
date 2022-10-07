#include "entities/World.h"

float World::deltaTime = 0.0f;
float World::lastFrame = 0.0f;

void World::OnUpdate()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float World::DeltaTime()
{
	return deltaTime;
}