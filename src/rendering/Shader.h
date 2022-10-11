#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>

class Shader
{
public:
	Shader() : ID(0) {}
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	uint32_t ID;

	void use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloat3(const std::string& name, const float* value) const;
	void SetMat4(const std::string& name, const glm::mat4& matrix);
	void SetVec3(const std::string& name, const glm::vec3& vec);
	void SetVec4(const std::string& name, const glm::vec4& vec);

private:
	uint32_t CompileShader(const char* shaderSource, GLenum shaderType);
	uint32_t LinkProgram(uint32_t vertexShader, uint32_t fragmentShader);
};