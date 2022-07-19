#pragma once
#include "scene.hpp"
#include "window.hpp"
#include "render/shader.hpp"

class App
{
	std::unique_ptr<Window> m_window;
	SceneManager m_scmanager;
	std::unordered_map<std::string, Shader> m_shaders;
public:
	App();
	void run();
	~App();
};
