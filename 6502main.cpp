#include <iostream>
#include "6502main.h"

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);
    cpu.Reset(memory);
    /*memory[0xFFFC] = CPU::INS_LDA_ABS;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;
    memory[0x0201] = 0x33;

    cpu.Execute(4, memory);
    */

    return 0;
}

/
