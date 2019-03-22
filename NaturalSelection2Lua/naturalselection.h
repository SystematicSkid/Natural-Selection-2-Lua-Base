#pragma once

// win
#include <Windows.h>
#include <winternl.h>
#include <Xinput.h>
#include <comdef.h>
#include <minwindef.h>
#include <winbase.h>
#include <intrin.h>
#include <psapi.h>

// stl
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <clocale>
#include <algorithm>
#include <sstream>

#include "memory.h"

#define PTR_MAX_VALUE ((PVOID)0x000F000000000000)
_forceinline bool IsValidPtr(PVOID p) { return (p >= (PVOID)0x10000) && (p < PTR_MAX_VALUE); }

//debug
#define XASSERT( x ) if( !x ) {MessageBoxW( 0, L#x, 0, 0 ); return false;}

extern bool isAbleToExecute;

#include "lua\lua.h"