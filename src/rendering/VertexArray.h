#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include "VertexArrayAttribute.h"
#include "Shader.h"

class VertexArray
{
public:
	VertexArray() : ID(0), m_ElementCount(0) {};
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void Generate();

	void AddVAA(VertexArrayAttribute VAA);
	uint32_t m_ElementCount;
private:
	uint32_t ID;
};