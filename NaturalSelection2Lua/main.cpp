#include "naturalselection.h"

bool isExecuted = false;
bool isAbleToExecute = false;

void Init()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	// Initialize all the lua signatures that we will need
	if (!lua.GetSignatures())
		MessageBoxA(NULL, "Lua: Failed to get signatures", NULL, NULL);

	if (!lua.Hook())
		MessageBoxA(NULL, "Lua: Failed to hook", NULL, NULL);

	while(true)
	{
		if (IsValidPtr(lua.World)) // is in game?
		{
			if (!isExecuted && isAbleToExecute)
			{
				Sleep(20000);
				printf("do\n");
				int r1 = lua.LoadStringA(lua.World->pState, "Print(\"Hello, World!\")");
				printf("r1: %i\n", r1);
				int r2 = lua.Call(lua.World->pState);
				printf("r2: %i\n", r2);
				isExecuted = true;
			}
		}
	}
	
}

DWORD __stdcall DllMain(HMODULE dll, DWORD callreason, void* reserved)
{
	if (callreason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Init, NULL, NULL, NULL); // We might not need to make a thread if we find something reliable to hook
		return 1;
	}
	return 0;
}