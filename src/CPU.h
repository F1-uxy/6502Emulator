#ifndef CPUH
#define CPUH

#include "6502main.h"
#include "Memory.h"
#include "types.h"

class Memory;

class CPU
{
public:
    using instruction_func = void (CPU::*)();

    explicit CPU(Memory& mem);
    
    Word PC; //Program Counter
    Byte SP; //Stack Pointer
    Byte StackPage;

    Byte Acc; //Accumulator
    Byte X, Y;//Registers

    //Flags: Carry(C), Negative(N), Overflow(V), Zero(Z), Decimal(D), IRQB_Disable(I)
    // N V 1 B D I Z C

    Byte N : 1;
    Byte V : 1;
    Byte B : 1;
    Byte D : 1;
    Byte I : 1;
    Byte Z : 1;
    Byte C : 1;

    Word RESET_VECTOR = 0xFFFC;

    s32 Cycles;

    int Execute(s32 Cycles, Memory& memory);
    int Execute(s32 Cycles, Memory& memory, Byte settings);

private:

    Memory& memory;

    instruction_func opcode_lut[256] = {
        /*      0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F*/
        /*1*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*2*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*3*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*4*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*5*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*6*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*7*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*8*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*9*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*A*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*B*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*C*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*D*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*E*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN,
        /*F*/ &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN, &CPU::NAN
    };

    void initialiseOpcodeLUT();
    void Reset(Memory& memory);

    void NAN();

    Byte Fetch(s32& Cycles, Memory& memory);
    Word FetchWord(s32& Cycles, Memory& memory);
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
    void AddrIndirectY(Byte& reg, s32& Cycles, Memory& memory);
    inline void LoadImmediate(Byte& reg, s32& Cycles, Memory& memory);
    void LoadZP(Byte& reg, s32& Cycles, Memory& memory);
    void LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);
    void LoadABS(Byte& reg, s32& Cycles, Memory& memory);
    void LoadABSOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);
    void printDebug();
    void printFlags();
    void SetZeroFlag(Byte reg);
    void SetNegativeFlag(Byte reg);
    void SetOverflowFlag(Byte reg);
    void LDASetStatus();
    void LDXSetStatus();
    void TAXSetStatus();
    void TAYSetStatus();
    void ANDSetStatus();
    void XORSetStatus();
    void IORSetStatus();
    void BITSetStatus(Byte result);
    

    // Operation Functions
    void LDA_IM();
    void LDA_ZP();
    void LDA_ZP_X();
    void LDA_ABS();
    void LDA_ABS_X();
    void LDA_ABS_Y();
    void LDA_IND_X();
    void LDA_IND_Y();
    void JMP_ABS();
    void JSR();
    void JMP_IND();
    void LDX_IM();
    void LDX_ZP();
    void LDX_ZP_Y();
    void LDX_ABS();
    void LDX_ABS_Y();
    void TAX();
    void TAY();
    void TSX();
    void TXA();
    void TXS();
    void TYA();
    void AND_IM();
    void AND_ZP();
    void AND_ZP_X();
    void AND_ABS();
    void AND_ABS_X();
    void AND_ABS_Y();
    void AND_IND_X();
    void AND_IND_Y();
    void XOR_IM();
    void XOR_ZP();
    void XOR_ZP_X();
    void XOR_ABS();
    void XOR_ABS_X();
    void XOR_ABS_Y();
    void XOR_IND_X();
    void XOR_IND_Y();
    void IOR_IM();
    void IOR_ZP();
    void IOR_ZP_X();
    void IOR_ABS();
    void IOR_ABS_X();
    void IOR_ABS_Y();
    void IOR_IND_X();
    void IOR_IND_Y();
    void BIT_ZP();
    void BIT_ABS();

    /*
    void CPU::LDA_IM()
{

}

void CPU::LDA_ZP()
{

}

void CPU::LDA_ZP_X()
{

}

void CPU::LDA_ABS()
{

}

void CPU::LDA_ABS_X()
{

}

void CPU::LDA_ABS_Y()
{

}

void CPU::LDA_IND_X()
{

}

void CPU::LDA_IND_Y()
{

}

void CPU::JMP_ABS()
{

}

void CPU::JSR()
{

}

void CPU::JMP_IND()
{

}

void CPU::LDX_IM()
{

}

void CPU::LDX_ZP()
{

}

void CPU::LDX_ZP_Y()
{

}

void CPU::LDX_ABS()
{

}

void CPU::LDX_ABS_Y()
{

}

void CPU::TAX()
{

}

void CPU::TAY()
{

}

void CPU::TSX()
{

}

void CPU::TXA()
{

}

void CPU::TXS()
{

}

void CPU::TYA()
{

}

void CPU::AND_IM()
{

}

void CPU::AND_ZP()
{

}

void CPU::AND_ZP_X()
{

}

void CPU::AND_ABS()
{

}

void CPU::AND_ABS_X()
{

}

void CPU::AND_ABS_Y()
{

}

void CPU::AND_IND_X()
{

}

void CPU::AND_IND_Y()
{

}

void CPU::XOR_IM()
{

}

void CPU::XOR_ZP()
{

}

void CPU::XOR_ZP_X()
{

}

void CPU::XOR_ABS()
{

}

void CPU::XOR_ABS_X()
{

}

void CPU::XOR_ABS_Y()
{

}

void CPU::XOR_IND_X()
{

}

void CPU::XOR_IND_Y()
{

}

void CPU::IOR_IM()
{

}

void CPU::IOR_ZP()
{

}

void CPU::IOR_ZP_X()
{

}

void CPU::IOR_ABS()
{

}

void CPU::IOR_ABS_X()
{

}

void CPU::IOR_ABS_Y()
{

}

void CPU::IOR_IND_X()
{

}

void CPU::IOR_IND_Y()
{

}

void CPU::BIT_ZP()
{

}

void CPU::BIT_ABS()
{

}

    */
};

#endif