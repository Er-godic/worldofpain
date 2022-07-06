#pragma once
#include <entt/entt.hpp>
#include "render/transform.hpp"

// component that gives an entity a name
struct Name
{
	std::string name;
	Name(const std::string& _name) : name(_name) {}
};

#define MAX_CHILDREN 10

// node component that allows transform hierarchy
struct Node
{
	unsigned char size = 0;
	std::array<entt::entity, MAX_CHILDREN> children{};
	Transform local;
	Transform world;

	Node(const glm::vec3&, const glm::quat&, const glm::vec3&);
	bool addChild(entt::entity);
	bool removeChild(entt::entity);
	void concatenate(entt::registry*, const Transform&);
};

// parent component that allows transform hierarchy
struct Parent
{
	entt::entity entity;
	Parent(entt::entity e) : entity(e) {}
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

	void addNode(const entt::entity, const Node&, const entt::entity = entt::null);

	void update(float dt);
	void interpolate(float L);

	template <typename R>
	void renderIt()
        {
                auto view = m_scene.view<Node, R>();

                for (auto entity : view)
                {
			const auto [node, r] = view.template get<Node, R>(entity);
                        glm::mat4 world = node.world.mat4();
                        r->render(world);
                }
        };

	void render();
};
