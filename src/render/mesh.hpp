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
	static Shader* s_shader;

public:
	Mesh(const std::vector<Vertex>&, const std::vector<uint>&);
	~Mesh();
        void render(glm::mat4&);

	static void init(Shader*);
	static void dinit();
	static void bind();
};
