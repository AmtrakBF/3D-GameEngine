#pragma once
#include "Model.h"
#include "Renderer.h"
#include <array>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Cube : public Model
{
public:
	Cube(const char* fileLocation)
		: Model(fileLocation)
	{
		vertices = new float[size]
	}
};