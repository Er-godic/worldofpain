#pragma once

class Shader
{
	uint m_shader_program;

public:
	Shader(const char* path);
	~Shader();

	uint id() const;
	void use() const;
};
