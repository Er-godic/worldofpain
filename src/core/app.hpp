#pragma once
#include "window.hpp"
#include "render/shader.hpp"

class App
{
	std::unique_ptr<Window> m_window;
	std::unordered_map<std::string, Shader> m_shaders;
public:
	App();
	void run();
	~App();
};
