#pragma once
#include "core/stdafx.hpp"
#include "vertex.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Mesh
{
	uint m_vbo, m_ibo, m_num_indices;
	std::vector<Texture> m_textures;
       	static uint s_vao;
	static Shader* s_shader;

public:
	Mesh(const std::vector<MeshVertex>&, const std::vector<uint>&, const std::vector<Texture>&);
	~Mesh();
        void render(glm::mat4&);

	static void init(Shader*);
	static void dinit();
	static void bind();
};
