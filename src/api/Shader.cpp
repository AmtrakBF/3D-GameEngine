#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: ID(0)
{
	std::string vertexCode, fragementCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragementCode = fShaderStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragementCode.c_str();

	uint32_t vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
	uint32_t fragement = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);
	ID = LinkProgram(vertex, fragement);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat3(const std::string& name, const float* value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& vec)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

uint32_t Shader::CompileShader(const char* shaderSource, GLenum shaderType)
{
	uint32_t shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

uint32_t Shader::LinkProgram(uint32_t vertexShader, uint32_t fragmentShader)
{
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}