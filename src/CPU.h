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

    //Flags: Carry(C), Negative(N), Unused (1), Overflow(V), Zero(Z), Decimal(D), IRQB_Disable(I)
    // N V 1 B D I Z C

    union ProcessorStatus
    {
        struct {
            unsigned N : 1; // Negative
            unsigned V : 1; // Overflow
            unsigned U : 1; // Unused
            unsigned B : 1; // Break
            unsigned D : 1; // Decimal
            unsigned I : 1; // Interrupt Disable
            unsigned Z : 1; // Zero
            unsigned C : 1; // Carry
        } flags;
        Byte status = 0;
    };
    
    

    Word RESET_VECTOR = 0xFFFC;

    s32 Cycles;

    int Execute(s32 Cycles, Memory& memory);
    int Execute(s32 Cycles, Memory& memory, Byte settings);

private:

    Memory& memory;

    instruction_func opcode_lut[256] = {
        /*          0          1          2                 3          4          5                 6               7              8          9          A              B          C          D                E          F*/
        /*0*/ &CPU::NAN, &CPU::IOR_IND_X, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::IOR_ZP,   &CPU::NAN,      &CPU::NAN, &CPU::PHP, &CPU::IOR_IM,    &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::IOR_ABS,   &CPU::NAN,       &CPU::NAN,
        /*1*/ &CPU::NAN, &CPU::IOR_IND_Y, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::IOR_ZP_X, &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::IOR_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::IOR_ABS_X, &CPU::NAN,       &CPU::NAN,
        /*2*/ &CPU::JSR, &CPU::AND_IND_X, &CPU::NAN,    &CPU::NAN, &CPU::BIT_ZP, &CPU::AND_ZP,   &CPU::NAN,      &CPU::NAN, &CPU::PLP, &CPU::AND_IM,    &CPU::NAN, &CPU::NAN, &CPU::BIT_ABS, &CPU::AND_ABS,   &CPU::NAN,       &CPU::NAN,
        /*3*/ &CPU::NAN, &CPU::AND_IND_Y, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::AND_ZP_X, &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::AND_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::AND_ABS_X, &CPU::NAN,       &CPU::NAN,
        /*4*/ &CPU::NAN, &CPU::XOR_IND_X, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::XOR_ZP,   &CPU::NAN,      &CPU::NAN, &CPU::PHA, &CPU::XOR_IM,    &CPU::NAN, &CPU::NAN, &CPU::JMP_ABS, &CPU::XOR_ABS,   &CPU::NAN,       &CPU::NAN,
        /*5*/ &CPU::NAN, &CPU::XOR_IND_Y, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::XOR_ZP_X, &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::XOR_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::XOR_ABS_X, &CPU::NAN,       &CPU::NAN,
        /*6*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::PLA, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::JMP_IND, &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*7*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*8*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::TXA, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*9*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::TYA, &CPU::NAN,       &CPU::TXS, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*A*/ &CPU::NAN, &CPU::LDA_IND_X, &CPU::LDX_IM, &CPU::NAN, &CPU::NAN,    &CPU::LDA_ZP,   &CPU::LDX_ZP,   &CPU::NAN, &CPU::TAY, &CPU::LDA_IM,    &CPU::TAX, &CPU::NAN, &CPU::NAN,     &CPU::LDA_ABS,   &CPU::LDX_ABS,   &CPU::NAN,
        /*B*/ &CPU::NAN, &CPU::LDA_IND_Y, &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::LDA_ZP_X, &CPU::LDX_ZP_Y, &CPU::NAN, &CPU::NAN, &CPU::LDA_ABS_Y, &CPU::TSX, &CPU::NAN, &CPU::NAN,     &CPU::LDA_ABS_X, &CPU::LDX_ABS_Y, &CPU::NAN,
        /*C*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*D*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*E*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN,
        /*F*/ &CPU::NAN, &CPU::NAN,       &CPU::NAN,    &CPU::NAN, &CPU::NAN,    &CPU::NAN,      &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::NAN, &CPU::NAN, &CPU::NAN,     &CPU::NAN,       &CPU::NAN,       &CPU::NAN
    };

    ProcessorStatus pStatus;

    void Reset(Memory& memory);

    void NAN();

    Byte Fetch(s32& Cycles, Memory& memory);
    Word FetchWord(s32& Cycles, Memory& memory);
    Word FetchStackAddress();
    Byte FetchByteFromStack(s32& Cycles, Memory& memory);
    Byte ReadByte(s32& Cycles, Memory& memory, Word Addr);
    Word ReadWord(s32& Cycles, Memory& memory, Word Addr);
    void WriteByte(s32& Cycles, Memory& memory, Word Addr, Byte Value);
    void WriteWord(s32& Cycles, Memory& memory, Word Addr, Word Value);
    void PushByteToStack(s32& Cycles, Memory& memory, Byte Addr);
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
    void PLASetStatus();
    

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

    void PHA();
    void PHP();
    void PLA();
    void PLP();

};

#endif