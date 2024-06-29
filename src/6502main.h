#ifndef CPUEMULATOR_6502MAIN_H
#define CPUEMULATOR_6502MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>



using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;



struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise()
    {
        std::fill(Data, Data + MAX_MEM, 0);
    }

    // Read 1 Byte
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }

    // Write 1 Byte
    Byte& operator[](u32 Address)
    {
        return Data[Address];
    }

};

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
            INS_TYA         = 0x98,
            INS_AND_IM      = 0x29,
            INS_AND_ZP      = 0x25,
            INS_AND_ZP_X    = 0x35,
            INS_AND_ABS     = 0x2D,
            INS_AND_ABS_X   = 0x3D,
            INS_AND_ABS_Y   = 0x39,
            INS_AND_IND_X   = 0x21,
            INS_AND_IND_Y   = 0x31;
  


    //Flags: Carry(C), Negative(N), Overflow(V), Zero(Z), Decimal(D), IRQB_Disable(I)
    // N V 1 B D I Z C


    void Reset(Memory& memory)
    {
        PC = RESET_VECTOR;
        SP = 0xFF;
        StackPage = 0x01;
        Acc = 0;
        X = 0;
        Y = 0;
        N = 0;
        V = 0;
        B = 0;
        D = 0;
        I = 0;
        Z = 0;
        C = 0;
        memory.Initialise();
    }

    Byte Fetch(s32& Cycles, Memory& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;

    }

    Word FetchWord(s32& Cycles, Memory& memory)
    {
        // Little Endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8); // Little Endian Shift
        PC++;

        Cycles -= 2;
        return Data;
    }
    // Return StackPointer as full 16 bit 0x01__ Address (First Page)
    Word FetchStackAddress()
    {
        return 0x0100 | SP;
    }

    Byte ReadByte(s32& Cycles, Memory& memory, Word Addr)
    {
        Byte Data = memory[Addr];
        Cycles--;
        return Data;
    }

    Word ReadWord(s32& Cycles, Memory& memory, Word Addr)
    {
        // Little Endian
        Word Address = memory[Addr];
        PC++;

        Word ReturnAddress = Address | (memory[Addr + 1] << 8);
        PC++;

        Cycles -= 2;

        return ReturnAddress;
    }

    void WriteByte(s32& Cycles, Memory& memory, Word Addr, Byte Value)
    {
        memory[Addr] = Value;
        Cycles--;
    }

    void WriteWord(s32& Cycles, Memory& memory, Word Addr, Word Value)
    {
        memory[Addr] = Value & 0xFF;
        memory[Addr + 1] = (Value >> 8) & 0xFF;
        Cycles -= 2;
    }

    void PushWordToStack(s32& Cycles, Memory& memory, Word Addr)
    {
        Byte LowAddrByte = Addr & 0xFF;
        Byte HighAddrByte = (Addr >> 8) & 0xFF;
        WriteByte(Cycles, memory, FetchStackAddress(), HighAddrByte);
        SP--;
        WriteByte(Cycles, memory, FetchStackAddress(), LowAddrByte);
        SP--;
    }

    void PushPCMinusOneToStack(s32& Cycles, Memory& memory)
    {
        PushWordToStack(Cycles, memory, PC-1);
    }

    void PushPCPlussOneToStack(s32& Cycles, Memory& memory)
    {
        PushWordToStack(Cycles, memory, PC+1);
    }

    void PushPCToStack(s32& Cycles, Memory& memory)
    {
        PushWordToStack(Cycles, memory, PC);
    }

    Word AddrIndirectX(s32& Cycles, Memory& memory)
    {
        Byte ZPAddress = Fetch(Cycles, memory);
        ZPAddress = (ZPAddress + X) & 0xFF;
        Cycles --;
        Word ReturnAddress = ReadWord(Cycles, memory, ZPAddress);
        return ReturnAddress;
    }

    // Load Word address value stored in address given by ZPPointer value offset by Y into given register
    void AddrIndirectY(Byte& reg, s32& Cycles, Memory& memory)
    {
        Word ZPPointer = Fetch(Cycles, memory);
        Word baseAddress = ReadWord(Cycles, memory, ZPPointer);
        Word effectiveAddress = baseAddress + Y;
        
        if((baseAddress & 0xFF00) != (effectiveAddress & 0xFF00))
        {
            Cycles--;
        }
        reg =  ReadByte(Cycles, memory, effectiveAddress);
    }

    // Load Immediate value into given register
    inline void LoadImmediate(Byte& reg, s32& Cycles, Memory& memory)
    {
        reg = Fetch(Cycles, memory);
    }

    // Load Byte address from Zero Page 0x00__ into given register
    void LoadZP(Byte& reg, s32& Cycles, Memory& memory)
    {
        Byte ZeroPageAddr = Fetch(Cycles, memory);
        reg = ReadByte(Cycles, memory, ZeroPageAddr);
    }

    // Load Byte address from Zero Page 0x00__ with Byte offset into given register
    void LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory)
    {
        Byte ZeroPageAddr = Fetch(Cycles, memory);
        ZeroPageAddr += offset;
        Cycles--;
        reg = ReadByte(Cycles, memory, ZeroPageAddr);

    }

    // Load Word address from next address in memory to given register
    void LoadABS(Byte& reg, s32& Cycles, Memory& memory)
    {
        Word AbsAddress = FetchWord(Cycles, memory);
        reg = ReadByte(Cycles, memory, AbsAddress);
    }

    // Load Word address from next address with Byte offset to given register
    void LoadABSOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory)
    {
        Word AbsAddress = FetchWord(Cycles, memory);
        Word FinalAddress = AbsAddress + offset;

        if((AbsAddress & 0xFF00) != (FinalAddress & 0xFF00))
        {
            Cycles--;
        }

        reg = ReadByte(Cycles, memory, FinalAddress);

    } 

    void LDASetStatus()
    {
        Z = (Acc == 0);
        N = (Acc & 0b10000000) > 0;
    }

    void LDXSetStatus()
    {
        Z = (X == 0);
        N = (X & 0b10000000) > 0;        
    }

    void TAXSetStatus()
    {
        Z = (X == 0);
        N = (X & 0b10000000) > 0;  
    }

    void TAYSetStatus()
    {
        Z = (Y == 0);
        N = (Y & 0b10000000) > 0;  
    }

    void ANDSetStatus()
    {
        Z = (Acc == 0);
        N = (Acc & 0b10000000) > 0;
    }

    int Execute(s32 Cycles, Memory& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = Fetch(Cycles, memory);

            switch (Ins)
            {
                case INS_LDA_IM:
                {
                    LoadImmediate(Acc, Cycles, memory);
                    LDASetStatus();
                } break;
                    // Zero Page Access Mode
                case INS_LDA_ZP:
                {
                    LoadZP(Acc, Cycles, memory);
                    LDASetStatus();
                } break;
                case INS_LDA_ZP_X:
                {
                    LoadZPOffset(Acc, X, Cycles, memory);
                    LDASetStatus();
                } break;
                case INS_LDA_ABS:
                {
                    LoadABS(Acc, Cycles, memory);
                    LDASetStatus();
                } break;
                case INS_LDA_ABS_X:
                {
                    LoadABSOffset(Acc, X, Cycles, memory);
                    LDASetStatus();
                }break;
                case INS_LDA_ABS_Y:
                {

                    LoadABSOffset(Acc, Y, Cycles, memory);
                    LDASetStatus();
                }break;
                    //  6502 Cannot obtain address if indirect vector falls on a page boundary
                case INS_LDA_IND_X:
                {
                    Word Address = AddrIndirectX(Cycles, memory);
                    Acc = ReadByte(Cycles, memory, Address);
                    LDASetStatus();
                }break;
                case INS_LDX_IM:
                {
                    LoadImmediate(X, Cycles, memory);
                    LDXSetStatus();
                }break;
                case INS_LDX_ZP:
                {
                    LoadZP(X, Cycles, memory);
                    LDXSetStatus();
                }break;
                case INS_LDX_ZP_Y:
                {
                    LoadZPOffset(X, Y, Cycles, memory);
                    LDXSetStatus();
                }break;
                case INS_LDX_ABS:
                {
                    LoadABS(X, Cycles, memory);
                    LDXSetStatus();
                }break;
                case INS_LDX_ABS_Y:
                {
                    LoadABSOffset(X, Y, Cycles, memory);
                    LDXSetStatus();
                }break;
                case INS_JMP_ABS:
                {
                    Word Address = FetchWord(Cycles, memory);
                    PC = Address;
                } break;
                case INS_JMP_IND:
                {
                    Word Address = FetchWord(Cycles, memory);
                    PC = ReadWord(Cycles, memory, Address);
                }break;
                case INS_JSR:
                {
                    Word SubAddress = FetchWord(Cycles, memory);

                    PushPCMinusOneToStack(Cycles, memory);

                    PC = SubAddress;
                    Cycles--;
                } break;
                case INS_TAX:
                {
                    X = Acc;
                    Cycles -= 2;
                    TAXSetStatus();
                } break;
                case INS_TAY:
                {
                    Y = Acc;
                    Cycles -= 2;
                    TAYSetStatus();
                } break;
                case INS_TSX:
                {
                    X = SP;
                    Cycles -= 2;
                    TAXSetStatus();
                } break;
                case INS_TXA:
                {
                    Acc = X;
                    Cycles -= 2;
                    LDASetStatus();
                } break;
                case INS_TXS:
                {
                    SP = X;
                    Cycles -= 2;
                } break;
                case INS_TYA:
                {
                    Acc = Y;
                    Cycles -= 2;
                    LDASetStatus();
                } break;
                case INS_AND_IM:
                {
                    Byte inputByte; 
                    LoadImmediate(inputByte, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_ZP:
                {
                    Byte inputByte;
                    LoadZP(inputByte, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_ZP_X:
                {
                    Byte inputByte;
                    LoadZPOffset(inputByte, X, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_ABS:
                {
                    Byte inputByte;
                    LoadABS(inputByte, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_ABS_X:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, X, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_ABS_Y:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, Y, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                case INS_AND_IND_X:
                {
                    Word indirectAddress = AddrIndirectX(Cycles, memory);
                    Acc = Acc & ReadByte(Cycles, memory, indirectAddress);
                    ANDSetStatus();
                }break;
                case INS_AND_IND_Y:
                {
                    Byte inputByte;
                    AddrIndirectY(inputByte, Cycles, memory);
                    Acc = Acc & inputByte;
                    ANDSetStatus();
                }break;
                default:
                {
                    printf("Instruction not found: %d \n", Ins);
                    printf("PC value: %d \n", PC);
                    return -1;
                } break;
            }

        }

        return Cycles;

    }


};

#endif CPUEMULATOR_6502MAIN_H
