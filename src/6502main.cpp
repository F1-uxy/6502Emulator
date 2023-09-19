#include <iostream>
#include "6502main.h"

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);

    memory[0xFFFA] = CPU::INS_LDA_ABS_X;
    memory[0xFFFB] = 0x01;
    memory[0xFFFC] = 0x02;
    cpu.X = 0x01;
    memory[0x2010] = 0x44;

    cpu.Execute(4, memory);

    printf("Memory[0x2010] value: %d", memory[0x2010]);


    return 0;
}