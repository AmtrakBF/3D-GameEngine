#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include <list>

#include "entities/WorldEntity.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "rendering/Model.h"

#include "entities/Actor.h"

class Renderer
{
private:
	static uint32_t m_EntityId;
public:
	static uint32_t SetId();

	void Draw(VertexArray& VAO, Shader& shader);
	static void Draw();
	static void Draw(Model* model);
	static void Draw(WorldEntity* entity);
	static void Draw(std::vector<WorldEntity*>& Entities);

	static void Clear();

	static std::list<WorldEntity*> Entities;
	static std::list<Actor*> Actors;
};