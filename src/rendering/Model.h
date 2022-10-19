#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "rendering/VertexArray.h"
#include "rendering/VertexBuffer.h"
#include "rendering/ElementBuffer.h"
#include "Shader.h"


class Model
{
protected:
	struct ModelData
	{
		glm::vec3 vertices;
		glm::vec3 normals;
		glm::vec2 uvs;
	};

public:
	Model(const char* src, Shader& shader);
	Model() : shader(nullptr), vertices(0), modelName(""), useIndexArray(false) {};
	Model(const Model& model);

	void LoadModel(const char* src);
	void InitVertexArray(GLenum drawType);
	uint32_t GetSizeInBytes();

	bool useIndexArray;

	std::vector<ModelData> vertices;
	std::vector<glm::uvec3> indices;
	std::string modelName;
	Shader* shader;
	VertexArray VAO;
	VertexBuffer VBO;
	ElementBuffer EBO;
};