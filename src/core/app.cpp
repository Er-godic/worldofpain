#include "app.hpp"
#include "clock.hpp"
#include "render/strip.hpp"
#include "render/mesh.hpp"
#include "render/transform.hpp"
#include "render/texture.hpp"

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
	Strip::init(&m_shaders.at("color"));
	Mesh::init(&m_shaders.at("mesh"));
}

void App::run()
{
	// game loop, https://gafferongames.com/post/fix_your_timestep
	
	Clock clock;
	Clock::seconds dt	     = Clock::seconds(0);
	Clock::seconds lag           = Clock::seconds(0);
	Clock::seconds frametime     = Clock::seconds(1. / 60);
	Clock::seconds max_frametime = Clock::seconds(250. / 1000); 	
	
	// temporary line strip code
	std::vector<ColorVertex> vertices(4);
	vertices[0].pos = glm::vec3(0.5f,  0.5f, 0.0f);  
        vertices[1].pos = glm::vec3(0.5f, -0.7f, 0.0f);  
        vertices[2].pos = glm::vec3(-0.5f, -0.2f, 0.0f); 
        vertices[3].pos = glm::vec3(-0.6f,  0.5f, 0.0f);

	vertices[0].color = glm::vec3(0.8f, 0.2f, 0.2f);
	vertices[1].color = glm::vec3(0.6f, 0.2f, 0.4f);
	vertices[2].color = glm::vec3(0.4f, 0.4f, 0.6f);
	vertices[3].color = glm::vec3(0.2f, 0.6f, 0.4f);
	
	Strip strip(vertices);

	entt::entity e = m_scene.create("mylines");
	m_scene.emplace<Strip*>(e, &strip);

	Transform trans;
	m_scene.emplace<Transform>(e, trans);

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
	Mesh::dinit();
	Strip::dinit();

	// dinit shaders
	m_shaders.clear();
}
