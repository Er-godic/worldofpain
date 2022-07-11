#pragma once

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Lua
{
	lua_State* m_L;
public:
	Lua();
	~Lua();
	lua_State* operator()() {return m_L;}
	void doFile(const char*);
	void doString(const char*);
	void printStack();

	typedef int (*lua_CFunction) (lua_State* L);
	//void pushCFunction(const char* name, lua_CFunction);
};
