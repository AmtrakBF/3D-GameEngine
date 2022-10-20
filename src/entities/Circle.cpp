#include "circle.h"

#include <math.h>

Circle::Circle(glm::vec3 position, int radius, int angleIncrement, Shader& inShader)
	: position(position), radius(radius), angleIncrement(angleIncrement)
{
	m_Shader = &inShader;
	GenerateCircle();
	GenerateTriangleIndices();

	b_UseIndexArray = true;

	VAO.Generate();
	VBO.CreateBuffer(&v_Vertices[0], GetSizeInBytes(), GL_STATIC_DRAW);
	EBO.CreateBuffer(&v_Indices[0], v_Indices.size() * sizeof(v_Indices[0]), GL_STATIC_DRAW);
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

	v_Vertices.push_back({ position, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} });

	float radian;
	for (float i = 0; i < MAX_ANGLE; i += angleIncrement)
	{
		radian = (i * PI) / 180;

		x = ((radius * cos(radian)) + position.x); // calculate X coord from cos
		y = ((radius * sin(radian)) + position.y); // calculate Y coord form sin

		v_Vertices.push_back({ {x, y, 0.0f }, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
	}
}

void Circle::GenerateTriangleIndices()
{
	int x = 0;
	unsigned int previous = 1;

	glm::uvec3 index;

	while (x < v_Vertices.size() - 1)
	{
		if (x == 0) // if [0] or every third index
			index.x = 0;
		if (x >= v_Vertices.size() - 2) // last set of vertices
		{
			index.y = 1; // first index
			index.z = previous;
		}
		else
		{
			index.y = previous;
			index.z = ++previous;
		}

		v_Indices.push_back(index);
		x++;
	}
}