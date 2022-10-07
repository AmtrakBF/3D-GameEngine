#include "VertexArray.h"

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(ID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::Generate()
{
	glGenVertexArrays(1, &ID);
}

void VertexArray::AddVAA(VertexArrayAttribute specifications)
{
	Bind();
	VertexBufferElement VBE;
	uint32_t offset = 0;

	for (int x = 0; x < specifications.GetElements().size(); x++)
	{
		VBE = specifications.GetElements()[x];
		glVertexAttribPointer(x, VBE.vertexCount, VBE.dataType, VBE.normalized, specifications.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(x);
		offset += VBE.vertexCount * VertexBufferElement::GetSizeOfType(VBE.dataType);
	}
}
