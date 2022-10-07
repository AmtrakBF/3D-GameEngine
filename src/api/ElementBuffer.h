#pragma once
#include "glad/glad.h"
#include "glfw3.h"

class ElementBuffer
{
public:
	ElementBuffer(const void* data, const uint32_t& dataSize, GLenum drawType);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() { return m_Count; }
private:
	uint32_t ID;
	uint32_t m_Count;
};