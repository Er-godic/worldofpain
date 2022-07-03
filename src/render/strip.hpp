#pragma once
#include "vertex.hpp"

class Strip
{
	uint m_vbo, m_num;

public:
	Strip(const std::vector<ColorVertex>&);
	~Strip();
        void render(glm::mat4&);
};
