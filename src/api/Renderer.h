#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include <vector>

#include "entities/WorldEntity.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "api/Model.h"

class Renderer
{
public:
	void Draw(VertexArray& VAO, Shader& shader);
	static void Draw(Model& model);
	static void Draw(std::vector<WorldEntity>& Entities);
	static void Clear();
private:
	std::vector<VertexArray> VAOs;
};