#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "api/VertexArray.h"
#include "api/VertexBuffer.h"
#include "Shader.h"


class Model
{
private:
	struct ModelData
	{
		glm::vec3 vertices;
		glm::vec3 normals;
		glm::vec2 uvs;
	};

public:
	Model(const char* src, Shader& shader);
	Model() : shader(nullptr), vertices(0), modelName(""){};
	Model(const Model& model);

	void LoadModel(const char* src);
	void InitVertexArray(GLenum drawType);
	uint32_t GetSizeInBytes();

	std::vector<ModelData> vertices;
	std::string modelName;
	Shader* shader;
	VertexArray VAO;
	VertexBuffer VBO;
};