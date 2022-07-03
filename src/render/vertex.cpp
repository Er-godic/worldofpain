#include "vertex.hpp"
#include "glad/glad.h"

uint ColorVertex::s_vao;
uint MeshVertex::s_vao;
Shader* ColorVertex::s_shader;
Shader* MeshVertex::s_shader;

void ColorVertex::init(Shader* shader)
{
	glCreateVertexArrays(1, &s_vao);

	for (uint i = 0; i < 2; i++)
		glEnableVertexArrayAttrib(s_vao, i);

	glVertexArrayAttribFormat(s_vao, 0, 3, GL_FLOAT, false, offsetof(ColorVertex, pos));
	glVertexArrayAttribFormat(s_vao, 1, 3, GL_FLOAT, false, offsetof(ColorVertex, color));

	for (uint i = 0; i < 2; i++)
		glVertexArrayAttribBinding(s_vao, i, 0);

	s_shader = shader;
}

void ColorVertex::dinit()
{
	s_shader = nullptr;
	glDeleteVertexArrays(1, &s_vao);
}

void ColorVertex::bind()
{
	s_shader->use();
	glBindVertexArray(s_vao);
}

void MeshVertex::init(Shader* shader)
{
	glCreateVertexArrays(1, &s_vao);

	for(uint i = 0; i < 3; i++)
		glEnableVertexArrayAttrib(s_vao, i);

	glVertexArrayAttribFormat(s_vao, 0, 3, GL_FLOAT, false, offsetof(MeshVertex, pos));
	glVertexArrayAttribFormat(s_vao, 1, 3, GL_FLOAT, false, offsetof(MeshVertex, normal));
	glVertexArrayAttribFormat(s_vao, 2, 2, GL_FLOAT, false, offsetof(MeshVertex, tex));

	for(uint i = 0; i < 3; i++)
		glVertexArrayAttribBinding(s_vao, i, 0);

	s_shader = shader;
}


void MeshVertex::dinit()
{
	s_shader = nullptr;
	glDeleteVertexArrays(1, &s_vao);
}

void MeshVertex::bind()
{
	s_shader->use();
	glBindVertexArray(s_vao);
}
