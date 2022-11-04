#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, const uint32_t& dataSize, const GLenum drawType)
	: ID(0)
{
	CreateBuffer(data, dataSize, drawType);
}

VertexBuffer::~VertexBuffer()
{
	if (ID)
		glDeleteBuffers(1, &ID);
}

void VertexBuffer::CreateBuffer(const void* data, const uint32_t& dataSize, const GLenum drawType)
{
	if (ID == 0)
	{
		glGenBuffers(1, &ID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, drawType);
	}
}

void VertexBuffer::UpdateBuffer(const void* data, const uint32_t& dataSize)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
