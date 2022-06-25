#include "mesh.hpp"
#include "glad/glad.h"

uint Mesh::s_vao;
Shader* Mesh::s_shader;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
{
	m_num_indices = indices.size();

	glCreateBuffers(1, &m_vbo);	
	glNamedBufferStorage(m_vbo, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &m_ibo);
	glNamedBufferStorage(m_ibo, sizeof(uint) * indices.size(), &indices[0], GL_DYNAMIC_STORAGE_BIT);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::render(glm::mat4& world)
{
	s_shader->setUniform("W", world);

	// draw indexed triangles
	glVertexArrayVertexBuffer(s_vao, 0, m_vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(s_vao, m_ibo);
	glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
}

void Mesh::init(Shader* shader)
{
	glCreateVertexArrays(1, &s_vao);

	for(uint i = 0; i < 3; i++)
		glEnableVertexArrayAttrib(s_vao, i);

	glVertexArrayAttribFormat(s_vao, 0, 3, GL_FLOAT, false, offsetof(Vertex, pos));
	glVertexArrayAttribFormat(s_vao, 1, 3, GL_FLOAT, false, offsetof(Vertex, normal));
	glVertexArrayAttribFormat(s_vao, 2, 2, GL_FLOAT, false, offsetof(Vertex, tex));

	for(uint i = 0; i < 3; i++)
		glVertexArrayAttribBinding(s_vao, i, 0);

	s_shader = shader;
}


void Mesh::dinit()
{
	s_shader = nullptr;
	glDeleteVertexArrays(1, &s_vao);
}

void Mesh::bind()
{
	s_shader->use();
	glBindVertexArray(s_vao);
}
