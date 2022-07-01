#pragma once
#include <glm/glm.hpp>

struct ColorVertex
{
	glm::vec3 pos;
	glm::vec3 color;
};

struct MeshVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
};
