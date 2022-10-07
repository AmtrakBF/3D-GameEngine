#pragma once
#include "glad/glad.h"
#include "glfw3.h"

class VertexBuffer
{
public:
	VertexBuffer() :ID(0) {};
	VertexBuffer(const void* data, const uint32_t& dataSize, const GLenum drawType);
	~VertexBuffer();

	void CreateBuffer(const void* data, const uint32_t& dataSize, const GLenum drawType);
	void UpdateBuffer(const void* data, const uint32_t& dataSize);

	void Bind() const;
	void Unbind() const;
private:
	uint32_t ID;
};