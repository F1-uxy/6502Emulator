#include "CPU.h"

CPU::CPU(Memory& mem) : memory(mem) {}

void CPU::Reset(Memory& memory)
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

Byte CPU::Fetch(s32& Cycles, Memory& memory)
{
    Byte Data = memory[PC];
    PC++;
    Cycles--;
    return Data;

}

Word CPU::FetchWord(s32& Cycles, Memory& memory)
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
Word CPU::FetchStackAddress()
{
    return 0x0100 | SP;
}

Byte CPU::ReadByte(s32& Cycles, Memory& memory, Word Addr)
{
    Byte Data = memory[Addr];
    Cycles--;
    return Data;
}

Word CPU::ReadWord(s32& Cycles, Memory& memory, Word Addr)
{
    // Little Endian
    Word Address = memory[Addr];
    PC++;

    Word ReturnAddress = Address | (memory[Addr + 1] << 8);
    PC++;

    Cycles -= 2;

    return ReturnAddress;
}

void CPU::WriteByte(s32& Cycles, Memory& memory, Word Addr, Byte Value)
{
    memory[Addr] = Value;
    Cycles--;
}

void CPU::WriteWord(s32& Cycles, Memory& memory, Word Addr, Word Value)
{
    memory[Addr] = Value & 0xFF;
    memory[Addr + 1] = (Value >> 8) & 0xFF;
    Cycles -= 2;
}

void CPU::PushWordToStack(s32& Cycles, Memory& memory, Word Addr)
{
    Byte LowAddrByte = Addr & 0xFF;
    Byte HighAddrByte = (Addr >> 8) & 0xFF;
    WriteByte(Cycles, memory, FetchStackAddress(), HighAddrByte);
    SP--;
    WriteByte(Cycles, memory, FetchStackAddress(), LowAddrByte);
    SP--;
}

void CPU::PushPCMinusOneToStack(s32& Cycles, Memory& memory)
{
    PushWordToStack(Cycles, memory, PC-1);
}

void CPU::PushPCPlussOneToStack(s32& Cycles, Memory& memory)
{
    PushWordToStack(Cycles, memory, PC+1);
}

void CPU::PushPCToStack(s32& Cycles, Memory& memory)
{
    PushWordToStack(Cycles, memory, PC);
}

Word CPU::AddrIndirectX(s32& Cycles, Memory& memory)
{
    Byte ZPAddress = Fetch(Cycles, memory);
    ZPAddress = (ZPAddress + X) & 0xFF;
    Cycles --;
    Word ReturnAddress = ReadWord(Cycles, memory, ZPAddress);
    return ReturnAddress;
}

// Load Word address value stored in address given by ZPPointer value offset by Y into given register
void CPU::AddrIndirectY(Byte& reg, s32& Cycles, Memory& memory)
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
inline void CPU::LoadImmediate(Byte& reg, s32& Cycles, Memory& memory)
{
    reg = Fetch(Cycles, memory);
}

// Load Byte address from Zero Page 0x00__ into given register
void CPU::LoadZP(Byte& reg, s32& Cycles, Memory& memory)
{
    Byte ZeroPageAddr = Fetch(Cycles, memory);
    reg = ReadByte(Cycles, memory, ZeroPageAddr);
}

// Load Byte address from Zero Page 0x00__ with Byte offset into given register
void CPU::LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory)
{
    Byte ZeroPageAddr = Fetch(Cycles, memory);
    ZeroPageAddr += offset;
    Cycles--;
    reg = ReadByte(Cycles, memory, ZeroPageAddr);
}

// Load Word address from next address in memory to given register
void CPU::LoadABS(Byte& reg, s32& Cycles, Memory& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    reg = ReadByte(Cycles, memory, AbsAddress);
}

