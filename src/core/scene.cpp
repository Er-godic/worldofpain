#include "scene.hpp"
#include "render/strip.hpp"
#include "render/colormesh.hpp"
#include "render/mesh.hpp"

Node::Node(const glm::vec3& t, const glm::quat& q, const glm::vec3& s)
	: local(t, q, s) {}

bool Node::addChild(entt::entity entity)
{
	if (size == MAX_CHILDREN)
	{
		const char* msg = "Child limit exceeded in Node::addChild()";
		Log::print(Log::core, Log::error, msg);
		return false;
	}

	children[size] = entity;
	size += 1;
	return true;
}

bool Node::removeChild(entt::entity entity)
{
	for (unsigned char i = 0; i < size; i++)
	{
		if (entity == children[i])
		{
			for (unsigned char j = i; j < size - 1; j++)
				children[j] = children[j + 1];

			children[size - 1] = entt::null;
			size -= 1;
			return true;
		}
	}

	const char* msg = "Entity is not a child in Node::removeChild()";
	Log::print(Log::core, Log::error, msg);
	return false;
}

void Node::concatenate(entt::registry* registry, const Transform& parent)
{
	world = local * parent;
	
	for(unsigned char i = 0; i < size; i++) 
	{
		Node& child = registry->get<Node>(children[i]);
		child.concatenate(registry, world);
	}
}

entt::entity Scene::find(const std::string& name)
{
	auto view = m_scene.view<Name>();
	for (auto entity : view)
	{
		std::string& temp  = view.get<Name>(entity).name;
		if (name == temp) return entity;
	}
	
	const char* msg = "Name not found in find()";
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
			const char* msg = "Duplicate name in create()";
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
		const char* msg = "find() failure in destroy()";
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
			const char* msg = "Parent in addNode() is not null and lacks node";
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
