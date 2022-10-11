#pragma once
#include "glad/glad.h"
#include "glfw3.h"

class Texture
{
public:
	Texture(const char* imgSource, GLenum textureType, GLenum TextureWrapS, GLenum TextureWrapT, GLenum TextureFilterMin, GLenum TextureFilterMag, GLenum colorType, GLenum dataType);
	~Texture();
	void Bind();

private:
	uint32_t ID;
	int width, height, nrChannels;
	GLenum textureType;
};