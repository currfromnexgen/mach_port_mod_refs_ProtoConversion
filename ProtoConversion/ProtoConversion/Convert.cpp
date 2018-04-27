#include <Windows.h>
#include "Types.h"
#include "Encryptions.h"
#include "RLua.h"

/*

* @ Author: mach_port_mod_refs#9156
* @ File: Convert.cpp

*/

r_Proto* ConvertProto(int r_LS, Proto* Proto, r_Proto* rProto, const char* Script) {
	r_Proto* _rProto = rluaF_newproto(r_LS);

	_rProto->sizecode = Proto->sizecode;
	auto pCode = (int*)rluaM_malloc(r_LS, _rProto->sizecode * sizeof(int));

	for (int Index = 0; Index < Proto->sizep; Index++) {
		pCode[Index] = rluaF_newproto(r_LS);
		ConvertProto(r_LS, Proto->p[Index], pCode[Index], 0);
	}

	_rProto->sizek = Proto->sizek;
	r_TValue* rTValue = (r_TValue*)rluaM_malloc(r_LS, _rProto->sizek * sizeof(r_TValue));
	for (int Index = 0; Index < _rProto->sizek; Index++) {
		r_TValue* rT = &rTValue[Index];
		TValue* T = &Proto->k[Index];

		switch (T->tt) {
		case LUA_TNIL: {
			rT->tt = LUA_TNIL;
			rT->value.n = NULL;
			break;
		}
		case LUA_TBOOLEAN: {
			rT->tt = LUA_TBOOLEAN;
			rT->value.b = T->value.b;
			break;
		}
		case LUA_TNUMBER: {
			rT->tt = LUA_TNUMBER;
			rT->value.n = T->value.n;
			break;
		}
		case LUA_TSTRING: {
			rT->tt = LUA_TSTRING;
			rT->value.gc = rluaS_new(r_LS, getstr((TString*)T->value.gc));
			break;
		}
		default: break;
		}

		rProto->sizecode = Proto->sizecode;
		auto pCode = (int*)rluaM_malloc(r_LS, _rProto->sizecode * sizeof(int));
		*(int*)(int)(&rProto) = (int)(pCode)+(int)(&rProto);

		for (int Key = 0; Key < Proto->sizecode; Key++) {
			Instruction Instruction = Proto->code[Key];
			int rInstruction = NULL;

			switch (Instruction) {
			case OP_JMP: {
				rInstruction = rInstruction >> 26 << 26 | JumpEncryption(rInstruction, Key);
				break;
			}
			case OP_CALL: {
				rInstruction = rInstruction >> 26 << 26 | call_enc(rInstruction, Key);
				break;
			}
			case OP_RETURN: {
				rInstruction = rInstruction >> 26 << 26 | DaxEncodeOp(rInstruction, Key, 1470882913, Key - 1577854801, 641680189) & 0x3FFFFFF;
				break;
			}
			case OP_CLOSURE: {
				rInstruction = rInstruction >> 26 << 26 | ClosureEnc(rInstruction, Key);
				break;
			}
			case OP_SETUPVAL: {
				rInstruction = rInstruction >> 26 << 26 | setupval_encryption(rInstruction, Key);
				break;
			}
			case OP_MOVE: {
				rInstruction = rInstruction & 0xFFFC21FF | 0x2000;
				break;
			}
			}
			pCode[Key] = rInstruction * Encode_Key;
			rProto->sizelineinfo = Proto->sizelineinfo;
			rProto->numparams = Proto->numparams;
			rProto->nups = Proto->nups;
			rProto->is_vararg = Proto->is_vararg;

			r_TString* Source = rluaS_new(r_LS, Script ? Script : "@zVA");

			for (int Key = 0; Key < Proto->sizelineinfo; Key++) {
				pCode[Key] = Proto->lineinfo[Key];
			}

			rProto->maxstacksize = Proto->maxstacksize;
			rProto->is_vararg = Proto->is_vararg;
			rProto->numparams = Proto->numparams;
		}
	}
}

void setLClosure(int r_LS, LClosure* Closure) {
	Proto* Proto = Closure->p;
	r_Proto* rProto = rluaF_newproto(r_LS);
	ConvertProto(r_LS, Proto, rProto, (const char*)lua_upvalueindex(1));
	r_LClosure* rLClosure = rluaF_newLClosure(r_LS, lua_upvalueindex(1), *(int*)(r_LS + 64));

	for (int Key = 0; Key < lua_upvalueindex(1); Key++) {
		*(int*)((int)&rLClosure->upvals[Key]) = rluaF_newupval(rL);
	}
	rlua_pushlclosure(r_LS, rLClosure);
}

void ExecuteScript(int r_LS, const char* Source) {
	lua_State* L = lua_open();
	if (!luaL_loadstring(L, Source)) {
		TValue* Value = (L->top) - 1;
		LClosure* Closure = &clvalue(Value)->l;
		setLClosure(r_LS, Closure);
		lua_pop(L, 1);
	}
}