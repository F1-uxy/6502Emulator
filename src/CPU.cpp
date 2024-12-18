#include "CPU.h"

CPU::CPU(Memory& mem) : memory(mem) {}

void CPU::Reset(Memory& memory)
{
    PC = RESET_VECTOR;
    SP = 0xFF;
    StackPage = 0x01;
    Acc = 0;
    pStatus.status = 0;
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

Byte CPU::FetchByteFromStack(s32& Cycles, Memory& memory)
{
    SP++;
    Byte Data = ReadByte(Cycles, memory, FetchStackAddress());
    return Data;
}

Word CPU::FetchWordFromStack(s32& Cycles, Memory& memory)
{
    SP++;
    Byte low = ReadByte(Cycles, memory, FetchStackAddress());

    SP++;
    Byte high = ReadByte(Cycles, memory, FetchStackAddress());
    return (static_cast<Word>(high) << 8) | low;
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

void CPU::PushByteToStack(s32& Cycles, Memory& memory, Byte Addr)
{
    WriteByte(Cycles, memory, FetchStackAddress(), Addr);
    SP--;
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

Word CPU::ReadIndirectY(s32& Cycles, Memory& memory)
{
    Word ZPPointer = Fetch(Cycles, memory);
    Word baseAddress = ReadWord(Cycles, memory, ZPPointer);
    Word effectiveAddress = baseAddress + Y;
    
    if((baseAddress & 0xFF00) != (effectiveAddress & 0xFF00))
    {
        Cycles--;
    }
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

inline Word CPU::ReadImmediate(s32& Cycles, Memory& memory)
{
    return PC;
}

// Load Byte address from Zero Page 0x00__ into given register
void CPU::LoadZP(Byte& reg, s32& Cycles, Memory& memory)
{
    Byte ZeroPageAddr = Fetch(Cycles, memory);
    reg = ReadByte(Cycles, memory, ZeroPageAddr);
}

Word CPU::ReadZP(s32& Cycles, Memory& memory)
{
    return static_cast<Word>(Fetch(Cycles, memory));
}

// Load Byte address from Zero Page 0x00__ with Byte offset into given register
void CPU::LoadZPOffset(Byte& reg, Byte& offset, s32& Cycles, Memory& memory)
{
    Byte ZeroPageAddr = Fetch(Cycles, memory);
    ZeroPageAddr += offset;
    Cycles--;
    reg = ReadByte(Cycles, memory, ZeroPageAddr);
}

Word CPU::ReadZPOffset(Byte& offset, s32& Cycles, Memory& memory)
{
    Byte ZeroPageAddr = Fetch(Cycles, memory);
    ZeroPageAddr += offset;
    return static_cast<Word>(ZeroPageAddr);
}

// Load Word address from next address in memory to given register
void CPU::LoadABS(Byte& reg, s32& Cycles, Memory& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    reg = ReadByte(Cycles, memory, AbsAddress);
}

Word CPU::ReadABS(s32& Cycles, Memory& memory)
{
    return FetchWord(Cycles, memory);
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

Word CPU::ReadABSOffset(Byte& offset, s32& Cycles, Memory& memory)
{
    Word AbsAddress = FetchWord(Cycles, memory);
    Word FinalAddress = AbsAddress + offset;

    if((AbsAddress & 0xFF00) != (FinalAddress & 0xFF00))
    {
        Cycles--;
    }

    return FinalAddress;
}

void CPU::calculateADC(Byte operand)
{
    Word sum = operand + Acc + pStatus.flags.C;
    pStatus.flags.C = (sum > 0xFF);

    bool operandSign = operand & 0x80;
    bool accSign = Acc & 0x80;
    bool resultSign = (sum & 0x80);
    pStatus.flags.V = (operandSign == accSign) && (resultSign != accSign);

    Acc =  static_cast<Byte>(sum);
}

void CPU::calculateSBC(Byte operand)
{
    Word sum = Acc - operand - (1 - pStatus.flags.C);
    pStatus.flags.C = (sum < 0x100);

    Acc =  static_cast<Byte>(sum);

    bool operandSign = operand & 0x80;
    bool accSign = Acc & 0x80;
    bool originalAccSign = Acc >= 0x80;
    pStatus.flags.V = ((originalAccSign ^ operandSign) && !(originalAccSign ^ accSign));
}

void CPU::printDebug()
{
    printf("PC: %#04x SP: %#04x Acc: %#04x\n", PC, SP, Acc);
}

void CPU::printFlags()
{
    printf("X Y N V B D I Z C\n");
    printf("%d %d %d %d %d %d %d %d %d\n", X, Y, pStatus.flags.N, pStatus.flags.V, pStatus.flags.B, pStatus.flags.D, pStatus.flags.I, pStatus.flags.Z, pStatus.flags.C);
}

void CPU::SetZeroFlag(Byte reg)
{
    pStatus.flags.Z = (reg == 0);
}

void CPU::SetNegativeFlag(Byte reg)
{
    pStatus.flags.N = (reg & 0b10000000) > 0;
}

void CPU::SetOverflowFlag(Byte reg)
{
    pStatus.flags.V = (reg & 0b01000000) > 0;
}

void CPU::SetCarryFlagMSB(Byte reg)
{
    pStatus.flags.C = (reg & 0b10000000) > 0;
}

void CPU::SetCarryFlagLSB(Byte reg)
{
    pStatus.flags.C = (reg & 0b00000001) > 0;
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

void CPU::LDYSetStatus()
{
    SetZeroFlag(Y);
    SetNegativeFlag(Y);
}

void CPU::STASetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::STXSetStatus()
{
    SetZeroFlag(X);
    SetNegativeFlag(X);
}

void CPU::STYSetStatus()
{
    SetZeroFlag(Y);
    SetNegativeFlag(Y);
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

void CPU::ASLSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetNegativeFlag(result);
}

void CPU::LSRSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetNegativeFlag(result);
}

void CPU::ROLSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetNegativeFlag(result);
}

void CPU::RORSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetNegativeFlag(result);
}

void CPU::PLASetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::ADCSetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::SBCSetStatus()
{
    SetZeroFlag(Acc);
    SetNegativeFlag(Acc);
}

void CPU::INCSetStatus(Byte result)
{
    SetZeroFlag(result);
    SetNegativeFlag(result);
}

void CPU::compare(Byte val, Byte reg)
{
    pStatus.flags.C = (reg >= val);
    pStatus.flags.Z = (reg == val);
    pStatus.flags.N = ((reg - val) & 0x80);
}

void CPU::incrementAddress(Word addr)
{
    Byte val;
    val = ReadByte(Cycles, memory, addr);
    val += 1;
    WriteByte(Cycles, memory, addr, val);
    INCSetStatus(val);
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

void CPU::branch(Byte offset)
{
    
    Byte oldPC = PC;
    PC += offset;

    if ((oldPC & 0xFF00) != (PC & 0xFF00)) {
        Cycles--;
    }

    Cycles -= 2;

}

// CPU Operation functions:

void CPU::NAN()
{
    printf("OPcode not found\n");
}

void CPU::LDA()
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

void CPU::RTS()
{
    PC = FetchWordFromStack(Cycles, memory) + 1;
}

void CPU::LDX()
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

void CPU::LDX_ABS_X()
{
    LoadABSOffset(X, X, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDX_ABS_Y()
{
    LoadABSOffset(X, Y, Cycles, memory);
    LDXSetStatus();
}

void CPU::LDY()
{
    LoadImmediate(Y, Cycles, memory);
    LDYSetStatus();
}

void CPU::LDY_ZP()
{
    LoadZP(Y, Cycles, memory);
    LDYSetStatus();
}

void CPU::LDY_ZP_X()
{
    LoadZPOffset(Y, X, Cycles, memory);
    LDYSetStatus();
}

void CPU::LDY_ABS_X()
{
    LoadABSOffset(X, X, Cycles, memory);
    LDYSetStatus();
}

void CPU::LDY_ABS()
{
    LoadABS(Y, Cycles, memory);
    LDYSetStatus();
}

void CPU::STA_ZP()
{
    Word zeroPageAddr = ReadZP(Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, Acc);
    STASetStatus();
}

void CPU::STA_ZP_X()
{
    Word zeroPageAddr = ReadZPOffset(X, Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, Acc);
    STASetStatus();
}

void CPU::STA_ABS()
{
    Word absAddr = ReadABS(Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Acc);
    STASetStatus();
}

void CPU::STA_ABS_X()
{
    Word absAddr = ReadABSOffset(X, Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Acc);
    STASetStatus();
}

void CPU::STA_ABS_Y()
{
    Word absAddr = ReadABSOffset(Y, Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Acc);
    STASetStatus();
}

void CPU::STA_IND_X()
{
    Word absAddr = AddrIndirectX(Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Acc);
    STASetStatus();
}

void CPU::STA_IND_Y()
{
    Word absAddr = ReadIndirectY(Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Acc);
    STASetStatus();
}

void CPU::STX_ZP()
{
    Word zeroPageAddr = ReadZP(Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, X);
    STXSetStatus();
}

void CPU::STX_ZP_Y()
{
    Word zeroPageAddr = ReadZPOffset(Y, Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, X);
    STXSetStatus();
}

void CPU::STX_ABS()
{
    Word absAddr = ReadABS(Cycles, memory);
    WriteByte(Cycles, memory, absAddr, X);
    STXSetStatus();
}

void CPU::STY_ZP()
{
    Word zeroPageAddr = ReadZP(Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, Y);
    STYSetStatus();
}

void CPU::STY_ZP_X()
{
    Word zeroPageAddr = ReadZPOffset(X, Cycles, memory);
    WriteByte(Cycles, memory, zeroPageAddr, Y);
    STYSetStatus();
}

void CPU::STY_ABS()
{
    Word absAddr = ReadABS(Cycles, memory);
    WriteByte(Cycles, memory, absAddr, Y);
    STYSetStatus();
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

void CPU::AND()
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

void CPU::XOR()
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

void CPU::IOR()
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

void CPU::PHA()
{
    PushByteToStack(Cycles, memory, Acc);
    Cycles-= 2;
}

void CPU::PHP()
{
    Byte storedStatus = pStatus.status;
    PushByteToStack(Cycles, memory, storedStatus);
}

void CPU::PLA()
{
    Acc = FetchByteFromStack(Cycles, memory);
    PLASetStatus();
}

void CPU::PLP()
{
    Byte newStatus = FetchByteFromStack(Cycles, memory);
    SP++;
    pStatus.status = newStatus;
}

void CPU::ASL()
{
    SetCarryFlagMSB(Acc);
    Acc <<= 1;

    ASLSetStatus(Acc);
}

void CPU::ASL_ZP()
{
    Byte zeroPageAddr = Fetch(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    SetCarryFlagMSB(data);
    data <<= 1;

    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    ASLSetStatus(data);
}

void CPU::ASL_ZP_X()
{
    Byte zeroPageAddr = Fetch(Cycles, memory);
    zeroPageAddr += X;
    
    Cycles--;

    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    SetCarryFlagMSB(data);
    data <<= 1;

    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    ASLSetStatus(data);
}

void CPU::ASL_ABS()
{
    Word absAddr = FetchWord(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, absAddr);

    SetCarryFlagMSB(data);
    data <<= 1;

    WriteByte(Cycles, memory, absAddr, data);

    ASLSetStatus(data);
}

void CPU::ASL_ABS_X()
{
    Word absAddr = FetchWord(Cycles, memory);
    absAddr += X;

    Cycles--;

    Byte data = ReadByte(Cycles, memory, absAddr);

    SetCarryFlagMSB(data);
    data <<= 1;

    WriteByte(Cycles, memory, absAddr, data);

    ASLSetStatus(data);
}

void CPU::LSR()
{
    SetCarryFlagLSB(Acc);
    Acc >>= 1;

    LSRSetStatus(Acc);
}

void CPU::LSR_ZP()
{
    Byte zeroPageAddr = Fetch(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    SetCarryFlagMSB(data);
    data >>= 1;

    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    LSRSetStatus(data);
}

void CPU::LSR_ZP_X()
{
    Byte zeroPageAddr = Fetch(Cycles, memory);
    zeroPageAddr += X;
    
    Cycles--;

    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    SetCarryFlagMSB(data);
    data >>= 1;

    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    LSRSetStatus(data);
}

void CPU::LSR_ABS()
{
    Word absAddr = FetchWord(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, absAddr);

    SetCarryFlagMSB(data);
    data >>= 1;

    WriteByte(Cycles, memory, absAddr, data);

    LSRSetStatus(data);
}

void CPU::LSR_ABS_X()
{
    Word absAddr = FetchWord(Cycles, memory);
    absAddr += X;

    Cycles--;

    Byte data = ReadByte(Cycles, memory, absAddr);

    SetCarryFlagMSB(data);
    data >>= 1;

    WriteByte(Cycles, memory, absAddr, data);

    LSRSetStatus(data);
}

void CPU::ROL()
{
    bool oldCarry = (Acc & 0b10000000) != 0;

    Acc <<= 1;
    if(pStatus.flags.C)
        Acc |= 0b00000001;
    
    pStatus.flags.C = oldCarry;

    ROLSetStatus(Acc);
}

void CPU::ROL_ZP()
{

    Byte zeroPageAddr = Fetch(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    bool oldCarry = (data & 0b10000000) != 0;

    data <<= 1;
    if(pStatus.flags.C)
        data |= 0b00000001;
    
    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    pStatus.flags.C = oldCarry;

    ROLSetStatus(data);
}

void CPU::ROL_ZP_X()
{

    Byte zeroPageAddr = Fetch(Cycles, memory);
    zeroPageAddr += X;
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    bool oldCarry = (data & 0b10000000) != 0;

    data <<= 1;
    if(pStatus.flags.C)
        data |= 0b00000001;
    
    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    pStatus.flags.C = oldCarry;

    ROLSetStatus(data);
}

void CPU::ROL_ABS()
{

    Word absAddr = FetchWord(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, absAddr);

    bool oldCarry = (data & 0b10000000) != 0;

    data <<= 1;
    if(pStatus.flags.C)
        data |= 0b00000001;
    
    WriteByte(Cycles, memory, absAddr, data);

    pStatus.flags.C = oldCarry;

    ROLSetStatus(data);
}

void CPU::ROL_ABS_X()
{

    Word absAddr = FetchWord(Cycles, memory);
    absAddr += X;
    Byte data = ReadByte(Cycles, memory, absAddr);

    bool oldCarry = (data & 0b10000000) != 0;

    data <<= 1;
    if(pStatus.flags.C)
        data |= 0b00000001;
    
    WriteByte(Cycles, memory, absAddr, data);

    pStatus.flags.C = oldCarry;

    ROLSetStatus(data);
}

void CPU::ROR()
{
    bool oldCarry = (Acc & 0b00000001) != 0;

    Acc >>= 1;
    if(pStatus.flags.C)
        Acc |= 0b10000000;
    
    pStatus.flags.C = oldCarry;

    RORSetStatus(Acc);
}

void CPU::ROR_ZP()
{

    Byte zeroPageAddr = Fetch(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    bool oldCarry = (data & 0b00000001) != 0;

    data >>= 1;
    if(pStatus.flags.C)
        data |= 0b10000000;
    
    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    pStatus.flags.C = oldCarry;

    RORSetStatus(Acc);
}

void CPU::ROR_ZP_X()
{

    Byte zeroPageAddr = Fetch(Cycles, memory);
    zeroPageAddr += X;
    Byte data = ReadByte(Cycles, memory, static_cast<Word>(zeroPageAddr));

    bool oldCarry = (data & 0b00000001) != 0;

    data >>= 1;
    if(pStatus.flags.C)
        data |= 0b10000000;
    
    WriteByte(Cycles, memory, static_cast<Word>(zeroPageAddr), data);

    pStatus.flags.C = oldCarry;

    RORSetStatus(Acc);
}

void CPU::ROR_ABS()
{

    Word absAddr = FetchWord(Cycles, memory);
    Byte data = ReadByte(Cycles, memory, absAddr);

    bool oldCarry = (data & 0b00000001) != 0;

    data >>= 1;
    if(pStatus.flags.C)
        data |= 0b10000000;
    
    WriteByte(Cycles, memory, absAddr, data);

    pStatus.flags.C = oldCarry;

    RORSetStatus(Acc);
}

void CPU::ROR_ABS_X()
{

    Word absAddr = FetchWord(Cycles, memory);
    absAddr += X;
    Byte data = ReadByte(Cycles, memory, absAddr);

    bool oldCarry = (data & 0b00000001) != 0;

    data >>= 1;
    if(pStatus.flags.C)
        data |= 0b10000000;
    
    WriteByte(Cycles, memory, absAddr, data);

    pStatus.flags.C = oldCarry;

    RORSetStatus(Acc);
}

void CPU::ADC()
{
    Byte operand = Fetch(Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_ZP()
{
    Byte operand;
    LoadZP(operand, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_ZP_X()
{
    Byte operand;
    LoadZPOffset(operand, X, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_ABS()
{
    Byte operand;
    LoadABS(operand, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_ABS_X()
{
    Byte operand;
    LoadABSOffset(operand, X, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_ABS_Y()
{
    Byte operand;
    LoadABSOffset(operand, Y, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_IND_X()
{
    Word addr = AddrIndirectX(Cycles, memory);
    Byte operand = ReadByte(Cycles, memory, addr);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::ADC_IND_Y()
{
    Byte operand;
    AddrIndirectY(operand, Cycles, memory);
    calculateADC(operand);
    ADCSetStatus();
}

void CPU::SBC()
{
    Byte operand = Fetch(Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_ZP()
{
    Byte operand;
    LoadZP(operand, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_ZP_X()
{
    Byte operand;
    LoadZPOffset(operand, X, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_ABS()
{
    Byte operand;
    LoadABS(operand, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_ABS_X()
{
    Byte operand;
    LoadABSOffset(operand, X, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_ABS_Y()
{
    Byte operand;
    LoadABSOffset(operand, Y, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_IND_X()
{
    Word addr = AddrIndirectX(Cycles, memory);
    Byte operand = ReadByte(Cycles, memory, addr);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::SBC_IND_Y()
{
    Byte operand;
    AddrIndirectY(operand, Cycles, memory);
    calculateSBC(operand);
    SBCSetStatus();
}

void CPU::CMP()
{
    Byte val;
    LoadImmediate(val, Cycles, memory);
    compare(val, Acc);
}

void CPU::CMP_ZP()
{
    Byte val;
    LoadZP(val, Cycles, memory);
    compare(val, Acc); 
}

void CPU::CMP_ZP_X()
{
    Byte val;
    LoadZPOffset(val, X, Cycles, memory);
    compare(val, Acc); 
}

void CPU::CMP_ABS()
{
    Byte val;
    LoadABS(val, Cycles, memory);
    compare(val, Acc);
}

void CPU::CMP_ABS_X()
{
    Byte val;
    LoadABSOffset(val, X, Cycles, memory);
    compare(val, Acc);
}

void CPU::CMP_ABS_Y()
{
    Byte val;
    LoadABSOffset(val, Y, Cycles, memory);
    compare(val, Acc);
}

void CPU::CMP_IND_X()
{
    Word addr = AddrIndirectX(Cycles, memory);
    Byte val = ReadByte(Cycles, memory, addr);
    compare(val, Acc);
}

void CPU::CMP_IND_Y()
{
    Byte val;
    AddrIndirectY(val, Cycles, memory);
    compare(val, Acc);
}

void CPU::CPX()
{
    Byte val;
    LoadImmediate(val, Cycles, memory);
    compare(val, X);
}

void CPU::CPX_ZP()
{
    Byte val;
    LoadZP(val, Cycles, memory);
    compare(val, X);
}

void CPU::CPX_ABS()
{
    Byte val;
    LoadABS(val, Cycles, memory);
    compare(val, X);
}

void CPU::CPY()
{
    Byte val;
    LoadImmediate(val, Cycles, memory);
    compare(val, Y);
}

void CPU::CPY_ZP()
{
    Byte val;
    LoadZP(val, Cycles, memory);
    compare(val, Y);
}

void CPU::CPY_ABS()
{
    Byte val;
    LoadABS(val, Cycles, memory);
    compare(val, Y);
}

void CPU::INC_ZP()
{
    Word addr;
    addr = ReadZP(Cycles, memory);Word
    incrementAddress(addr);
}

void CPU::INC_ZP_X()
{
    Word addr;
    addr = ReadZPOffset(X, Cycles, memory);
    incrementAddress(addr);
}

void CPU::INC_ABS()
{
    Word addr;
    addr = ReadABS(Cycles, memory);
    incrementAddress(addr);
}

void CPU::INC_ABS_X()
{
    Word addr;
    addr = ReadABSOffset(X, Cycles, memory);
    incrementAddress(addr);
}

void CPU::INX()
{
    X += 1;
    Cycles--;
}

void CPU::INY()
{
    Y += 1;
    Cycles--;
}

void CPU::DEX()
{
    X -= 1;
    Cycles--;
}

void CPU::DEY()
{
    Y -= 1;
    Cycles--;
}

void CPU::BCC()
{
    Byte offset = Fetch(Cycles, memory);

    if(!pStatus.flags.C)
    {
        branch(offset);
    }
}

void CPU::BCS()
{
    Byte offset = Fetch(Cycles, memory);

    if(pStatus.flags.C)
    {
        branch(offset);
    }
}

void CPU::BEQ()
{
    Byte offset = Fetch(Cycles, memory);

    if(pStatus.flags.Z)
    {
        branch(offset);
    }
}

void CPU::BMI()
{
    Byte offset = Fetch(Cycles, memory);

    if(pStatus.flags.N)
    {
        branch(offset);
    }
}

void CPU::BNE()
{
    Byte offset = Fetch(Cycles, memory);

    if(!pStatus.flags.Z)
    {
        branch(offset);
    }
}

void CPU::BPL()
{
    Byte offset = Fetch(Cycles, memory);

    if(!pStatus.flags.N)
    {
        branch(offset);
    }
}

void CPU::BVC()
{
    Byte offset = Fetch(Cycles, memory);

    if(!pStatus.flags.V)
    {
        branch(offset);
    }
}

void CPU::BVS()
{
    Byte offset = Fetch(Cycles, memory);

    if(pStatus.flags.V)
    {
        branch(offset);
    }
}

void CPU::CLC()
{
    pStatus.flags.C = 0;
    Cycles--;
}

void CPU::CLD()
{
    pStatus.flags.D = 0;
    Cycles--;
}

void CPU::CLI()
{
    pStatus.flags.I = 0;
    Cycles--;
}

void CPU::CLV()
{
    pStatus.flags.V = 0;
    Cycles--;
}

void CPU::SEC()
{
    pStatus.flags.C = 1;
    Cycles--;
}

void CPU::SED()
{
    pStatus.flags.D = 1;
    Cycles--;
}

void CPU::SEI()
{
    pStatus.flags.I = 1;
    Cycles--;
}

void CPU::BRK()
{
    PushPCToStack(Cycles, memory);
    PushByteToStack(Cycles, memory, pStatus.status);

    pStatus.flags.I = 1;

    Byte low = ReadByte(Cycles, memory, 0xFFFE);
    Byte high = ReadByte(Cycles, memory, 0xFFFF);
    PC = (high << 8) | low;
}

void CPU::NOP()
{
    Cycles--;
}

void CPU::RTI()
{
    pStatus.status = FetchByteFromStack(Cycles, memory);
    PC = FetchWordFromStack(Cycles, memory);
}