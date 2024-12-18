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
        /*         0              1               2              3                  4             5                 6               7           8          9                A          B          C              D                E                F*/
        /*0*/ &CPU::NAN,     &CPU::IOR_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::IOR_ZP,      &CPU::ASL_ZP,   &CPU::NAN, &CPU::PHP, &CPU::IOR,       &CPU::ASL, &CPU::NAN, &CPU::NAN,       &CPU::IOR_ABS,   &CPU::ASL_ABS,   &CPU::NAN,
        /*1*/ &CPU::NAN,     &CPU::IOR_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::IOR_ZP_X,    &CPU::ASL_ZP_X, &CPU::NAN, &CPU::NAN, &CPU::IOR_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::IOR_ABS_X, &CPU::ASL_ABS_X, &CPU::NAN,
        /*2*/ &CPU::JSR,     &CPU::AND_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::BIT_ZP,   &CPU::AND_ZP,      &CPU::ROL_ZP,   &CPU::NAN, &CPU::PLP, &CPU::AND,       &CPU::ROL, &CPU::NAN, &CPU::BIT_ABS,   &CPU::AND_ABS,   &CPU::ROL_ABS,   &CPU::NAN,
        /*3*/ &CPU::NAN,     &CPU::AND_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::AND_ZP_X,    &CPU::ROL_ZP_X, &CPU::NAN, &CPU::NAN, &CPU::AND_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::AND_ABS_X, &CPU::ROL_ABS_X, &CPU::NAN,
        /*4*/ &CPU::NAN,     &CPU::XOR_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::XOR_ZP,      &CPU::LSR_ZP,   &CPU::NAN, &CPU::PHA, &CPU::XOR,       &CPU::LSR, &CPU::NAN, &CPU::JMP_ABS,   &CPU::XOR_ABS,   &CPU::LSR_ABS,   &CPU::NAN,
        /*5*/ &CPU::NAN,     &CPU::XOR_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::XOR_ZP_X,    &CPU::LSR_ZP_X, &CPU::NAN, &CPU::NAN, &CPU::XOR_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::XOR_ABS_X, &CPU::LSR_ABS_X, &CPU::NAN,
        /*6*/ &CPU::RTS,     &CPU::ADC_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::ADC_ZP,      &CPU::ROR_ZP,   &CPU::NAN, &CPU::PLA, &CPU::ADC,       &CPU::ROR, &CPU::NAN, &CPU::JMP_IND,   &CPU::ADC_ABS,   &CPU::ROR_ABS,   &CPU::NAN,
        /*7*/ &CPU::NAN,     &CPU::ADC_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::ADC_ZP_X,    &CPU::ROR_ZP_X, &CPU::NAN, &CPU::NAN, &CPU::ADC_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::ADC_ABS_X, &CPU::ROR_ABS_X, &CPU::NAN,
        /*8*/ &CPU::NAN,     &CPU::STA_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::STY_ZP,   &CPU::STA_ZP,      &CPU::STX_ZP,   &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::TXA, &CPU::NAN, &CPU::STY_ABS,   &CPU::STA_ABS,   &CPU::STX_ABS,   &CPU::NAN,
        /*9*/ &CPU::NAN,     &CPU::STA_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::STY_ZP_X, &CPU::STA_ZP_X,    &CPU::STX_ZP_Y, &CPU::NAN, &CPU::TYA, &CPU::STA_ABS_Y, &CPU::TXS, &CPU::NAN, &CPU::NAN,       &CPU::STA_ABS_X, &CPU::NAN,       &CPU::NAN,
        /*A*/ &CPU::LDY,     &CPU::LDA_IND_X, &CPU::LDX,     &CPU::NAN,        &CPU::LDY_ZP,   &CPU::LDA_ZP,      &CPU::LDX_ZP,   &CPU::NAN, &CPU::TAY, &CPU::LDA,       &CPU::TAX, &CPU::NAN, &CPU::LDY_ABS,   &CPU::LDA_ABS,   &CPU::LDX_ABS_X, &CPU::NAN,
        /*B*/ &CPU::NAN,     &CPU::LDA_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::LDY_ZP_X, &CPU::LDA_ZP_X,    &CPU::LDX_ZP_Y, &CPU::NAN, &CPU::NAN, &CPU::LDA_ABS_Y, &CPU::TSX, &CPU::NAN, &CPU::LDY_ABS_X, &CPU::LDA_ABS_X, &CPU::LDX_ABS_Y, &CPU::NAN,
        /*C*/ &CPU::CPY,     &CPU::CMP_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::CPY_ZP,   &CPU::CMP_ZP,      &CPU::NAN,      &CPU::NAN, &CPU::INY, &CPU::CMP,       &CPU::NAN, &CPU::NAN, &CPU::CPY_ABS,   &CPU::CMP_ABS,   &CPU::NAN,       &CPU::NAN,
        /*D*/ &CPU::NAN,     &CPU::CMP_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::CMP_ZP_X,    &CPU::NAN,      &CPU::NAN, &CPU::NAN, &CPU::CMP_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::CMP_ABS_X, &CPU::NAN,       &CPU::NAN,
        /*E*/ &CPU::CPX,     &CPU::SBC_IND_X, &CPU::NAN,     &CPU::NAN,        &CPU::CPX_ZP,   &CPU::SBC_ZP,      &CPU::INC_ZP,   &CPU::NAN, &CPU::INX, &CPU::SBC,       &CPU::NAN, &CPU::NAN, &CPU::CPX_ABS,   &CPU::SBC_ABS,   &CPU::INC_ABS,   &CPU::NAN,
        /*F*/ &CPU::NAN,     &CPU::SBC_IND_Y, &CPU::NAN,     &CPU::NAN,        &CPU::NAN,      &CPU::SBC_ZP_X,    &CPU::INC_ZP_X, &CPU::NAN, &CPU::NAN, &CPU::SBC_ABS_Y, &CPU::NAN, &CPU::NAN, &CPU::NAN,       &CPU::SBC_ABS_X, &CPU::INC_ABS_X, &CPU::NAN
    };

    ProcessorStatus pStatus;

    void Reset(Memory& memory);

    void NAN();

    Byte Fetch(s32& Cycles, Memory& memory);
    Word FetchWord(s32& Cycles, Memory& memory);
    Word FetchStackAddress();
    Byte FetchByteFromStack(s32& Cycles, Memory& memory);
    Word FetchWordFromStack(s32& Cycles, Memory& memory);
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
    inline Word ReadImmediate(s32& Cycles, Memory& memory);
    void LoadZP(Byte& reg, s32& Cycles, Memory& memory);
    Word ReadZP(s32& Cycles, Memory& memory);
    void LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);
    Word ReadZPOffset(Byte& offset, s32& Cycles, Memory& memory);
    void LoadABS(Byte& reg, s32& Cycles, Memory& memory);
    Word ReadABS(s32& Cycles, Memory& memory);
    void LoadABSOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory);
    Word ReadABSOffset(Byte& offset, s32& Cycles, Memory& memory);
    Word ReadIndirectY(s32& Cycles, Memory& memory);

    void calculateADC(Byte operand);
    void calculateSBC(Byte operand);
    void compare(Byte val, Byte reg);
    void incrementAddress(Word addr);
    void branch(Byte offset);

    void printDebug();
    void printFlags();
    void SetZeroFlag(Byte reg);
    void SetNegativeFlag(Byte reg);
    void SetOverflowFlag(Byte reg);
    void SetCarryFlagMSB(Byte reg);
    void SetCarryFlagLSB(Byte reg);

    void LDASetStatus();
    void LDXSetStatus();
    void LDYSetStatus();
    void STASetStatus();
    void STXSetStatus();
    void STYSetStatus();
    void TAXSetStatus();
    void TAYSetStatus();
    void ANDSetStatus();
    void XORSetStatus();
    void IORSetStatus();
    void BITSetStatus(Byte result);
    void PLASetStatus();
    void ASLSetStatus(Byte result);
    void LSRSetStatus(Byte result);
    void ROLSetStatus(Byte result);
    void RORSetStatus(Byte result);
    void ADCSetStatus();
    void SBCSetStatus();
    void INCSetStatus(Byte result);
    

    // Operation Functions

    //Load/Store Operations:
    void LDA();
    void LDA_ZP();
    void LDA_ZP_X();
    void LDA_ABS();
    void LDA_ABS_X();
    void LDA_ABS_Y();
    void LDA_IND_X();
    void LDA_IND_Y();

    void LDX();
    void LDX_ZP();
    void LDX_ZP_Y();
    void LDX_ABS_X();
    void LDX_ABS_Y();

    void LDY();
    void LDY_ZP();
    void LDY_ZP_X();
    void LDY_ABS();
    void LDY_ABS_X();

    void STA_ZP();
    void STA_ZP_X();
    void STA_ABS();
    void STA_ABS_X();
    void STA_ABS_Y();
    void STA_IND_X();
    void STA_IND_Y();

    void STX_ZP();
    void STX_ZP_Y();
    void STX_ABS();

    void STY_ZP();
    void STY_ZP_X();
    void STY_ABS();

    // Transfer Operations:
    void TAX();
    void TAY();
    void TXA();
    void TYA();

    // Stack Operations:
    void TSX();
    void TXS();
    void PHA();
    void PHP();
    void PLA();
    void PLP();

    // Arithmetic Operations:
    void ADC();
    void ADC_ZP();
    void ADC_ZP_X();
    void ADC_ABS();
    void ADC_ABS_X();
    void ADC_ABS_Y();
    void ADC_IND_X();
    void ADC_IND_Y();

    void SBC();
    void SBC_ZP();
    void SBC_ZP_X();
    void SBC_ABS();
    void SBC_ABS_X();
    void SBC_ABS_Y();
    void SBC_IND_X();
    void SBC_IND_Y();

    void CMP();
    void CMP_ZP();
    void CMP_ZP_X();
    void CMP_ABS();
    void CMP_ABS_X();
    void CMP_ABS_Y();
    void CMP_IND_X();
    void CMP_IND_Y();

    void CPX();
    void CPX_ZP();
    void CPX_ABS();

    void CPY();
    void CPY_ZP();
    void CPY_ABS();

    // Logical Operations
    void AND();
    void AND_ZP();
    void AND_ZP_X();
    void AND_ABS();
    void AND_ABS_X();
    void AND_ABS_Y();
    void AND_IND_X();
    void AND_IND_Y();

    void XOR();
    void XOR_ZP();
    void XOR_ZP_X();
    void XOR_ABS();
    void XOR_ABS_X();
    void XOR_ABS_Y();
    void XOR_IND_X();
    void XOR_IND_Y();

    void IOR();
    void IOR_ZP();
    void IOR_ZP_X();
    void IOR_ABS();
    void IOR_ABS_X();
    void IOR_ABS_Y();
    void IOR_IND_X();
    void IOR_IND_Y();

    void BIT_ZP();
    void BIT_ABS();

    // Shift Operations
    void ASL();
    void ASL_ZP();
    void ASL_ZP_X();
    void ASL_ABS();
    void ASL_ABS_X();

    void LSR();
    void LSR_ZP();
    void LSR_ZP_X();
    void LSR_ABS();
    void LSR_ABS_X();

    void ROL();
    void ROL_ZP();
    void ROL_ZP_X();
    void ROL_ABS();
    void ROL_ABS_X();

    void ROR();
    void ROR_ZP();
    void ROR_ZP_X();
    void ROR_ABS();
    void ROR_ABS_X();

    // Increment & Decrements
    void INC_ZP();
    void INC_ZP_X();
    void INC_ABS();
    void INC_ABS_X();

    void INX();
    void INY();

    void DEC();
    void DEC_ZP();
    void DEC_ZP_X();
    void DEC_ABS();
    void DEC_ABS_X();

    void DEX();
    void DEY();

    // Jumps and Calls
    void JMP_ABS();
    void JSR();
    void JMP_IND();
    void RTS();

    // Branches
    void BCC();
    void BCS();
    void BEQ();
    void BMI();
    void BNE();
    void BPL();
    void BVC();
    void BVS();

    // Flag Operations
    void CLC();
    void CLD();
    void CLI();
    void CLV();
    void SEC(); // Washed southern conference
    void SED();
    void SEI();

    // System Functions
    void BRK();
    void NOP();
    void RTI();

};

#endif