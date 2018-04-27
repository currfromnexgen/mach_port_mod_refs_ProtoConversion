#pragma once
#include <Windows.h>
#include <exception>

void* placeHook(DWORD Address, void* Hook, bool Revert = false);
__declspec(naked) void INT3BreakpointHook();