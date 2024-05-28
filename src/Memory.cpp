#include "CPU.h"
#include "Memory.h"

struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise()
    {
        std::fill(Data, Data + MAX_MEM, 0);
    }

    // Read 1 Byte
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }

    // Write 1 Byte
    Byte& operator[](u32 Address)
    {
        return Data[Address];
    }

};