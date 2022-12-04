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
	Model(Shader* shader, const std::vector<glm::vec3>* vertices, const std::vector<glm::uvec3>* indices = nullptr);
	Model() : m_Shader(nullptr), v_Vertices(0), m_ModelName(""), b_UseIndexArray(false), m_Dimensions(0.0f), b_IsCollision(false)
	, m_EntityMax(0.0f), m_EntityMin(0.0f) {};
	Model(const Model& model);

	void LoadModel(const char* src);
	void InitVertexArray(GLenum drawType);
	void CalculateEntityMaxMin();
	uint32_t GetSizeInBytes();

	inline glm::vec3 Dimensions() const { return m_Dimensions; }

	bool b_UseIndexArray;
	bool b_IsCollision;

	std::vector<CollisionBox::CollisionData> v_CollisonDimensions;
	std::vector<ModelData> v_Vertices;
	std::vector<glm::uvec3> v_Indices;

	std::string m_ModelName;
	glm::vec3 m_Dimensions;
	glm::vec3 m_EntityMax, m_EntityMin;

	Shader* m_Shader;
	VertexArray VAO;
	VertexBuffer VBO;
	ElementBuffer EBO;

private:
	void CreateCollisionBox(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices, int indexOffset);


};