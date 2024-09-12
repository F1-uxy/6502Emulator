#include "6502main.h"
#include "Memory.h"
#include "CPU.h"
#include "Loader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <filesystem>

int main(int argc, char *argv[])
{
    Memory memory;
    CPU cpu(memory);
    //cpu.Reset(memory);
	// Set up memory with INS_JSR instruction and subroutine address
    //memory[0xFFFC] = CPU::INS_JSR; // JSR instruction
    //memory[0xFFFD] = 0x01; // Low byte of subroutine address
    //memory[0xFFFE] = 0x02; // High byte of subroutine address

    // Execute the CPU instruction
    //
    
    if(argc < 2)
    {
        std::cerr << "I need some arguments!" << std::endl;
        return 1;
    }

    std::string filename = argv[1]; 
    Word offset = 0x0000;
    Loader::loadProgram(filename, memory, offset);
    
    //Settings settings;
    Byte settings = 0;
    settings |= FLAG_INFINITE; 
    settings |= FLAG_IGNORE;
    settings |= FLAG_DEBUG;
    settings |= FLAG_STEP;
    settings |= FLAG_FLAGS;

    u32 cyclesLeft = cpu.Execute(-1, memory, settings);

    return 0;
}
