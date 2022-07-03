#include "colormesh.hpp"
#include "glad/glad.h"

ColorMesh::ColorMesh(const std::vector<ColorVertex>& vertices, const std::vector<uint>& indices)
        : m_num_indices(indices.size())
{
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, sizeof(ColorVertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);

        glCreateBuffers(1, &m_ibo);
        glNamedBufferStorage(m_ibo, sizeof(uint) * indices.size(), &indices[0], GL_DYNAMIC_STORAGE_BIT);
}

ColorMesh::~ColorMesh()
{
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);
}

void ColorMesh::render(glm::mat4& world)
{
        Shader* shader = ColorVertex::s_shader;
        uint vao = ColorVertex::s_vao;

        shader->setUniform("W", world);

        // draw indexed triangles
        glVertexArrayVertexBuffer(vao, 0, m_vbo, 0, sizeof(ColorVertex));
        glVertexArrayElementBuffer(vao, m_ibo);
        glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
}
