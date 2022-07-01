#pragma once
#include "core/stdafx.hpp"
#include "vertex.hpp"
#include "shader.hpp"

class Strip
{
	uint m_vbo, m_num;
       	static uint s_vao;
	static Shader* s_shader;

public:
	Strip(const std::vector<ColorVertex>&);
	~Strip();
        void render(glm::mat4&);

	static void init(Shader*);
	static void dinit();
	static void bind();
};
