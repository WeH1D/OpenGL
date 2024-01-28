#include "Texture.h"
#include "Utils.h"
#include <iostream>
#include "vendor/stb_image/stb_image.h"

Texture::Texture(std::string src, int width, int height, int channels) : 
	_rendererId(0), _fileSrc(src), _width(width), _height(height), _channels(channels)
{
	unsigned char* texture = stbi_load(src.c_str(), &_width, &_height, &_channels, 0);
	
	GLCall(glGenTextures(1, &_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererId));

	if (texture)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture))
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(texture);
}

Texture::~Texture()
{
}

void Texture::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, _rendererId));
};

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
