#include "app.hpp"
#include "clock.hpp"

App::App() {}

void App::run()
{
	// game loop, https://gafferongames.com/post/fix_your_timestep
	Clock clock;
	Clock::seconds dt	     = Clock::seconds(0);
	Clock::seconds lag           = Clock::seconds(0);
	Clock::seconds frametime     = Clock::seconds(1. / 60);
	Clock::seconds max_frametime = Clock::seconds(250. / 1000); // prevent 'spiral of death'
	
	clock.start();
	while (true)
	{
		dt = clock.tick();
		dt = dt < max_frametime ? dt : max_frametime;

		// poll input

		for (lag += dt; lag >= frametime; lag -= frametime)
		{
			// update simulation/phyiscs/logic time step
		}

		float L = lag.count() / frametime.count();
		// interpolate game states
		
		// clear window
		// render
		// swap buffers
	}
}

App::~App() {}
