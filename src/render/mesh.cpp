#include "mesh.hpp"
#include "glad/glad.h"

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint>& indices, 
	const std::vector<Texture>& textures) : m_num_indices(indices.size()), m_textures(textures)
{	
	glCreateBuffers(1, &m_vbo);	
	glNamedBufferStorage(m_vbo, sizeof(MeshVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);

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
	Shader* shader = MeshVertex::s_shader;
	uint vao = MeshVertex::s_vao;

	shader->setUniform("W", world);
	
	for(uint i = 0; i < m_textures.size(); i++)
		m_textures[i].render(i, shader);

	// draw indexed triangles
	glVertexArrayVertexBuffer(vao, 0, m_vbo, 0, sizeof(MeshVertex));
	glVertexArrayElementBuffer(vao, m_ibo);
	glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
}
