#include <iostream>
#include "6502main.h"
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <filesystem>

int main(int argc, char *argv[])
{
    Memory memory;
    CPU cpu;
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
    loadProgram(filename, memory, offset);
    
    //Settings settings;
    Byte settings = 0;
    settings |= FLAG_INFINITE; 
    //settings |= FLAG_IGNORE;

    u32 cyclesLeft = cpu.Execute(-1, memory, settings);

    return 0;
}

void loadProgram(const std::string& filename, Memory& memory, Word memoffset)
{
    if(filename.empty())
    {
        std::cerr << "Filename is empty!" << std::endl;
        return;
    }

    if(!std::filesystem::exists(filename))
    {
        std::cerr << "File does not exist " << filename << std::endl;
        return;
    }

    std::streampos size;
    std::ifstream file (filename , std::ios::in | std::ios::binary | std::ios::ate);
    
    if(file.is_open())
    {
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        if(memoffset + size > memory.MAX_MEM)
        {
            std::cerr << "Memory offset and file size exceed memory bounds!" << std::endl;
            return;
        }
        
        if(file.read(reinterpret_cast<char*>(memory.Data + memoffset), size))
        {
            std::cout << "Success: Binary copied to memory starting at " << std::hex << std::showbase << memoffset << std::endl;
        } else
        {
            std::cerr << "Unable to copy binary to memory" << std::endl;
        }
        file.close();
        
    } else {
        std::cerr << "Failed to open program file: " << filename << std::endl;
    }
}
