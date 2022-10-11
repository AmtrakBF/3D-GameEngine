#include "circle.h"

#include <math.h>
#include <iostream>

Circle::Circle(glm::vec3 position, int radius, int angleIncrement, Shader& inShader)
	: position(position), radius(radius), angleIncrement(angleIncrement)
{
	shader = &inShader;
	GenerateCircle();
	GenerateTriangleIndices();

	useIndexArray = true;

	VAO.Generate();
	VBO.CreateBuffer(&vertices[0], GetSizeInBytes(), GL_STATIC_DRAW);
	EBO.CreateBuffer(&indices[0], indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);
	VertexArrayAttribute VAA;
	VAA.Push(3, GL_FLOAT, GL_FALSE);
	VAA.Push(3, GL_FLOAT, GL_FALSE);
	VAA.Push(2, GL_FLOAT, GL_FALSE);
	VAO.AddVAA(VAA);
	VAO.Bind();
	VBO.Bind();
	EBO.Bind();
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

Circle::~Circle()
{

}

void Circle::GenerateCircle()
{
	// int center point in array
	//vertices.push_back({ position, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} });

	float x, y;

	vertices.push_back({ position, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} });

	float radian;
	for (float i = 0; i < MAX_ANGLE; i += angleIncrement)
	{
		radian = (i * PI) / 180;

		x = ((radius * cos(radian)) + position.x); // calculate X coord from cos
		y = ((radius * sin(radian)) + position.y); // calculate Y coord form sin

		vertices.push_back({ {x, y, 0.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});

		std::cout << "Vertices: " << x << ", " << y << std::endl;
	}
}

void Circle::GenerateTriangleIndices()
{
	int x = 0;
	unsigned int previous = 1;

	glm::uvec3 index;

	while (x < vertices.size() - 1)
	{
		if (x == 0) // if [0] or every third index
			index.x = 0;
		if (x >= vertices.size() - 2) // last set of vertices
		{
			index.y = 1; // first index
			index.z = previous;
		}
		else
		{
			index.y = previous;
			index.z = ++previous;
		}
		std::cout << index.x << ", " << index.y << ", " << index.z << std::endl;

		indices.push_back(index);
		x++;
	}
}