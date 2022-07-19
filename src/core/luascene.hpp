#pragma once
#include "scene.hpp"
#include "render/node.hpp"
#include "render/strip.hpp"
#include "render/colormesh.hpp"
#include "render/mesh.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <new>

// add entity to scene registry /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int entity(lua_State* L)
{
	SceneManager* mng = SceneManager::s_scmanager;
	std::string name = luaL_checkstring(L, 1);
	void* p = lua_newuserdata(L, sizeof(entt::entity));
	new (p) entt::entity(mng->create(name));
	return 1;
}

// glm::vec3 in lua /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int vec3(lua_State* L)
{
	float r[3] = {0, 0, 0}; // default values
	for (int i = 0; i < 3; i++) 
		if (!lua_isnoneornil(L, i+1)) r[i] = luaL_checknumber(L, i+1);

       	void* p = lua_newuserdata(L, sizeof(glm::vec3));
	new (p) glm::vec3(r[0], r[1], r[2]);
	return 1;
}


// glm::quat in lua /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int quat(lua_State* L)
{
	float r[4] = {0, 0, 0, 1}; // default values, angle-axis representation
	for (int i = 0; i < 4; i++) 
		if (!lua_isnoneornil(L, i+1)) r[i] = luaL_checknumber(L, i+1);

	void* p = lua_newuserdata(L, sizeof(glm::quat));
	glm::vec3 axis(r[1], r[2], r[3]);
	glm::quat q = glm::angleAxis(glm::radians(r[0]), axis);
	new (p) glm::quat(q); 
	return 1;
}


// Node in lua //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int node(lua_State* L)
{
	glm::vec3* s = (glm::vec3*)lua_touserdata(L, 1);
	glm::quat* q = (glm::quat*)lua_touserdata(L, 2);
	glm::vec3* t = (glm::vec3*)lua_touserdata(L, 3);


	void* p = lua_newuserdata(L, sizeof(Node));
	new (p) Node(*s, *q, *t);
	return 1;
}

static int addNode(lua_State *L)
{
	SceneManager* mng    = SceneManager::s_scmanager;
	entt::entity* entity = (entt::entity*)lua_touserdata(L, 1);
	Node* node           = (Node*)lua_touserdata(L, 2);

	if (lua_isnoneornil(L, 3))
		mng->addNode(*entity, *node);
	else
	{
		entt::entity* parent = (entt::entity*)lua_touserdata(L, 3);
		mng->addNode(*entity, *node, *parent);
	}

	return 0;
}

}
