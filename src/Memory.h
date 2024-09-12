#ifndef MEMORYH
#define MEMORYH

#include "CPU.h"
#include "types.h"


class Memory
{
public:
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    // Read 1 Byte
    Byte operator[](u32 Address) const;

    // Write 1 Byte
    Byte& operator[](u32 Address);

    void Initialise();
};

#endif