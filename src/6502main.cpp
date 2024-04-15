#include <iostream>
#include "6502main.h"

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);
	// Set up memory with INS_JSR instruction and subroutine address
    memory[0xFFFC] = CPU::INS_JSR; // JSR instruction
    memory[0xFFFD] = 0x01; // Low byte of subroutine address
    memory[0xFFFE] = 0x02; // High byte of subroutine address

    // Execute the CPU instruction
    u32 cyclesLeft = cpu.Execute(6, memory);





    return 0;
}