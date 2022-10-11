#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const void* data, const uint32_t& dataSize, GLenum drawType)
	: ID(0), m_Count(0)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, drawType);
	m_Count = dataSize / sizeof(uint32_t);
}

ElementBuffer::ElementBuffer()
	: ID(0), m_Count(0)
{

}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &ID);
}

void ElementBuffer::CreateBuffer(const void* data, const uint32_t& dataSize, const GLenum drawType)
{
	if (ID == 0)
	{
		glGenBuffers(1, &ID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, drawType);
		m_Count = dataSize / sizeof(uint32_t);
	}
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}