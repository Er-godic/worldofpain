#pragma once
#include "scene.hpp"
#include "window.hpp"
#include "render/shader.hpp"

class App
{
	Scene m_scene;
	std::unique_ptr<Window> m_window;
	std::unordered_map<std::string, Shader> m_shaders;
public:
	App();
	void run();
	~App();
};
