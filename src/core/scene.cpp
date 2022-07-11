#include "scene.hpp"
#include "lua.hpp"
#include "render/strip.hpp"
#include "render/colormesh.hpp"
#include "render/mesh.hpp"

void Scene::init()
{
	Lua script;
	script.doFile("resources/scripts/test.lua");

	auto* L = script();	
	lua_getglobal(L, "a");
	lua_getglobal(L, "b");
	lua_getglobal(L, "c");
	lua_getglobal(L, "d");
	script.printStack();
}

entt::entity Scene::find(const std::string& name)
{
	auto view = m_scene.view<Name>();
	for (auto entity : view)
	{
		std::string& temp  = view.get<Name>(entity).name;
		if (name == temp) return entity;
	}
	
	const char* msg = "Name not found in Scene::find()";
	Log::print(Log::core, Log::error, msg);
	return entt::null;
}

entt::entity Scene::create(const std::string& name)
{
	// make sure the label is unique
	auto view = m_scene.view<Name>();
	for (auto entity : view)
	{
		auto& temp = view.get<Name>(entity).name;
		if (name == temp)
		{
			const char* msg = "Duplicate name in Scene::create()";
			Log::print(Log::core, Log::error, msg);
			return entt::null;
		}
	}	

	// make entity
	auto entity = m_scene.create();
	m_scene.emplace<Name>(entity, name);
	return entity;
}

void Scene::destroy(const std::string& name)
{
	entt::entity e = find(name);
	if (e == entt::null)
	{
		const char* msg = "find() failure in Scene::destroy()";
		Log::print(Log::core, Log::error, msg);
	}
	else m_scene.destroy(e);
}

void Scene::addNode(const entt::entity entity, const Node& node, const entt::entity parent)
{
	if (parent != entt::null)
	{
		// check if parent has node component
		bool has_node = m_scene.all_of<Node>(parent);
		if (!has_node)
		{
			const char* msg = "Parent in Scene::addNode() is not null and lacks node";
			Log::print(Log::core, Log::error, msg);
			return;
		}
		
		// add node as child to parent
		Node& parent_node = m_scene.get<Node>(parent);
		if(!parent_node.addChild(entity)) return;
		
		// node not root, add parent component to entity
		m_scene.emplace<Parent>(entity, parent);
	}

	m_scene.emplace<Node>(entity, node);
}

void Scene::update(float dt) {}

void Scene::interpolate(float L) {}

void Scene::render()
{	
	// concatenate up each tree from the root
	auto group = m_scene.group<Node>({}, entt::exclude<Parent>);
	for (auto entity : group)
	{
		Node& node = group.get<Node>(entity);
		Transform identity;
		node.concatenate(&m_scene, identity);
	}

	// color vertex
	ColorVertex::bind();
	renderIt<Strip*>();
	renderIt<ColorMesh*>();

	// mesh vertex
	MeshVertex::bind();
	renderIt<Mesh*>();	
}
