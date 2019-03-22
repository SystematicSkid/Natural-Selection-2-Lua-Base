#include "../naturalselection.h"
#include "../PolyHook.hpp"


PLH::VEHHook* RegisterClientHook;
PLH::VEHHook* LoadBufferHook;

typedef int(__fastcall* lua_pushstring_t)(lua_State* thisptr, const char* s);
lua_pushstring_t pushstring;
typedef __int64(__fastcall* luaL_loadbuffer_t)(lua_State* thisptr, const char* a2, DWORD64 size, const char* name);
luaL_loadbuffer_t oLoadBuffer;

typedef int(__cdecl* luaL_loadstring_t)(lua_State *L, const char *s);
luaL_loadstring_t loadstring;
typedef int(__cdecl* lua_pcall_t)(lua_State* L, int nargs, int nresults, int errfunc);
lua_pcall_t pcall;
typedef void(__fastcall* RegisterClientLoaded)(lua_State* L, __int64 a2, __int64 a3);
RegisterClientLoaded oRegisterClientLoaded;

bool Lua::GetSignatures()
{
	HMODULE lua51 = GetModuleHandle("lua51.dll");
	XASSERT(lua51);

	pushstringaddy = (ptr)GetProcAddress(lua51, "lua_pushstring");
	XASSERT(pushstringaddy);
	pushstring = (lua_pushstring_t)pushstringaddy;

	loadbuffer = (ptr)GetProcAddress(lua51, "luaL_loadbuffer");
	XASSERT(loadbuffer);

	loadstringaddy = (ptr)(GetProcAddress(lua51, "luaL_loadstring"));
	XASSERT(loadstringaddy);
	loadstring = (luaL_loadstring_t)loadstringaddy;

	pcalladdy = (ptr)(GetProcAddress(lua51, "lua_pcall"));
	XASSERT(pcalladdy);
	pcall = (lua_pcall_t)pcalladdy;

	registerclient = Memory::SigScan("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B F2", "NS2.exe");
	XASSERT(registerclient);

	return true;
}

void Lua::Execute(lua_State * L, const char * cmd)
{
	if (IsValidPtr(L))
	{
		int r1 = pushstring(L, cmd); // load the string...
		printf("r1: %i\n", r1);
		int r2 = pcall(L, 0, LUA_MULTRET, 0);
		printf("r2: %i\n", r2);
	}
}

int Lua::LoadString(lua_State* L, const char* s)
{
	return loadstring(L, s);
}

int Lua::Call(lua_State* L)
{
	return pcall(L, 0, LUA_MULTRET, 0);
}

void __fastcall hkRegisterClientLoaded(lua_State* L, __int64 a2, ClientWorld* pWorld)
{
	auto ProtectionObject = RegisterClientHook->GetProtectionObject();
	printf("Hook!\n");
	printf("ClientGame: 0x%p\nWorld: 0x%p\n", a2, pWorld);
	lua.World = pWorld;
	return oRegisterClientLoaded(L, a2, a2);
}

int __cdecl hkLoadBuffer(lua_State* thisptr, const char* a2, size_t size, const char* name)
{
	auto ProtectionObject = LoadBufferHook->GetProtectionObject();
	printf("Name: %s\n", name);
	if (!strcmp(name, "@lua/GUIPing.lua"))
	{
		isAbleToExecute = true;
	}

	return oLoadBuffer(thisptr, a2, size, name);
}

bool Lua::Hook()
{
	if (!pushstring)
		return false;

	RegisterClientHook = new PLH::VEHHook();
	RegisterClientHook->SetupHook((BYTE*)registerclient, (BYTE*)hkRegisterClientLoaded, PLH::VEHHook::VEHMethod::INT3_BP);
	if (!RegisterClientHook->Hook())
		return false;
	oRegisterClientLoaded = RegisterClientHook->GetOriginal<RegisterClientLoaded>();

	LoadBufferHook = new PLH::VEHHook();
	LoadBufferHook->SetupHook((BYTE*)loadbuffer, (BYTE*)hkLoadBuffer, PLH::VEHHook::VEHMethod::INT3_BP);
	if (!LoadBufferHook->Hook())
		return false;
	oLoadBuffer = LoadBufferHook->GetOriginal<luaL_loadbuffer_t>();

	return true;
}

Lua lua;