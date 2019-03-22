#pragma once

extern "C" {
# include "lua.h"
# include "luajit.h"
# include "lauxlib.h"
# include "lualib.h"
}

#pragma comment(lib, "../lua/lua5.1.3-static.lib")
#pragma comment(lib, "../lua/lua51.lib") 
#pragma comment(lib, "../lua/luajit.lib")

#include "..\sdk\world.h"

class Lua
{
private:
	ptr pushstringaddy;
	ptr loadbuffer;
	ptr loadstringaddy;
	ptr pcalladdy;
	ptr registerclient;

	
public:
	lua_State* L;
	ClientWorld* World = nullptr;
	bool GetSignatures();
	bool Hook();
	static void Execute(lua_State* L, const char* cmd);
	lua_State* GetLuaState();
	ClientWorld* GetWorld();

	int LoadString(lua_State* L, const char* s);
	int Call(lua_State* L);
};

extern Lua lua;