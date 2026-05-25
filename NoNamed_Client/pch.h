#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "Debug\\NoNamedCore.lib")

#else
#pragma comment(lib, "Release\\NoNamedCore.lib")

#endif // _DEBUG

#include "CorePch.h"