#ifndef CPUEMULATOR_6502MAIN_H
#define CPUEMULATOR_6502MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;

static constexpr Byte FLAG_DEBUG    = 0x01;
static constexpr Byte FLAG_INFINITE = 0x02;
static constexpr Byte FLAG_IGNORE   = 0x04;
static constexpr Byte FLAG_STEP     = 0x08;
static constexpr Byte FLAG_FLAGS    = 0x10;

struct Settings {
    Byte flags = 0;

    void setFlag(Byte flag)
    {
        flags |= flag;
    }

    void clearFlag(Byte flag)
    {
        flag &= ~flag;
    }

    bool hasFlag(Byte flag)
    {
        return (flag & flags) != 0;
    }

    bool operator|(const Byte other) const {
        return this->flags | other;
    }

    Settings& operator|=(const Byte other) {
        this->flags |= other;
        return *this;
    }
};

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
            INS_AND_IND_Y   = 0x31,
            INS_XOR_IM      = 0x49,
            INS_XOR_ZP      = 0x45,
            INS_XOR_ZP_X    = 0x55,
            INS_XOR_ABS     = 0x4D,
            INS_XOR_ABS_X   = 0x5D,
            INS_XOR_ABS_Y   = 0x59,
            INS_XOR_IND_X   = 0x41,
            INS_XOR_IND_Y   = 0x51,
            INS_IOR_IM      = 0x09,
            INS_IOR_ZP      = 0x05,
            INS_IOR_ZP_X    = 0x15,
            INS_IOR_ABS     = 0x0D,
            INS_IOR_ABS_X   = 0x1D,
            INS_IOR_ABS_Y   = 0x19,
            INS_IOR_IND_X   = 0x01,
            INS_IOR_IND_Y   = 0x11,
            INS_BIT_ZP      = 0x24,
            INS_BIT_ABS     = 0x2C;
  


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

    void printDebug()
    {
        printf("PC: %#04x SP: %#04x Acc: %#04x\n", PC, SP, Acc);
    }

    void printFlags()
    {
        printf("X Y N V B D I Z C\n");
        printf("%d %d %d %d %d %d %d %d %d\n", X, Y, N, V, B, D, I, Z, C);
    }

    void SetZeroFlag(Byte reg)
    {
        Z = (reg == 0);
    }

    void SetNegativeFlag(Byte reg)
    {
        N = (reg & 0b10000000) > 0;
    }

    void SetOverflowFlag(Byte reg)
    {
        V = (reg & 0b01000000) > 0;
    }

    void LDASetStatus()
    {
        SetZeroFlag(Acc);
        SetNegativeFlag(Acc);
    }

    void LDXSetStatus()
    {
        SetZeroFlag(X);
        SetNegativeFlag(X);
    }

    void TAXSetStatus()
    {
        SetZeroFlag(X);
        SetNegativeFlag(X);
    }

    void TAYSetStatus()
    {
        SetZeroFlag(Y);
        SetNegativeFlag(Y);
    }

    void ANDSetStatus()
    {
        SetZeroFlag(Acc);
        SetNegativeFlag(Acc);
    }

    void XORSetStatus()
    {
        SetZeroFlag(Acc);
        SetNegativeFlag(Acc);
    }

    void IORSetStatus()
    {
        SetZeroFlag(Acc);
        SetNegativeFlag(Acc);
    }

    void BITSetStatus(Byte result)
    {
        SetZeroFlag(result);
        SetOverflowFlag(result);
        SetNegativeFlag(result);
    }

    int Execute(s32 Cycles, Memory& memory)
    {
        Settings defaultSettings;
        return Execute(Cycles, memory, 0);
    }

    int Execute(s32 Cycles, Memory& memory, Byte settings)
    {
        while (Cycles > 0 || settings | FLAG_INFINITE)
        {
            Byte Ins = Fetch(Cycles, memory);
            if(settings & FLAG_STEP){
                //while(!std::cin.get());
                std::cin.get();
            }
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
                case INS_XOR_IM:
                {
                    Byte inputByte;
                    LoadABS(inputByte, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_ZP:
                {
                    Byte inputByte;
                    LoadImmediate(inputByte, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_ZP_X:
                {
                    Byte inputByte;
                    LoadZPOffset(inputByte, X, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_ABS:
                {
                    Byte inputByte;
                    LoadABS(inputByte, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_ABS_X:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, X, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_ABS_Y:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, Y, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_IND_X:
                {
                    Word indirectAddress = AddrIndirectX(Cycles, memory);
                    Byte inputByte = ReadByte(Cycles, memory, indirectAddress);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_XOR_IND_Y:
                {
                    Byte inputByte;
                    AddrIndirectY(inputByte, Cycles, memory);
                    Acc = Acc ^ inputByte;
                    XORSetStatus();
                }break;
                case INS_IOR_IM:
                {
                    Byte inputByte;
                    LoadImmediate(inputByte, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_ZP:
                {
                    Byte inputByte;
                    LoadZP(inputByte, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_ZP_X:
                {
                    Byte inputByte;
                    LoadZPOffset(inputByte, X, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_ABS:
                {
                    Byte inputByte;
                    LoadABS(inputByte, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_ABS_X:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, X, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_ABS_Y:
                {
                    Byte inputByte;
                    LoadABSOffset(inputByte, Y, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_IND_X:
                {
                    Word indirectAddress = AddrIndirectX(Cycles, memory);
                    Byte inputByte = ReadByte(Cycles, memory, indirectAddress);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_IOR_IND_Y:
                {
                    Byte inputByte;
                    AddrIndirectY(inputByte, Cycles, memory);
                    Acc = Acc | inputByte;
                    IORSetStatus();
                }break;
                case INS_BIT_ZP:
                {
                    Byte inputByte;
                    Byte result;
                    LoadZP(inputByte, Cycles, memory);
                    result = inputByte & Acc;
                    BITSetStatus(result);
                }break;
                case INS_BIT_ABS:
                {
                    Word inputAddr;
                    Byte inputByte;
                    inputByte = ReadByte(Cycles, memory, inputAddr);
                    Byte result = inputByte & Acc;
                    BITSetStatus(result);
                }break;
                default:
                {
                    printf("Instruction not found: %d \n", Ins);
                    printf("PC value: %d \n", PC);

                    if(!(settings & FLAG_IGNORE)) return -1;
                } break;
            }
            printf ("\033\143");
            if(settings & FLAG_DEBUG) printDebug();
            if(settings & FLAG_FLAGS) printFlags();

        }

        return Cycles;

    }


};

void loadProgram(const std::string&, Memory&, Word);

#endif CPUEMULATOR_6502MAIN_H
