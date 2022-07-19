#include "scene.hpp"
#include "luascene.hpp"

SceneManager* SceneManager::s_scmanager;

void SceneManager::init()
{
	m_lua.pushGlobalFnc("Entity", entity);
	m_lua.pushGlobalFnc("Vec3", vec3);
	m_lua.pushGlobalFnc("Quat", quat);
	m_lua.pushGlobalFnc("Node", node);
	m_lua.pushGlobalFnc("addNode", ::addNode);
	
	m_lua.doFile("resources/scripts/scmanager.lua");
}

SceneManager::~SceneManager()
{
	m_reg.clear();
}

entt::entity SceneManager::find(const std::string& name)
{
	auto view = m_reg.view<Name>();
	for (auto entity : view)
	{
		std::string& temp  = view.get<Name>(entity).name;
		if (name == temp) return entity;
	}
	
	const char* msg = "Name not found in SceneManager::find()";
	Log::print(Log::core, Log::error, msg);
	return entt::null;
}

entt::entity SceneManager::create(const std::string& name)
{
	// make sure the label is unique
	auto view = m_reg.view<Name>();
	for (auto entity : view)
	{
		auto& temp = view.get<Name>(entity).name;
		if (name == temp)
		{
			const char* msg = "Duplicate name in SceneManager::create()";
			Log::print(Log::core, Log::error, msg);
			return entt::null;
		}
	}	

	// make entity
	auto entity = m_reg.create();
	m_reg.emplace<Name>(entity, name);
	return entity;
}

void SceneManager::destroy(const std::string& name)
{
	entt::entity e = find(name);
	if (e == entt::null)
	{
		const char* msg = "find() failure in SceneManager::destroy()";
		Log::print(Log::core, Log::error, msg);
	}
	else m_reg.destroy(e);
}

void SceneManager::addNode(const entt::entity entity, const Node& node, const entt::entity parent)
{
	if (parent != entt::null)
	{
		// check if parent has node component
		bool has_node = m_reg.all_of<Node>(parent);
		if (!has_node)
		{
			const char* msg = "Parent in SceneManager::addNode() is not null and lacks node";
			Log::print(Log::core, Log::error, msg);
			return;
		}
		
		// add node as child to parent
		Node& parent_node = m_reg.get<Node>(parent);
		if(!parent_node.addChild(entity)) return;
		
		// node not root, add parent component to entity
		m_reg.emplace<Parent>(entity, parent);
	}

	m_reg.emplace<Node>(entity, node);
}

void SceneManager::update(float dt) {}

void SceneManager::interpolate(float L) {}

void SceneManager::render()
{	
	// concatenate up each tree from the root
	auto group = m_reg.group<Node>({}, entt::exclude<Parent>);
	for (auto entity : group)
	{
		Node& node = group.get<Node>(entity);
		Transform identity;
		node.concatenate(&m_reg, identity);
	}

	// color vertex
	ColorVertex::bind();
	renderIt<Strip*>();
	renderIt<ColorMesh*>();

	// mesh vertex
	MeshVertex::bind();
	renderIt<Mesh*>();	
}
