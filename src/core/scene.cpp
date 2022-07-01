#include "scene.hpp"
#include "render/strip.hpp"
#include "render/mesh.hpp"
#include "render/transform.hpp"

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

void Scene::update(float dt) {}

void Scene::interpolate(float L) {}

void Scene::render()
{
	// render line strips
	{	
		Strip::bind();
		auto view = m_scene.view<Strip*, Transform>();
		
		for (auto entity : view)
		{
			const auto [strip, transform] = view.get<Strip*, Transform>(entity);
			glm::mat4 world = transform.mat4();
			strip->render(world);
		}
	}

	// render meshes
	{	
		Mesh::bind();
		auto view = m_scene.view<Mesh*, Transform>();
		
		for (auto entity : view)
		{
			const auto [mesh, transform] = view.get<Mesh*, Transform>(entity);
			glm::mat4 world = transform.mat4();
			mesh->render(world);
		}
	}
}
