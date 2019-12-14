﻿#include "pch.h"
#include "Texture.h"

//stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint Texture::Load(const std::string& directory)
{
	stbi_set_flip_vertically_on_load(true);

	GLuint texture = 0;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(directory.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GL_ATTEMPT(glGenTextures(1, &texture));
		GL_ATTEMPT(glBindTexture(GL_TEXTURE_2D, texture));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		auto glChannels = GL_RGBA;
		switch (nrChannels)
		{
		case 3:
			glChannels = GL_RGB;
			break;
		case 4:
			glChannels = GL_RGBA;
			break;
		default:
			break;
		}

		GL_ATTEMPT(glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);
		GL_ATTEMPT(glGenerateMipmap(GL_TEXTURE_2D)));
	}
	else
	{
		LOG_DEBUG("Failed to load texture", Log::Trace);
	}

	stbi_image_free(data);
	return texture;
}
GLuint Texture::LoadNormal(const std::string& directory)
{
	stbi_set_flip_vertically_on_load(true);

	GLuint texture = 1;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(directory.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GL_ATTEMPT(glGenTextures(1, &texture));
		GL_ATTEMPT(glBindTexture(GL_TEXTURE_2D, texture));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ATTEMPT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		auto glChannels = GL_RGBA;
		switch (nrChannels)
		{
		case 3:
			glChannels = GL_RGB;
			break;
		case 4:
			glChannels = GL_RGBA;
			break;
		default:
			break;
		}

		GL_ATTEMPT(glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);
		GL_ATTEMPT(glGenerateMipmap(GL_TEXTURE_2D)));
	}
	else
	{
		LOG_DEBUG("Failed to load texture", Log::Trace);
	}

	stbi_image_free(data);
	LOG_DEBUG(std::to_string(texture), Log::Trace);
	return texture;
}


Texture::Texture(std::string path) :m_texture(Load(path)), m_directory(path)
{

}

Texture::Texture(std::string path, std::string normalPath) :m_texture(Load(path)), m_normal(LoadNormal(normalPath)), m_directory(path)
{

}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);	
}

void Texture::BindNormal()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}