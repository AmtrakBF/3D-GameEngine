#pragma once
#include "glfw3.h"
#include <vector>

struct VertexBufferElement
{
	uint32_t vertexCount;
	GLenum dataType;
	GLenum normalized;
	uint32_t dataTypeSize;

	static uint32_t GetSizeOfType(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		return 0;
	}
};

class VertexArrayAttribute
{
public:
	VertexArrayAttribute()
		: m_Stride(0) {}

	void Push(const uint32_t& vertexCount, const GLenum dataType, const GLenum normalized)
	{
		m_Stride += VertexBufferElement::GetSizeOfType(dataType) * vertexCount;
		m_Elements.push_back({ vertexCount, dataType, normalized, VertexBufferElement::GetSizeOfType(dataType)});
	}

	inline int GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
private:
	std::vector<VertexBufferElement> m_Elements;
	int m_Stride;
};