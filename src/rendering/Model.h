#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "collision/CollisionBox.h"

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
	Model() : m_Shader(nullptr), v_Vertices(0), m_ModelName(""), b_UseIndexArray(false), b_CollisionBox(false) {};
	Model(const Model& model);

	void LoadModel(const char* src);
	void InitVertexArray(GLenum drawType);
	uint32_t GetSizeInBytes();

	bool b_UseIndexArray;
	bool b_CollisionBox;

	std::vector<glm::vec3> v_CollisonDimensions;

	std::vector<ModelData> v_Vertices;
	std::vector<glm::uvec3> v_Indices;
	std::string m_ModelName;
	Shader* m_Shader;
	VertexArray VAO;
	VertexBuffer VBO;
	ElementBuffer EBO;

private:
	void CreateCollisionBox(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices, int indexOffset);

};