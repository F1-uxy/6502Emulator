#include "Memory.h"

void Memory::Initialise()
{
    std::fill(Data, Data + MAX_MEM, 0);
}

// Read 1 Byte
Byte Memory::operator[](u32 Address) const
{
    return Data[Address];
}

// Write 1 Byte
Byte& Memory::operator[](u32 Address)
{
    return Data[Address];
}
