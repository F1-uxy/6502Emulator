#ifndef CPUEMULATOR_6502MAIN_H
#define CPUEMULATOR_6502MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>



using Byte = unsigned char;
using Word = unsigned short;

using uint = unsigned int;



struct Memory
{
    static constexpr uint MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise()
    {
        std::fill(Data, Data + MAX_MEM, 0);
    }

    // Read 1 Byte
    Byte operator[](uint Address) const
    {
        return Data[Address];
    }

    // Write 1 Byte
    Byte& operator[](uint Address)
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
    Byte X, Y;  //Registers

    Byte N : 1;
    Byte V : 1;
    Byte B : 1;
    Byte D : 1;
    Byte I : 1;
    Byte Z : 1;
    Byte C : 1;

    // Opcodes
    static constexpr Byte
            INS_LDA_IM = 0xA9,
            INS_LDA_ZP = 0xA5,
            INS_LDA_ZP_X = 0xB5,
            INS_LDA_ABS = 0xAD,
            INS_LDA_ABS_X = 0xBD,
            INS_JMP_ABS = 0x4C,
            INS_JSR = 0x20,
            INS_JMP_IND = 0x6C;


    //Flags: Carry(C), Negative(N), Overflow(V), Zero(Z), Decimal(D), IRQB_Disable(I)
    // N V 1 B D I Z C


    void Reset(Memory& memory)
    {
        PC = 0xFFFC;
        SP = 0xFF;
        StackPage = 0x01;
        Acc, X, Y = 0;
        N, V, B, D, I, Z, C = 0;
        memory.Initialise();
    }

    Byte Fetch(uint& Cycles, Memory& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;

    }

    Word FetchWord(uint& Cycles, Memory& memory)
    {
        // Little Endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8); // Little Endian Shift
        PC++;

        Cycles -= 2;

        return Data;
    }

    Word FetchStackAddress()
    {
        return 0x0100 | SP;
    }

    Byte Read(uint& Cycles, Memory& memory, Byte Addr)
    {
        Byte Data = memory[Addr];
        Cycles--;
        return Data;
    }

    Byte ReadWord(uint& Cycles, Memory memory, Byte Addr)
    {
        // Little Endian
        Word Address = memory[Addr];
        PC++;

        Address | (memory[PC] << 8);
        PC++;

        Cycles -= 2;

        return Address;
    }


    void LDASetStatus()
    {
        Z = (Acc == 0);
        N = (Acc & 0b10000000) > 0;
    }

    void Execute(uint Cycles, Memory& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = Fetch(Cycles, memory);

            switch (Ins)
            {
                case INS_LDA_IM:
                {
                    Acc = Fetch(Cycles, memory);
                    LDASetStatus();
                } break;
                    // Zero Page Access Mode
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddr = Fetch(Cycles, memory);
                    Acc = Read(Cycles, memory, ZeroPageAddr);
                    LDASetStatus();
                } break;
                case INS_LDA_ZP_X:
                {
                    Byte ZeroPageAddr = Fetch(Cycles, memory);
                    ZeroPageAddr += X;
                    Cycles--;
                    Acc = Read(Cycles, memory, ZeroPageAddr);
                    LDASetStatus();
                } break;
                case INS_LDA_ABS:
                {
                    Word AbsAddress = FetchWord(Cycles, memory);
                    Acc = Read(Cycles, memory, AbsAddress);
                    LDASetStatus();

                } break;
                case INS_LDA_ABS_X:
                {
                    Word AbsAddress = FetchWord(Cycles, memory);
                    AbsAddress += X;
                    Cycles--;
                    Acc = Read(Cycles, memory, AbsAddress);
                    LDASetStatus();
                } break;
                    //  6502 Cannot obtain address if indirect vector falls on a page boundary
                case INS_JMP_ABS:
                {
                    Word Address = FetchWord(Cycles, memory);
                    PC = Address;
                    Cycles--;
                } break;
                case INS_JMP_IND:
                {
                    Word Address = FetchWord(Cycles, memory);
                    PC = ReadWord(Cycles, memory, Address);

                }
                case INS_JSR:
                {
                    Word SubAddress = FetchWord(Cycles, memory);
                    memory[SP] = PC - 1;
                    Cycles--;
                    PC = SubAddress;
                    Cycles--;
                } break;
                default:
                {
                    printf("Instruction not found: %d", Ins);
                } break;
            }

        }
    }


};

#endif CPUEMULATOR_6502MAIN_H
