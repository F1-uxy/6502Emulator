#include <iostream>
#include "6502main.h"
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[])
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

    std::string filename = argv[1];
    loadProgram(filename, memory);



    return 0;
}

void loadProgram(const std::string& filename, Memory& memory)
{
    std::streampos size;
    std::ifstream file (filename , std::ios::in | std::ios::binary | std::ios::ate);
    char* memblock;
    
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }

    if(file.is_open())
    {
        size = file.tellg();
        file.seekg(0, std::ios::beg);
        memblock = new char [size];
        file.read(memblock, size);

        std::cout << memblock;
        file.close();
    } else {
        std::cerr << "Failed to open program file: " << filename << std::endl;
    }
}
