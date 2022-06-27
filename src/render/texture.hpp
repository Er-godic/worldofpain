#pragma once
#include "shader.hpp"

class Texture
{
public:	
	enum TextureType
	{
		none = 0, diffuse, specular, ambient
	};

private:
	uint m_id;
	TextureType m_type = TextureType::none;

public:
	Texture(const std::string&, TextureType);
	void render(int, Shader*);
};
