using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;


struct Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise();

    // Read 1 Byte
    Byte operator[](u32 Address) const;

    // Write 1 Byte
    Byte& operator[](u32 Address);
};