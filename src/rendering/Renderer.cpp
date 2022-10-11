#include "Renderer.h"

void Renderer::Draw(VertexArray& VAO, Shader& shader)
{
	VAO.Bind();
	shader.use();
	glDrawElements(GL_TRIANGLES, VAO.m_ElementCount, GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}

void Renderer::Draw(Model& model)
{
	model.VAO.Bind();
	model.shader->use();

	if (model.useIndexArray)
		glDrawElements(GL_TRIANGLES, model.EBO.GetCount(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, (uint32_t)model.vertices.size());
	model.VAO.Unbind();
}

void Renderer::Draw(WorldEntity& entity)
{
	entity.m_Model.VAO.Bind();
	entity.m_Model.shader->use();
	glDrawArrays(GL_TRIANGLES, 0, (uint32_t)entity.m_Model.vertices.size());
	entity.m_Model.VAO.Unbind();
}

void Renderer::Draw(std::vector<WorldEntity>& Entities)
{
	for (int x = 0; x < Entities.size(); x++)
		Draw(Entities[x].m_Model);
}

void Renderer::Clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

