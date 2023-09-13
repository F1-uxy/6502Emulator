#include <iostream>
#include "6502main.h"

int main()
{
    Memory memory;
    CPU cpu;
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x84;
    cpu.Execute(3, memory);

    std::cout << cpu.FetchStackAddress();

    return 0;
}


