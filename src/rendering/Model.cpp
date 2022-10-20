#include "Model.h"
#include "rendering/VertexArrayAttribute.h"

#include <fstream>
#include <sstream>

Model::Model(const char* src, Shader& shader)
	: m_Shader(&shader), m_ModelName(""), v_Vertices(0), b_UseIndexArray(false), b_CollisionBox(false)
{
	LoadModel(src);
}

Model::Model(const Model& model)
{
	v_Vertices = model.v_Vertices;
	m_ModelName = model.m_ModelName;
	v_CollisonDimensions = model.v_CollisonDimensions;
	m_Shader = model.m_Shader;
	b_UseIndexArray = false;
	b_CollisionBox = false;
}

void Model::LoadModel(const char* src)
{
	std::string line = "";
	std::string prefix = "";
	int tempInt;
	int indexOffset = 0;
	int collisionVertexCount = 0;

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::vector<uint32_t> faceIndices;
	std::vector<uint32_t> uvIndices;
	std::vector<uint32_t> normIndices;

	std::vector<glm::vec3> collisionVertices;
	std::vector<uint32_t> collisionIndices;

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
			std::string name;
			ss >> name;

			if (name != "CollisionBox")
			{
				b_CollisionBox = false;
				m_ModelName = name;
			}
			else
			{
				b_CollisionBox = true;
				if (!collisionIndices.empty() && !collisionVertices.empty())
				{
					collisionVertexCount += collisionVertices.size();
					indexOffset = faceIndices.size() + collisionVertexCount;
					CreateCollisionBox(collisionVertices, collisionIndices, indexOffset);
					collisionIndices.clear();
					collisionIndices.clear();
				}
			}
		}
		else if (prefix == "v")
		{
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;

			if (b_CollisionBox)
				collisionVertices.push_back(vertex);
			else 
				temp_vertices.push_back(vertex);
		}
		else if (prefix == "vt")
		{
			if (!b_CollisionBox)
			{
				glm::vec2 texCoord;
				ss >> texCoord.x >> texCoord.y;
				temp_uvs.push_back(texCoord);
			}
		}
		else if (prefix == "vn")
		{
			if (!b_CollisionBox)
			{
				glm::vec3 normals;
				ss >> normals.x >> normals.y >> normals.z;
				temp_normals.push_back(normals);
			}
		}
		else if (prefix == "s")
		{
			//! ------------------------------------- DO SOMETHING ------------------------------------- 
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> tempInt)
			{
				if (counter == 0 && !b_CollisionBox)
					faceIndices.push_back(tempInt);

				if (counter == 0 && b_CollisionBox)
					collisionIndices.push_back(tempInt);

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

	if (!collisionIndices.empty() && !collisionVertices.empty())
	{
		indexOffset += collisionVertices.size();
		CreateCollisionBox(collisionVertices, collisionIndices, indexOffset);
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

		v_Vertices.push_back({ face, normal, uv });
	}
	file.close();
}

void Model::InitVertexArray(GLenum drawType)
{
	VAO.Generate();
	VBO.CreateBuffer(&v_Vertices[0], GetSizeInBytes(), drawType);
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
	return sizeof(ModelData) * (uint32_t)v_Vertices.size();
}

void Model::CreateCollisionBox(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices, int indexOffset)
{
	glm::vec3 max = vertices[0], min = vertices[0];

	unsigned int vertexIndex = 0;
	std::vector<glm::vec3> newVertices;

	glm::vec3 face(0.0f);

	for (int x = 0; x < indices.size(); x++)
	{
		vertexIndex = indices[x];
		face = vertices[vertexIndex - 1 - indexOffset];
		newVertices.push_back(face);
	}

	for (int x = 0; x < newVertices.size(); x++)
	{
		if (newVertices[x].x > max.x) max.x = newVertices[x].x;
		if (newVertices[x].y > max.y) max.y = newVertices[x].y;
		if (newVertices[x].z > max.z) max.z = newVertices[x].z;

		if (newVertices[x].x < min.x) min.x = newVertices[x].x;
		if (newVertices[x].y < min.y) min.y = newVertices[x].y;
		if (newVertices[x].z < min.z) min.z = newVertices[x].z;
	}

	v_CollisonDimensions.push_back(abs(max) + abs(min));
}
