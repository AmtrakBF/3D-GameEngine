#pragma once
#include "rendering/Model.h"
#include "rendering/VertexArray.h"
#include "rendering/VertexBuffer.h"

class Circle : public Model
{
public:

private:
	const int MAX_ANGLE = 360;
	const float PI = 3.14159265359f;

	int radius, angleIncrement;
	glm::vec3 position;

	void GenerateCircle();
	void GenerateTriangleIndices();
public:

	Circle(glm::vec3 position, int radius, int angleIncrement, Shader& shader);
	~Circle();
};