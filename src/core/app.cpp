#include "app.hpp"
#include "clock.hpp"
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

	// generate mesh format, use at() not []
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
	
	// temporary quad code
	std::vector<Vertex> vertices(4);
	vertices[0].pos = glm::vec3(0.5f,  0.5f, 0.0f);  
        vertices[1].pos = glm::vec3(0.5f, -0.5f, 0.0f);  
        vertices[2].pos = glm::vec3(-0.5f, -0.5f, 0.0f); 
        vertices[3].pos = glm::vec3(-0.5f,  0.5f, 0.0f);

	vertices[0].tex = glm::vec2(1.0f, 1.0f);
	vertices[1].tex = glm::vec2(1.0f, 0.0f);
	vertices[2].tex = glm::vec2(0.0f, 0.0f);
	vertices[3].tex = glm::vec2(0.0f, 1.0f);

	std::vector<uint> indices = { 0, 1, 3, 1, 2, 3};

	std::vector<Texture> textures;
	textures.emplace_back("resources/textures/test.png", Texture::ambient);

	Mesh quad(vertices, indices, textures);

	entt::entity e = m_scene.create("myquad");
	m_scene.emplace<Mesh*>(e, &quad);

	{
		glm::vec3 s = glm::vec3(0.5f, 1.5f, 0.0f);
		glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::quat q = glm::angleAxis(glm::radians(20.0f), z_axis);
		glm::vec3 t = glm::vec3(0.5f, 0.0f, 0.0f);
		m_scene.emplace<Transform>(e, t, q, s);
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
	// delete mesh format
	Mesh::dinit();

	// dinit shaders
	m_shaders.clear();
}
