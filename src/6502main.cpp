#include <iostream>
#include "6502main.h"

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFC] = CPU::INS_JSR;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;

    uint CyclesLeft;

    CyclesLeft = cpu.Execute(8, memory);

    printf("PC value: %d \n", cpu.PC);
    printf("Cycles Left: %d \n", CyclesLeft);


    return 0;
}