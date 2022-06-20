#include "app.hpp"
#include "clock.hpp"

App::App() 
{
	// init logging with spdlog
	Log::init();

	// create window and store pointer
	m_window = std::make_unique<Window>(1200, 750, "WOP");
	Window::s_win = m_window.get();
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
			// TO DO update simulation/physics/logic each time step
		}

		float L = lag.count() / frametime.count();
		// TO DO interpolate game states
		
		m_window->clear();
		// TO DO render
		m_window->swapBuffers();
	}
}

App::~App() {}
