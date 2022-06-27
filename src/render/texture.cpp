#include "texture.hpp"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path, TextureType type)
{
	// load image data
	
	int w, h, ch; // width, height, channels
	unsigned char* buffer = stbi_load(path.c_str(), &w, &h, &ch, 0);
	if (!buffer)
	{
		std::string msg = "Texture failed to load at path {0}";
		Log::print(Log::render, Log::error, msg, path);
		stbi_image_free(buffer); 
		return;
	}

	GLenum format = [ch]()
	{	
		switch (ch)
		{
			case 1 :  return GL_RED;
			case 3 :  return GL_RGB;
			case 4 :  return GL_RGBA;
			default : return GL_FALSE;
		}
	}();

	if (format == GL_FALSE) 
	{
		std::string msg = "Internal format error in Texture()";
		Log::print(Log::render, Log::error, msg);
		stbi_image_free(buffer);
		return;
	}

	// make texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTextureStorage2D(m_id, 1, GL_RGBA8, w, h);
	glTextureSubImage2D(m_id, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, buffer);
	
	glGenerateTextureMipmap(m_id);
	
	stbi_image_free(buffer);
	m_type = type;
}

void Texture::render(int loc, Shader* shader)
{
	// textures should be in a material struct in fragment shader code
	std::string name = [&]()
	{
		switch (m_type)
		{
			case diffuse  : return "material.diffuse";
			case specular : return "material.specular";
			case ambient  : return "material.ambient";
			default       : return "";
		}
	}();

	shader->setUniform(name.c_str(), loc);
	glBindTextureUnit(loc, m_id);
}
