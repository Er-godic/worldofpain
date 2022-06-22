#pragma once
#include "core/stdafx.hpp"
#include <glm/glm.hpp>
#include "shader.hpp"

struct Vertex
{ 
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
};

class Mesh
{
	uint m_vbo, m_ibo, m_num_indices;
       	static uint s_vao;
	static const Shader* s_shader;

public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
	~Mesh();
        void render();

	static void init(Shader* shader);
	static void dinit();
	static void bind();
};
