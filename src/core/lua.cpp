#include "lua.hpp"

Lua::Lua()
{
	// create new virtual machine state
	m_L = luaL_newstate();
	// load standard lua libraries
	luaL_openlibs(m_L);
}

Lua::~Lua() {lua_close(m_L);}

void Lua::doFile(const char* path)
{
	if (luaL_dofile(m_L, path) != LUA_OK)
		Log::print(Log::core, Log::error, lua_tostring(m_L, -1));
}

void Lua::doString(const char* str)
{
	if (luaL_dostring(m_L, str) != LUA_OK)
		Log::print(Log::core, Log::error, lua_tostring(m_L, -1));
}

void Lua::printStack()
{
	int top = lua_gettop(m_L);

	std::string str = "From top to bottom, the lua stack is \n";
	for (uint index = top; index > 0; index--)
	{
		int type = lua_type(m_L, index);
		switch(type)
		{
			case LUA_TSTRING : 
				str = str + lua_tostring(m_L, index); 
				break;
			case LUA_TBOOLEAN : 
				str = str + (lua_toboolean(m_L, index) ? "true" : "false");
				break;
			case LUA_TNUMBER : 
				str = str + std::to_string(lua_tonumber(m_L, index));
				break;
			default : 
				str = str + lua_typename(m_L, type);
				break;
		}
		
		str = str + "\n";
	}
	
	str = str + "\n";
	Log::print(Log::core, Log::error, str);
}

void Lua::pushGlobalFnc(const char* name, lua_CFunction funct)
{
	lua_pushcfunction(m_L, funct);
	lua_setglobal(m_L, name);
}
