#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* imgSource, GLenum textureType, GLenum TextureWrapS, GLenum TextureWrapT, GLenum TextureFilterMin, GLenum TextureFilterMag, GLenum colorType, GLenum dataType)
{
	width = 0;
	height = 0;
	nrChannels = 0;

	this->textureType = textureType;

	glGenTextures(1, &ID);
	glBindTexture(textureType, ID);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgSource, &width, &height, &nrChannels, 0);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, TextureWrapS); // S = X
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, TextureWrapT); // T = Y
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, TextureFilterMin);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, TextureFilterMag);

	if (data)
	{
		glTexImage2D(textureType, 0, colorType, width, height, 0, colorType, dataType, data);
		glGenerateMipmap(textureType);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::Bind()
{
	glBindTexture(textureType, ID);
}