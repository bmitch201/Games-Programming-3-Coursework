#pragma once

#include "pch.h"
#include "Common.h"

class Texture
{
private:
	GLuint m_texture, m_normal;
	std::string m_directory;

	static GLuint Load(const std::string& directory);
	static GLuint LoadNormal(const std::string& directory);

public:
	Texture(std::string path);
	Texture(std::string path, std::string normalPath);
	void Bind();
	void BindNormal();
};