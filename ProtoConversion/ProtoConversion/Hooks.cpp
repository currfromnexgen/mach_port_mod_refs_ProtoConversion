#include "Hooks.h"
#include <Windows.h>

void* placeHook(DWORD Address, void* Hook, bool Revert = false) { /* Credits go to Louka/sloppey for this function. */
	DWORD OldProtection;
	if (!Revert) {
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)Address, sizeof(void*) * 4);
		VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &OldProtection);
		*(char*)Address = 0xE9; *(DWORD*)(Address + 1) = (DWORD)Hook - Address - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)Address, 1, OldProtection, &OldProtection);
		return result;
	}
	else {
		VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &OldProtection);
		memcpy((void*)Address, Hook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)Address, 1, OldProtection, &OldProtection);
		return NULL;
	}
}