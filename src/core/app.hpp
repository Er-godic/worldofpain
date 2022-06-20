#pragma once
#include "window.hpp"

class App
{
	std::unique_ptr<Window> m_window;
public:
	App();
	void run();
	~App();
};
