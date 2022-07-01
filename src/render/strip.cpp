#include "strip.hpp"
#include "glad/glad.h"

uint Strip::s_vao;
Shader* Strip::s_shader;

Strip::Strip(const std::vector<ColorVertex>& vertices)
{	
	m_num = vertices.size();	
	glCreateBuffers(1, &m_vbo);	
	glNamedBufferStorage(m_vbo, sizeof(ColorVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);
}

Strip::~Strip()
{
	glDeleteBuffers(1, &m_vbo);
}

void Strip::render(glm::mat4& world)
{
	s_shader->setUniform("W", world);
	
	// draw
	glVertexArrayVertexBuffer(s_vao, 0, m_vbo, 0, sizeof(ColorVertex));
	glDrawArrays(GL_LINE_STRIP, 0, m_num);
}

void Strip::init(Shader* shader)
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

void Strip::dinit()
{
	s_shader = nullptr;
	glDeleteVertexArrays(1, &s_vao);
}

void Strip::bind()
{
	s_shader->use();
	glBindVertexArray(s_vao);
}
