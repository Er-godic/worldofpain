#include "app.hpp"
#include "clock.hpp"
#include "render/strip.hpp"
#include "render/mesh.hpp"
#include "render/colormesh.hpp"

App::App() 
{
	// init logging with spdlog
	Log::init();

	// create window and store pointer
	m_window = std::make_unique<Window>(1200, 750, "WOP");
	Window::s_win = m_window.get();

	// init shaders (use emplace rather than [] so that the destructor isn't called)
	for (auto& p : std::filesystem::directory_iterator("resources/shaders"))
	{
		std::string path = p.path().string();
		size_t loc = path.find_last_of("/") + 1;
		std::string name = path.substr(loc, path.rfind('.') - loc);
		m_shaders.emplace(name, path.c_str());
	}

	// generate formats, use at() not []
	ColorVertex::init(&m_shaders.at("color"));
	MeshVertex::init(&m_shaders.at("mesh"));

	// init scene
	m_scene.init();
}

void App::run()
{
	// game loop, https://gafferongames.com/post/fix_your_timestep
	
	Clock clock;
	Clock::seconds dt	     = Clock::seconds(0);
	Clock::seconds lag           = Clock::seconds(0);
	Clock::seconds frametime     = Clock::seconds(1. / 60);
	Clock::seconds max_frametime = Clock::seconds(250. / 1000); 	
     	
	clock.start();
	while (m_window->isOpen())
	{
		dt = clock.tick();
		dt = dt < max_frametime ? dt : max_frametime; // prevent 'spiral of death'

		m_window->pollInput();

		for (lag += dt; lag >= frametime; lag -= frametime)
		{
			m_scene.update(dt.count());
		}

		float L = lag.count() / frametime.count();
		m_scene.interpolate(L);
		
		m_window->clear();
		m_scene.render();
		m_window->swapBuffers();
	}
}

App::~App() 
{
	// delete formats
	MeshVertex::dinit();
	ColorVertex::dinit();

	// dinit shaders
	m_shaders.clear();
}
