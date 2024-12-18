#include "Loader.h"
#include <fstream>
#include <filesystem>

void Loader::loadProgram(const std::string& filename, Memory& memory, Word memoffset)
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
