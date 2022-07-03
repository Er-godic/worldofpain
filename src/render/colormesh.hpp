#pragma once
#include "vertex.hpp"
#include "texture.hpp"

class ColorMesh
{
        uint m_vbo, m_ibo, m_num_indices;

public:
        ColorMesh(const std::vector<ColorVertex>&, const std::vector<uint>&);
        ~ColorMesh();
        void render(glm::mat4&);
};
