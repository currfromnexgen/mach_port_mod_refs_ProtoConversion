#include "Convert.h"
#include "Hooks.h"
#include "RLua.h"
#include <Windows.h>
#include <iostream>
#include <sstream>

#define ASLR(x)(x - 0x400000 + (DWORD)GetModuleHandle(NULL))

DWORD* ScriptContext, ScriptContextVFTable;

bool Compare(const char* pData, const char* bMask, const char* szMask) {
	while (*szMask) {
		if (*szMask != '?') {
			if (*pData != *bMask) {
				return 0;
			};
		};
		++szMask, ++pData, ++bMask;
	};
	return 1;
};

DWORD ScanVftable(const char* _Vftable) {
	MEMORY_BASIC_INFORMATION MBI = { 0 };
	SYSTEM_INFO SI = { 0 };
	GetSystemInfo(&SI);
	DWORD Start = (DWORD)SI.lpMinimumApplicationAddress;
	DWORD End = (DWORD)SI.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)Start, &MBI, sizeof(MBI))) {
			if ((MBI.Protect & PAGE_READWRITE) && !(MBI.Protect & PAGE_GUARD)) {
				for (DWORD Vftable = (DWORD)(MBI.BaseAddress); Vftable - (DWORD)(MBI.BaseAddress) < MBI.RegionSize; ++Vftable) {
					if (Compare((const char*)Vftable, _Vftable, "xxxx"))
						return Vftable;
				};
			};
			Start += MBI.RegionSize;
		};
	} while (Start < End);
	return 0;
};

std::string GetInput() {
	std::string Input;
	getline(std::cin, Input);
	return Input;
}

VOID Main() {
	FILE* jFile;
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(FreeConsole), "\xC3", 1, NULL);
	AllocConsole();
	SetConsoleTitle("zVA | By mach_port_mod_refs#9156");
	freopen_s((FILE**)jFile, "CONOUT$", "w", stdout);
	freopen_s((FILE**)jFile, "CONIN$", "r", stdin);

	ScriptContextVFTable = ASLR(0x111915C);
	ScriptContext = (DWORD*)ScanVftable((char*)&ScriptContextVFTable);
	int r_LS = ScriptContext[41] - (DWORD)&ScriptContext[41];

	void* Hook = placeHook(ASLR(0x401018), INT3BreakpointHook);

	if (Hook == NULL) {
		do {
			Sleep(1);
		} while (Hook == NULL);
	}
	do {
		ExecuteScript(r_LS, GetInput().c_str());
	} while (true);
}

BOOL WINAPI DllMain(HMODULE Module, DWORD Reason, LPVOID lpReserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
	}
	return TRUE;
}