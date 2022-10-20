#include "Renderer.h"

std::list<WorldEntity*> Renderer::Entities;
std::list<Actor*> Renderer::Actors;

uint32_t Renderer::SetId()
{
	m_EntityId++;
	return m_EntityId;
}

uint32_t Renderer::m_EntityId = -1;

void Renderer::Draw(VertexArray& VAO, Shader& shader)
{
	VAO.Bind();
	shader.use();
	glDrawElements(GL_TRIANGLES, VAO.m_ElementCount, GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}

void Renderer::Draw(Model* model)
{
	if (!model)
		throw "ERROR::RENDERER::MODEL::UNDEFINED MODEL";

	model->VAO.Bind();
	model->m_Shader->use();

	if (model->b_UseIndexArray)
		glDrawElements(GL_TRIANGLES, model->EBO.GetCount(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, (uint32_t)model->v_Vertices.size());
	model->VAO.Unbind();
}

void Renderer::Draw(WorldEntity* entity)
{
	if (!entity)
		return;

	entity->m_Model.VAO.Bind();
	entity->m_Model.m_Shader->use();
	glDrawArrays(GL_TRIANGLES, 0, (uint32_t)entity->m_Model.v_Vertices.size());
	entity->m_Model.VAO.Unbind();
}

void Renderer::Draw(std::vector<WorldEntity*>& Entities)
{
	for (int x = 0; x < Entities.size(); x++)
	{
		Draw(Entities[x]);
	}

}

void Renderer::Draw()
{
	for (auto const& i : Entities) {
		Draw(i);
	}
}

void Renderer::Clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

