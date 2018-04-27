#pragma once
#include <iostream>
#include <cstdint>

/*

* @ Credits go to Eternal and Austin for their Encryptions.

*/

uint32_t DaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd);
int call_enc(int a1, int a2);
int ClosureEnc(int a1, int a2);

unsigned int EncryptFunction(unsigned int instr, unsigned int pc, unsigned int init, unsigned int constant, unsigned int mask);
unsigned int jumpDecryption(unsigned int instr, unsigned int pc);
unsigned int setupval_encryption(unsigned int instr, unsigned int pc);
unsigned int JumpEncryption(unsigned int instr, unsigned int pc);