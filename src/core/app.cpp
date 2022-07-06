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
}

void App::run()
{
	// game loop, https://gafferongames.com/post/fix_your_timestep
	
	Clock clock;
	Clock::seconds dt	     = Clock::seconds(0);
	Clock::seconds lag           = Clock::seconds(0);
	Clock::seconds frametime     = Clock::seconds(1. / 60);
	Clock::seconds max_frametime = Clock::seconds(250. / 1000); 	
       
	// temporary color quad code
	std::unique_ptr<ColorMesh> quad;
	{ 
		std::vector<ColorVertex> vertices(4);
        	vertices[0].pos = glm::vec3(0.5f,  0.5f, 0.0f);
        	vertices[1].pos = glm::vec3(0.5f, -0.5f, 0.0f);
        	vertices[2].pos = glm::vec3(-0.5f, -0.5f, 0.0f);
        	vertices[3].pos = glm::vec3(-0.5f,  0.5f, 0.0f);

        	vertices[0].color = glm::vec3(0.5f, 0.5f, 0.5f);
        	vertices[1].color = glm::vec3(0.7f, 0.3f, 0.5f);
        	vertices[2].color = glm::vec3(0.7f, 0.5f, 0.3f);
        	vertices[3].color = glm::vec3(0.3f, 0.8f, 0.8f);

        	std::vector<uint> indices = { 0, 1, 3, 1, 2, 3};
        	
		quad = std::make_unique<ColorMesh>(vertices, indices);

        	entt::entity e1 = m_scene.create("myquad1");
        	entt::entity e2 = m_scene.create("myquad2");
		m_scene.emplace<ColorMesh*>(e1, quad.get());
		m_scene.emplace<ColorMesh*>(e2, quad.get());

               	glm::vec3 s = glm::vec3(0.5f, 0.5f, 0.0f);
            	glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
                glm::quat q = glm::angleAxis(glm::radians(45.0f), z_axis);
                glm::vec3 t = glm::vec3(-0.5f, 0.0f, 0.0f);
		Node nd = Node(t, q, s);
                
		m_scene.addNode(e1, nd);
		m_scene.addNode(e2, nd, e1);
	}

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