// Load Word address from next address with Byte offset to given register
void CPU::LoadABSOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word FinalAddress = AbsAddress + offset;

    if((AbsAddress & 0xFF00) != (FinalAddress & 0xFF00))
    {
        Cycles--;
    }

    reg = ReadByte(Cycles, memory, FinalAddress);
}

void CPU::printDebug()
{
    printf("PC: %#04x SP: %#04x Acc: %#04x\n", PC, SP, Acc);
}

void CPU::printFlags()
{
    printf("X Y N V B D I Z C\n");
    printf("%d %d %d %d %d %d %d %d %d\n", X, Y, N, V, B, D, I, Z, C);
}

void CPU::SetZeroFlag(Byte reg)
{
    Z = (reg == 0);
}

void CPU::SetNegativeFlag(Byte reg)
{
    N = (reg & 0b10000000) > 0;
}

void CPU::SetOverflowFlag(Byte reg)
{
    V = (reg & 0b01000000) > 0;
}

void CPU::LDASetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::LDXSetStatus()
{
    SetZeroFlag(X);
    SetNegativeFlag(X);
}

void CPU::TAXSetStatus()
{
    SetZeroFlag(X);
    SetNegativeFlag(X);
}

void CPU::TAYSetStatus()
{
    SetZeroFlag(Y);
    SetNegativeFlag(Y);
}

void CPU::ANDSetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::XORSetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::IORSetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::BITSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetOverflowFlag(result);
    SetNegativeFlag(result);
}

int CPU::Execute(s32 Cycles, Memory& memory)
{
    return Execute(Cycles, memory, 0);
}

int CPU::Execute(s32 Cycles, Memory& inputMemory, Byte settings)
{
    //Copy inputted memory to CPU class variable memory pointer - This is jank probably
    memory = inputMemory;

    while (Cycles > 0 || settings | FLAG_INFINITE)
    {
        Byte Ins = Fetch(Cycles, memory);
        if(settings & FLAG_STEP){
            //while(!std::cin.get());
            std::cin.get();
        }

        instruction_func v= opcode_lut[Ins];
        (this->*v)();

        if(settings & FLAG_DEBUG) printDebug();
        if(settings & FLAG_FLAGS) printFlags();

    }

    return Cycles;
}

// CPU Operation functions:

void CPU::NAN()
{
    printf("OPcode not found\n");
}

