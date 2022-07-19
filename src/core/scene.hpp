#pragma once
#include <entt/entt.hpp>
#include "lua.hpp"
#include "render/node.hpp"

// component that gives an entity a name
struct Name
{
	std::string name;
	Name(const std::string& _name) : name(_name) {}
};

class SceneManager
{
	entt::registry m_reg;
	Lua m_lua;
public:
	static SceneManager* s_scmanager;

	~SceneManager();
	entt::entity find(const std::string&);
	entt::entity create(const std::string&);
	void destroy(const std::string&);
	
	template <typename Component, typename... Args>
	decltype(auto) emplace(const entt::entity e, Args &&...args)
	{
		return m_reg.emplace<Component>(e, std::forward<Args>(args)...);
	}

	void addNode(const entt::entity, const Node&, const entt::entity = entt::null);

	void init();
	void update(float dt);
	void interpolate(float L);

	template <typename R>
	void renderIt()
        {
                auto view = m_reg.view<Node, R>();

                for (auto entity : view)
                {
			const auto [node, r] = view.template get<Node, R>(entity);
                        glm::mat4 world = node.world.mat4();
                        r->render(world);
                }
        };

	void render();
};
