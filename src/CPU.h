#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Memory.h"

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;

struct CPU
{

    Word PC; //Program Counter
    Byte SP; //Stack Pointer
    Byte StackPage;

    Byte Acc; //Accumulator
    Byte X, Y;//Registers

    Byte N : 1;
    Byte V : 1;
    Byte B : 1;
    Byte D : 1;
    Byte I : 1;
    Byte Z : 1;
    Byte C : 1;

    Word RESET_VECTOR = 0xFFFC;

    // Opcodes
    static constexpr Byte
            INS_LDA_IM      = 0xA9,
            INS_LDA_ZP      = 0xA5,
            INS_LDA_ZP_X    = 0xB5,
            INS_LDA_ABS     = 0xAD,
            INS_LDA_ABS_X   = 0xBD,
            INS_LDA_ABS_Y   = 0xB9,
            INS_LDA_IND_X   = 0xA1,
            INS_LDA_IND_Y   = 0XB1,
            INS_JMP_ABS     = 0x4C,
            INS_JSR         = 0x20,
            INS_JMP_IND     = 0x6C,
            INS_LDX_IM      = 0xA2,
            INS_LDX_ZP      = 0xA6,
            INS_LDX_ZP_Y    = 0xB6,
            INS_LDX_ABS     = 0xAE,
            INS_LDX_ABS_Y   = 0xBE,
            INS_TAX         = 0xAA,
            INS_TAY         = 0xA8,
            INS_TSX         = 0xBA,
            INS_TXA         = 0x8A,
            INS_TXS         = 0x9A,
            INS_TYA         = 0x98;


    //Flags: Carry(C), Negative(N), Overflow(V), Zero(Z), Decimal(D), IRQB_Disable(I)
    // N V 1 B D I Z C


    void Reset(Memory& memory);

    Byte Fetch(s32& Cycles, Memory& memory);

    Word FetchWord(s32& Cycles, Memory& memory);
    // Return StackPointer as full 16 bit 0x01__ Address (First Page)
    Word FetchStackAddress();

    Byte ReadByte(s32& Cycles, Memory& memory, Word Addr);

    Word ReadWord(s32& Cycles, Memory& memory, Word Addr);

    void WriteByte(s32& Cycles, Memory& memory, Word Addr, Byte Value);

    void WriteWord(s32& Cycles, Memory& memory, Word Addr, Word Value);

    void PushWordToStack(s32& Cycles, Memory& memory, Word Addr);

    void PushPCMinusOneToStack(s32& Cycles, Memory& memory);

    void PushPCPlussOneToStack(s32& Cycles, Memory& memory);

    void PushPCToStack(s32& Cycles, Memory& memory);

    Word AddrIndirectX(s32& Cycles, Memory& memory);

    inline void LoadImmediate(Byte& reg, s32& Cycles, Memory& memory);

    void LoadZP(Byte& reg, s32& Cycles, Memory& memory);

    void LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);

    void LoadABS(Byte& reg, s32& Cycles, Memory& memory);

    void LoadABSOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);

    void LDASetStatus();

    void LDXSetStatus();

    void TAXSetStatus();

    void TAYSetStatus();

    int Execute(s32 Cycles, Memory& memory);


};