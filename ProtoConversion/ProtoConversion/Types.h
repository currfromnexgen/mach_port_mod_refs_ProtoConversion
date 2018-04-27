#pragma once
#include <Windows.h>
#include "Lua\lua.hpp"

union r_Value {
	PVOID gc;
	PVOID p;
	double n;
	int b;
};

typedef struct r_TString {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	unsigned int len;			// +08	08
	unsigned int hash;			// +12  0C
};

struct r_TValue {
	int value_0;
	int value_1;
	r_Value value;
	int tt;
	int unk;
};

struct r_Proto {
	CommonHeader;    // +00
	unsigned char Unk0;   // +07
	r_TValue* k;     // +08
	int* source;    // +12
	int sizek;     // +16
	int* lineinfo;    // +20
	Instruction* code;   // +24
	int* locvars;    // +28
	r_Proto* p;     // +32
	int sizeupvalues;   // +36
	int* upvalues;    // +40
	int linedefined;   // +44
	int sizelocvars;   // +48
	unsigned char Unk1[4];  // +52
	int sizep;     // +56
	int sizelineinfo;   // +60
	int sizecode;    // +64
	unsigned char Unk2[4];  // +68
	int lastlinedefined;  // +72
	byte numparams;    // +76
	byte is_vararg;    // +77
	byte nups;     // +78
	byte maxstacksize;   // +79
};

struct r_LClosure {
	unsigned char Unk[15];		// +00
	r_Proto* p;					// +16
	int upvals[1];				// +20
};