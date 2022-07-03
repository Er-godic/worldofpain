#include "strip.hpp"
#include "glad/glad.h"

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
	Shader* shader = ColorVertex::s_shader;
	uint vao = ColorVertex::s_vao;
	
	shader->setUniform("W", world);
	
	// draw
	glVertexArrayVertexBuffer(vao, 0, m_vbo, 0, sizeof(ColorVertex));
	glDrawArrays(GL_LINE_STRIP, 0, m_num);
}
