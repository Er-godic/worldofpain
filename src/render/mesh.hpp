#pragma once
#include "vertex.hpp"
#include "texture.hpp"

class Mesh
{
	uint m_vbo, m_ibo, m_num_indices;
	std::vector<Texture> m_textures;

public:
	Mesh(const std::vector<MeshVertex>&, const std::vector<uint>&, const std::vector<Texture>&);
	~Mesh();
        void render(glm::mat4&);
};
