#include "Model.h"
#include "rendering/VertexArrayAttribute.h"

#include <fstream>
#include <iostream>
#include <sstream>


Model::Model(const char* src, Shader& shader)
	: shader(&shader), modelName(""), vertices(0), useIndexArray(false)
{
	LoadModel(src);
}

Model::Model(const Model& model)
{
	vertices = model.vertices;
	modelName = model.modelName;
	shader = model.shader;
	useIndexArray = false;
}

void Model::LoadModel(const char* src)
{
	std::string line = "";
	std::string prefix = "";
	int tempInt;

	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::vector<uint32_t> faceIndices;
	std::vector<uint32_t> uvIndices;
	std::vector<uint32_t> normIndices;

	std::stringstream ss;
	std::ifstream file;
	file.open(src);

	if (!file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}
	while (getline(file, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{
			ss >> modelName;
		}
		else if (prefix == "v")
		{
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texCoord;
			ss >> texCoord.x >> texCoord.y;
			temp_uvs.push_back(texCoord);
		}
		else if (prefix == "vn")
		{
			glm::vec3 normals;
			ss >> normals.x >> normals.y >> normals.z;
			temp_normals.push_back(normals);
		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> tempInt)
			{
				if (counter == 0) 
					faceIndices.push_back(tempInt);

				if (counter == 1) 
					uvIndices.push_back(tempInt);

				if (counter == 2) 
					normIndices.push_back(tempInt);

				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				if (counter > 2)
					counter = 0;
			}
		}
	}

	if (faceIndices.size() != uvIndices.size() && uvIndices.size() != normIndices.size())
		throw "ERROR::INDEXING::Size of indices are mismatched.";

	unsigned int vertexIndex = 0, uvIndex = 0, normIndex = 0;
	glm::vec3 normal{}, face{};
	glm::vec2 uv{};

	for (int x = 0; x < faceIndices.size(); x++)
	{
		vertexIndex = faceIndices[x];
		face = temp_vertices[vertexIndex - 1];

		uvIndex = uvIndices[x];
		uv = temp_uvs[uvIndex - 1];

		normIndex = normIndices[x];
		normal = temp_normals[normIndex - 1];

		vertices.push_back({ face, normal, uv });
	}
	file.close();
}

void Model::InitVertexArray(GLenum drawType)
{
	VAO.Generate();
	VBO.CreateBuffer(&vertices[0], GetSizeInBytes(), drawType);
	VertexArrayAttribute VAA;
	VAA.Push(3, GL_FLOAT, GL_FALSE);
	VAA.Push(3, GL_FLOAT, GL_FALSE);
	VAA.Push(2, GL_FLOAT, GL_FALSE);
	VAO.AddVAA(VAA);
	VAO.Bind();
	VBO.Bind();
	VAO.Unbind();
	VBO.Unbind();
}

uint32_t Model::GetSizeInBytes()
{
	return sizeof(ModelData) * (uint32_t)vertices.size();
}

