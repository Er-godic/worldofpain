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


	// temporary texture quad code
	std::unique_ptr<Mesh> quad;
	{ 
		std::vector<MeshVertex> vertices(4);
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

        	quad = std::make_unique<Mesh>(vertices, indices, textures);

        	entt::entity e = m_scene.create("myquad");
		m_scene.emplace<Mesh*>(e, quad.get());

        	glm::vec3 s = glm::vec3(0.5f, 1.5f, 0.0f);
               	glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
               	glm::quat q = glm::angleAxis(glm::radians(20.0f), z_axis);
               	glm::vec3 t = glm::vec3(0.5f, 0.0f, 0.0f);
               	m_scene.emplace<Transform>(e, t, q, s);
	}
       
	// temporary color quad code
	std::unique_ptr<ColorMesh> quad2;
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
        	
		quad2 = std::make_unique<ColorMesh>(vertices, indices);

        	entt::entity e = m_scene.create("myquad2");
		m_scene.emplace<ColorMesh*>(e, quad2.get());

               	glm::vec3 s = glm::vec3(0.5f, 0.25f, 0.0f);
            	glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
                glm::quat q = glm::angleAxis(glm::radians(-15.0f), z_axis);
                glm::vec3 t = glm::vec3(-0.5f, -0.5f, 0.0f);
                m_scene.emplace<Transform>(e, t, q, s);
	}

	// temporary line strip code
	std::unique_ptr<Strip> strip;
	{
		std::vector<ColorVertex> vertices(4);
		vertices[0].pos = glm::vec3(0.5f,  0.5f, 0.0f);  
		vertices[1].pos = glm::vec3(0.5f, -0.7f, 0.0f);  
		vertices[2].pos = glm::vec3(-0.5f, -0.2f, 0.0f); 
		vertices[3].pos = glm::vec3(-0.6f,  0.5f, 0.0f);
		
		vertices[0].color = glm::vec3(0.8f, 0.2f, 0.2f);
		vertices[1].color = glm::vec3(0.6f, 0.2f, 0.4f);
		vertices[2].color = glm::vec3(0.4f, 0.4f, 0.6f);
		vertices[3].color = glm::vec3(0.2f, 0.6f, 0.4f);

		strip = std::make_unique<Strip>(vertices);
		entt::entity e = m_scene.create("mylines");
		m_scene.emplace<Strip*>(e, strip.get());
		
		Transform trans;
		m_scene.emplace<Transform>(e, trans);
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
