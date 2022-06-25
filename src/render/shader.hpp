#pragma once
#include <glm/glm.hpp>

class Shader
{
	uint m_shader_program;
	std::unordered_map<std::string, int> m_uniformloc;

public:
	Shader(const char* path);
	~Shader();

	uint id() const;
	void use() const;

	int getloc (const std::string& name);
	void setUniform(const std::string& name, glm::mat4& mat4);
};
