#pragma once
#include <entt/entt.hpp>

// component that gives an entity a name
struct Name
{
	std::string name;
	Name(const std::string& _name) : name(_name) {}
};

// scene class for entities
class Scene
{
	entt::registry m_scene;
public:
	~Scene() { m_scene.clear(); }
	entt::entity find(const std::string&);
	entt::entity create(const std::string&);
	void destroy(const std::string&);
	
	template <typename Component, typename... Args>
	decltype(auto) emplace(const entt::entity e, Args &&...args)
	{
		return m_scene.emplace<Component>(e, std::forward<Args>(args)...);
	}

	void update(float dt);
	void interpolate(float L);
	void render();
};
