#include <gtest/gtest.h>
#include "../src/6502main.h"
#include "../src/CPU.h"
#include "../src/Memory.h"
/*

CPU cpu;
Memory memory;

TEST(MemoryTest, MemoryInitialisation)
{
	memory.Initialise();


	for(u32 i = 0; i < Memory::MAX_MEM; i++)
	{
		EXPECT_EQ(memory[i], 0);
	}
}

TEST(CPUTest, CPURegisterReset_PC)
{
	cpu.Reset(memory);

	// Verify program counter is correct value
	EXPECT_EQ(cpu.PC, 0xFFFC);
}

TEST(CPUTest, CPURegisterReset_StackAddress) {

    cpu.Reset(memory);

    // Verify stack address is correct value
    EXPECT_EQ(cpu.FetchStackAddress(), 0x01FF);
}

TEST(CPUTest, CPURegisterReset_Accumulator) {

    cpu.Reset(memory);

    // Verify accumulator register is correct value
    EXPECT_EQ(cpu.Acc, 0);
}

TEST(CPUTest, LoadValueIntoAccumulator_Immediate)
{	
	//LDA_IM Load value into ACC Immediate

	cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x00;

    u32 CyclesLeft = cpu.Execute(2, memory);

	EXPECT_EQ(cpu.Acc, 0x00);

	EXPECT_EQ(cpu.Z, 1);
	EXPECT_EQ(cpu.N, 0);
	EXPECT_FALSE(CyclesLeft);
}

TEST(CPUTest, LoadValueIntoXRegister_Immediate)
{	
	//LDA_IM Load value into ACC Immediate

	cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDX_IM;
    memory[0xFFFD] = 0x00;

    u32 CyclesLeft = cpu.Execute(2, memory);

	EXPECT_EQ(cpu.X, 0x00);

	EXPECT_EQ(cpu.Z, 1);
	EXPECT_EQ(cpu.N, 0);
	EXPECT_FALSE(CyclesLeft);
}

TEST(CPUTest, LoadValueIntoAccumulator)
{
	// LDA_ZP can Load Value into Acc

	cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x33;

    u32 CyclesLeft = cpu.Execute(3, memory);

    EXPECT_EQ(cpu.Acc, 0x33);
    EXPECT_EQ(cpu.Z, 0);
    EXPECT_EQ(cpu.N, 0);
    EXPECT_FALSE(CyclesLeft);
}

TEST(CPUTest, LoadValueIntoXRegister)
{
	// LDA_ZP can Load Value into Acc

	cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDX_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x33;

    u32 CyclesLeft = cpu.Execute(3, memory);

    EXPECT_EQ(cpu.X, 0x33);
    EXPECT_EQ(cpu.Z, 0);
    EXPECT_EQ(cpu.N, 0);
    EXPECT_FALSE(CyclesLeft);
}

TEST(CPUTest, LDA_ZP_X_LoadValueIntoAccWithOffsetX) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LDA_ZP_X instruction and offset X
    memory[0xFFFC] = CPU::INS_LDA_ZP_X; // LDA zero page indexed with X instruction
    cpu.X = 0x0F;
    memory[0xFFFD] = 0x80; // Address in zero page
    memory[0x8F] = 0x33; // Value at zero page address with offset X

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify accumulator register is loaded with value at zero page address with offset X
    EXPECT_EQ(cpu.Acc, 0x33);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, LDX_ZP_Y_LoadValueIntoXWithOffsetY) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LDX_ZP_Y instruction and offset Y
    memory[0xFFFC] = CPU::INS_LDX_ZP_Y; // LDX zero page indexed with Y instruction
    cpu.Y = 0x0F;
    memory[0xFFFD] = 0x80; // Address in zero page
    memory[0x8F] = 0x33; // Value at zero page address with offset Y

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify X register is loaded with value at zero page address with offset Y
    EXPECT_EQ(cpu.X, 0x33);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, LDA_ABS_LoadValueIntoAcc) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LDA_ABS instruction and absolute address
    memory[0xFFFC] = CPU::INS_LDA_ABS; // LDA absolute instruction
    memory[0xFFFD] = 0x01; // Low byte of address
    memory[0xFFFE] = 0x02; // High byte of address
    memory[0x0201] = 0x33; // Value at absolute address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify accumulator register is loaded with value at absolute address
    EXPECT_EQ(cpu.Acc, 0x33);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, LDX_ABS_LoadValueIntoX) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LDX_ABS instruction and absolute address
    memory[0xFFFC] = CPU::INS_LDX_ABS; // LDAXabsolute instruction
    memory[0xFFFD] = 0x01; // Low byte of address
    memory[0xFFFE] = 0x02; // High byte of address
    memory[0x0201] = 0x33; // Value at absolute address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify accumulator register is loaded with value at absolute address
    EXPECT_EQ(cpu.X, 0x33);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}


TEST(CPUTest, LDA_ABS_X_LoadAddressOffsetByX) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LDA_ABS_X instruction and absolute address with offset X
    memory[0xFFFC] = CPU::INS_LDA_ABS_X; // LDA absolute indexed with X instruction
    memory[0xFFFD] = 0x01; // Low byte of address
    memory[0xFFFE] = 0x02; // High byte of address
    cpu.X = 0x01; // Offset X
    memory[0x0202] = 0x44; // Value at absolute address offset by X

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify accumulator register is loaded with value at absolute address offset by X
    EXPECT_EQ(cpu.Acc, 0x44);

    // Verify X register is not changed
    EXPECT_EQ(cpu.X, 0x01);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, LDX_ABS_Y_LoadAddressOffsetByY) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with LD_ABS_X instruction and absolute address with offset Y
    memory[0xFFFC] = CPU::INS_LDX_ABS_Y; // LDX absolute indexed with Y instruction
    memory[0xFFFD] = 0x01; // Low byte of address
    memory[0xFFFE] = 0x02; // High byte of address
    cpu.Y = 0x01; // Offset Y
    memory[0x0202] = 0x44; // Value at absolute address offset by Y

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(4, memory);

    // Verify X register is loaded with value at absolute address offset by Y
    EXPECT_EQ(cpu.X, 0x44);

    // Verify Y register is not changed
    EXPECT_EQ(cpu.Y, 0x01);

    // Verify zero flag is clear
    EXPECT_EQ(cpu.Z, 0);

    // Verify negative flag is clear
    EXPECT_EQ(cpu.N, 0);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, LDA_ABS_X_LoadAddressOffsetByXJumpPage)
{
	cpu.Reset(memory);
	memory[0xFFFC] = CPU::INS_LDA_ABS_X;
	memory[0xFFFD] = 0x01;
	memory[0xFFFE] = 0x00;
	cpu.X = 0xFF;
	memory[0x100] = 0x44; //0x0001 + 0xFF = 0x100 & crosses page boundary

	u32 cyclesLeft = cpu.Execute(5, memory);

	EXPECT_EQ(cpu.Acc, 0x44);
	EXPECT_EQ(cpu.X, 0xFF);
	EXPECT_EQ(cpu.Z, 0);
	EXPECT_EQ(cpu.N, 0);

	EXPECT_EQ(cyclesLeft, 0);

}

TEST(CPUTest, LDX_ABS_Y_LoadAddressOffsetByYJumpPage)
{
	cpu.Reset(memory);
	memory[0xFFFC] = CPU::INS_LDX_ABS_Y;
	memory[0xFFFD] = 0x01;
	memory[0xFFFE] = 0x00;
	cpu.Y = 0xFF;
	memory[0x100] = 0x44; //0x0001 + 0xFF = 0x100 & crosses page boundary

	u32 cyclesLeft = cpu.Execute(5, memory);

	EXPECT_EQ(cpu.X, 0x44);
	EXPECT_EQ(cpu.Y, 0xFF);
	EXPECT_EQ(cpu.Z, 0);
	EXPECT_EQ(cpu.N, 0);

	EXPECT_EQ(cyclesLeft, 0);

}

TEST(CPUTest, LDA_ABS_IND_X_LoadIndexedIndirect)
{
	cpu.Reset(memory);
	memory[0xFFFC] = CPU::INS_LDA_IND_X;
	memory[0xFFFD] = 0x40;

	cpu.X = 0x05;
	memory[0x45] = 0x44;
	memory[0x44] = 0x33;

	u32 cyclesLeft = cpu.Execute(6, memory);

	EXPECT_EQ(cpu.Acc, 0x33);
	EXPECT_EQ(cpu.X, 0x05);
	EXPECT_EQ(cpu.Z, 0);
	EXPECT_EQ(cpu.N, 0);

}

TEST(CPUTest, JMP_ABS_ChangePC) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with INS_JMP_ABS instruction and absolute address
    memory[0xFFFC] = CPU::INS_JMP_ABS; // JMP absolute instruction
    memory[0xFFFD] = 0x01; // Low byte of address
    memory[0xFFFE] = 0x02; // High byte of address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(3, memory);

    // Verify PC register is changed to the target address
    EXPECT_EQ(cpu.PC, 0x0201);

    // Verify no cycles left
    EXPECT_EQ(cyclesLeft, 0);
}

TEST(CPUTest, JMP_IND_ChangePC) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with INS_JMP_IND instruction and indirect address
    memory[0xFFFC] = CPU::INS_JMP_IND; // JMP indirect instruction
    memory[0xFFFD] = 0x01; // Low byte of indirect address
    memory[0xFFFE] = 0x02; // High byte of indirect address
    memory[0x0201] = 0x03; // Low byte of target address
    memory[0x0202] = 0x02; // High byte of target address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(5, memory);

    // Verify PC register is changed to the target address
    EXPECT_EQ(cpu.PC, 0x0203);

    // Verify no cycles left
    EXPECT_FALSE(cyclesLeft);
}

TEST(CPUTest, JSR_PushAddressToStackAndSetPC) {
    Memory memory;
    CPU cpu;

    // Reset the CPU
    cpu.Reset(memory);

    // Set up memory with INS_JSR instruction and subroutine address
    memory[0xFFFC] = CPU::INS_JSR; // JSR instruction
    memory[0xFFFD] = 0x01; // Low byte of subroutine address
    memory[0xFFFE] = 0x02; // High byte of subroutine address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(6, memory);

    // Verify PC register is changed to the subroutine address
    EXPECT_EQ(cpu.PC, 0x0201);

    // Verify SP register is decremented
    EXPECT_EQ(cpu.SP, 0xFD);

    // Verify no cycles left
    EXPECT_EQ(cyclesLeft, 0);
}

TEST(CPUTest, AND_IM_BitwiseANDAccImmediate)
{
    Memory memory;
    CPU cpu;

    //Reset the CPUTest
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IM;
    memory[0xFFFD] = 0x11;

    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(2, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);

}

TEST(CPUTest, AND_IM_BitewiseANDAccImmediate_Negative)
{
    Memory memory;
    CPU cpu;

    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IM;
    memory[0xFFFD] = 0x80;

    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(2, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);

}

TEST(CPUTest, AND_IM_BitewiseANDAccImmediate_Zero)
{
    Memory memory;
    CPU cpu;

    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IM;
    memory[0xFFFD] = 0xF0;

    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(2, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0x11;

    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(3, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0xF0;

    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(3, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0x80;

    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(3, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP_X)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP_X;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0x11;

    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP_X_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP_X;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0x80;

    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccZP_X_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ZP_X;
    memory[0xFFFD] = 0x05;
    memory[0x0005] = 0xF0;

    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4030] = 0x11;

    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4030] = 0xF0;

    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4030] = 0x80;

    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_X)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_X;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0x11;


    cpu.X   = 0x05;
    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_X_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_X;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0xF0;


    cpu.X   = 0x05;
    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_X_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_X;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0x80;

    
    cpu.X   = 0x05;
    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_X_PageCrossed)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_X;
    memory[0xFFFD] = 0xFF;
    memory[0xFFFE] = 0x40;
    memory[0x4101] = 0x11;


    cpu.X   = 0x02;
    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(5, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Y)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_Y;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0x11;


    cpu.Y   = 0x05;
    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Y_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_Y;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0xF0;


    cpu.Y   = 0x05;
    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Y_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_Y;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = 0x40;
    memory[0x4035] = 0x80;

    
    cpu.Y   = 0x05;
    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(4, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccABS_Y_PageCrossed)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_ABS_Y;
    memory[0xFFFD] = 0xFF;
    memory[0xFFFE] = 0x40;
    memory[0x4101] = 0x11;


    cpu.Y   = 0x02;
    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(5, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_X)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_X;
    memory[0xFFFD] = 0x20;
    memory[0x0024] = 0x30;
    memory[0x0025] = 0x40;
    memory[0x4030] = 0x11;

    cpu.X   = 0x04;
    cpu.Acc = 0x51;

    u32 cyclesLeft = cpu.Execute(6, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_X_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_X;
    memory[0xFFFD] = 0x20;
    memory[0x0024] = 0x30;
    memory[0x0025] = 0x40;
    memory[0x4030] = 0xF0;

    cpu.X   = 0x04;
    cpu.Acc = 0x0F;

    u32 cyclesLeft = cpu.Execute(6, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_X_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_X;
    memory[0xFFFD] = 0x20;
    memory[0x0024] = 0x30;
    memory[0x0025] = 0x40;
    memory[0x4030] = 0x80;

    cpu.X   = 0x04;
    cpu.Acc = 0xFF;

    u32 cyclesLeft = cpu.Execute(6, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_Y)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_Y;
    memory[0xFFFD] = 0x20;
    memory[0x0020] = 0x30;
    memory[0x0021] = 0x40;
    memory[0x4035] = 0x11;

    cpu.Y   = 0x05;
    cpu.Acc = 0x51;

    int32_t cyclesLeft = cpu.Execute(5, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_Y_Negative)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_Y;
    memory[0xFFFD] = 0x20;
    memory[0x0020] = 0x30;
    memory[0x0021] = 0x40;
    memory[0x4035] = 0x80;

    cpu.Y   = 0x05;
    cpu.Acc = 0xFF;

    int32_t cyclesLeft = cpu.Execute(5, memory);

    EXPECT_EQ(cpu.Acc, 0x80);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 1);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_Y_Zero)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_Y;
    memory[0xFFFD] = 0x20;
    memory[0x0020] = 0x30;
    memory[0x0021] = 0x40;
    memory[0x4035] = 0xF0;

    cpu.Y   = 0x05;
    cpu.Acc = 0x0F;

    int32_t cyclesLeft = cpu.Execute(5, memory);

    EXPECT_EQ(cpu.Acc, 0x00);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 1);

    EXPECT_EQ(cpu.N, 0);
}

TEST(CPUTest, AND_IM_BitewiseANDAccIND_Y_Page_Crossed)
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_AND_IND_Y;
    memory[0xFFFD] = 0x20;
    memory[0x0020] = 0xFF;
    memory[0x0021] = 0x40;
    memory[0x4104] = 0x11;

    cpu.Y   = 0x05;
    cpu.Acc = 0x51;

    int32_t cyclesLeft = cpu.Execute(6, memory);

    EXPECT_EQ(cpu.Acc, 0x11);

    EXPECT_EQ(cyclesLeft, 0);

    EXPECT_EQ(cpu.Z, 0);

    EXPECT_EQ(cpu.N, 0);
}
*/