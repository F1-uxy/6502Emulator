//
// Created by Fluxy on 14/09/2023.
//


// my_tests.cpp
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <Catch2/catch_all.hpp>
#include "../src/6502main.h"

CPU cpu;
Memory memory;


TEST_CASE("Memory Initialisation", "[memory]")
{
    memory.Initialise();

    //Check specific memory locations are a certain value e.g. (0)
    /*
    SECTION("Verify individual memory locations are zero")
    {
        REQUIRE(memory[0x0000] == 0);
    }*/

    // Check Whole memory is initialised to 0
    SECTION("Verify memory is zero in a range")
    {
        for (u32 i = 0; i < Memory::MAX_MEM; i++) {
            REQUIRE(memory[i] == 0);
        }
    }
}

TEST_CASE("CPU Register Reset", "[cpu]")
{
    cpu.Reset(memory);

    SECTION("Verify registers are correct values")
    {
        REQUIRE(cpu.PC == 0xFFFC);
        REQUIRE(cpu.FetchStackAddress() == 0x01FF);
        REQUIRE(cpu.Acc == 0);
    }

}

TEST_CASE("LDA_IM Load value into ACC Immediate")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0x00;

    u32 CyclesLeft = cpu.Execute(2, memory);

    REQUIRE(cpu.Acc == 0x00);

    REQUIRE(cpu.Z == 1);
    REQUIRE(cpu.N == 0);
    REQUIRE_FALSE(CyclesLeft);

}

TEST_CASE("LDA_ZP can Load Value into Acc")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x33;

    u32 CyclesLeft = cpu.Execute(3, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
    REQUIRE_FALSE(CyclesLeft);
}

TEST_CASE("LDA_ZP_X can Load Value into Acc with offset X")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP_X;
    cpu.X = 0x0F;
    memory[0xFFFD] = 0x80;
    memory[0x8F] = 0x33;

    u32 CyclesLeft = cpu.Execute(4, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
    REQUIRE_FALSE(CyclesLeft);
}

TEST_CASE("LDA_ABS can Load ")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ABS;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;
    memory[0x0201] = 0x33;

    u32 CyclesLeft = cpu.Execute(4, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
    REQUIRE_FALSE(CyclesLeft);

}

TEST_CASE("LDA_ABS_X can Load address offset by x")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ABS_X;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;
    cpu.X = 0x01;
    memory[0x202] = 0x44;

    u32 CyclesLeft = cpu.Execute(4, memory);

    REQUIRE(cpu.Acc == 0x44);
    REQUIRE(cpu.X == 0x01);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
    REQUIRE_FALSE(CyclesLeft);
}
/*
TEST_CASE("LDA_ABS_X can Load address offset by x and Jump page")
{
cpu.Reset(memory);
memory[0xFFFC] = CPU::INS_LDA_ABS_X;
memory[0xFFFD] = 0x01;
memory[0xFFFE] = 0x02;
cpu.X = 0x01;
memory[0x202] = 0xFF; //0x0201 + 0xFF crosses page boundary

cpu.Execute(5, memory);

REQUIRE(cpu.Acc == 0x44);
REQUIRE(cpu.X == 0x01);
REQUIRE(cpu.Z == 0);
REQUIRE(cpu.N == 0);
}
*/
TEST_CASE("INS_JMP_ABS can change PC")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_JMP_ABS;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;

    u32 CyclesLeft = cpu.Execute(3, memory);

    REQUIRE(cpu.PC == 0x0201);
    REQUIRE_FALSE(CyclesLeft);

}

TEST_CASE("INS_JMP_IND can change PC taking a 16 bit address that "
          "points to lower 8 bits of another address")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_JMP_IND;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;

    memory[0x0201] = 0x03;
    memory[0x0202] = 0x02;


    u32 CyclesLeft = cpu.Execute(5, memory);

    REQUIRE(cpu.PC == 0x0203);
    REQUIRE_FALSE(CyclesLeft);


}


TEST_CASE("INS_JSR can push address to stack and set PC to target")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_JSR;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;

    u32 CyclesLeft = cpu.Execute(6, memory);

    REQUIRE(cpu.PC == 0x0201);
    REQUIRE(cpu.SP == 0xFD);
    REQUIRE_FALSE(CyclesLeft);

}


