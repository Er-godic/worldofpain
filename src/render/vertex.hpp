#pragma once
#include "core/stdafx.hpp"
#include <glm/glm.hpp>
#include "shader.hpp"

struct ColorVertex
{
	glm::vec3 pos;
	glm::vec3 color;

	static uint s_vao;
	static Shader* s_shader;
	static void init(Shader*);
	static void dinit();
	static void bind();
};

struct MeshVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;

	static uint s_vao;
	static Shader* s_shader;
	static void init(Shader*);
	static void dinit();
	static void bind();
};