void CPU::LDA_IM()
{
    LoadImmediate(Acc, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_ZP()
{
    LoadZP(Acc, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_ZP_X()
{
    LoadZPOffset(Acc, X, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_ABS()
{
    LoadABS(Acc, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_ABS_X()
{
    LoadABSOffset(Acc, X, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_ABS_Y()
{
    LoadABSOffset(Acc, Y, Cycles, memory);
    LDASetStatus();
}

void CPU::LDA_IND_X()
{
    Word Address = AddrIndirectX(Cycles, memory);
    Acc = ReadByte(Cycles, memory, Address);
    LDASetStatus();
}

void CPU::LDA_IND_Y() // Needs testing
{
    Byte inputByte;
    AddrIndirectY(inputByte, Cycles, memory);
    Word addr = 0xFF00 & inputByte;
    Acc = ReadByte(Cycles, memory, addr);
    LDASetStatus();
}

void CPU::JMP_ABS()
{
    Word Address = FetchWord(Cycles, memory);
    PC = Address;
}

void CPU::JSR()
{
    Word SubAddress = FetchWord(Cycles, memory);
    PushPCMinusOneToStack(Cycles, memory);
    PC = SubAddress;
    Cycles--;
}

void CPU::JMP_IND()
{
    Word Address = FetchWord(Cycles, memory);
    PC = ReadWord(Cycles, memory, Address);
}

void CPU::LDX_IM()
{
    LoadImmediate(X, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDX_ZP()
{
    LoadZP(X, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDX_ZP_Y()
{
    LoadZPOffset(X, Y, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDX_ABS()
{
    LoadABS(X, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDX_ABS_Y()
{
    LoadABSOffset(X, Y, Cycles, memory);
    LDXSetStatus();
}

void CPU::TAX()
{
    X = Acc;
    Cycles -= 2;
    TAXSetStatus();
}

void CPU::TAY()
{
    Y = Acc;
    Cycles -= 2;
    TAYSetStatus();
}

void CPU::TSX()
{
    X = SP;
    Cycles -= 2;
    TAXSetStatus();
}

void CPU::TXA()
{
    Acc = X;
    Cycles -= 2;
    LDASetStatus();
}

void CPU::TXS()
{
    SP = X;
    Cycles -= 2;
}

void CPU::TYA()
{
    Acc = Y;
    Cycles -= 2;
    LDASetStatus();
}

void CPU::AND_IM()
{
    Byte inputByte; 
    LoadImmediate(inputByte, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_ZP()
{
    Byte inputByte;
    LoadZP(inputByte, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_ZP_X()
{
    Byte inputByte;
    LoadZPOffset(inputByte, X, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_ABS()
{
    Byte inputByte;
    LoadABS(inputByte, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_ABS_X()
{
    Byte inputByte;
    LoadABSOffset(inputByte, X, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_ABS_Y()
{
    Byte inputByte;
    LoadABSOffset(inputByte, Y, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::AND_IND_X()
{
    Word indirectAddress = AddrIndirectX(Cycles, memory);
    Acc = Acc & ReadByte(Cycles, memory, indirectAddress);
    ANDSetStatus();
}

void CPU::AND_IND_Y()
{
    Byte inputByte;
    AddrIndirectY(inputByte, Cycles, memory);
    Acc = Acc & inputByte;
    ANDSetStatus();
}

void CPU::XOR_IM()
{
    Byte inputByte;
    LoadABS(inputByte, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_ZP()
{
    Byte inputByte;
    LoadImmediate(inputByte, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_ZP_X()
{
    Byte inputByte;
    LoadZPOffset(inputByte, X, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_ABS()
{
    Byte inputByte;
    LoadABS(inputByte, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_ABS_X()
{
    Byte inputByte;
    LoadABSOffset(inputByte, X, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_ABS_Y()
{
    Byte inputByte;
    LoadABSOffset(inputByte, Y, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_IND_X()
{
    Word indirectAddress = AddrIndirectX(Cycles, memory);
    Byte inputByte = ReadByte(Cycles, memory, indirectAddress);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::XOR_IND_Y()
{
    Byte inputByte;
    AddrIndirectY(inputByte, Cycles, memory);
    Acc = Acc ^ inputByte;
    XORSetStatus();
}

void CPU::IOR_IM()
{
    Byte inputByte;
    LoadImmediate(inputByte, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_ZP()
{
    Byte inputByte;
    LoadZP(inputByte, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_ZP_X()
{
    Byte inputByte;
    LoadZPOffset(inputByte, X, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_ABS()
{
    Byte inputByte;
    LoadABS(inputByte, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_ABS_X()
{
    Byte inputByte;
    LoadABSOffset(inputByte, X, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_ABS_Y()
{
    Byte inputByte;
    LoadABSOffset(inputByte, Y, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_IND_X()
{
    Word indirectAddress = AddrIndirectX(Cycles, memory);
    Byte inputByte = ReadByte(Cycles, memory, indirectAddress);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::IOR_IND_Y()
{
    Byte inputByte;
    AddrIndirectY(inputByte, Cycles, memory);
    Acc = Acc | inputByte;
    IORSetStatus();
}

void CPU::BIT_ZP()
{
    Byte inputByte;
    Byte result;
    LoadZP(inputByte, Cycles, memory);
    result = inputByte & Acc;
    BITSetStatus(result);
}

void CPU::BIT_ABS()
{
    Word inputAddr;
    Byte inputByte;
    inputByte = ReadByte(Cycles, memory, inputAddr);
    Byte result = inputByte & Acc;
    BITSetStatus(result);
}