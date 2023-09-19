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
        for (uint i = 0; i < Memory::MAX_MEM; i++) {
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

    cpu.Execute(2, memory);

    REQUIRE(cpu.Acc == 0x00);

    REQUIRE(cpu.Z == 1);
    REQUIRE(cpu.N == 0);

}

TEST_CASE("LDA_ZP can Load Value into Acc")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x33;

    cpu.Execute(3, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
}

TEST_CASE("LDA_ZP_X can Load Value into Acc with offset X")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ZP_X;
    cpu.X = 0x0F;
    memory[0xFFFD] = 0x80;
    memory[0x8F] = 0x33;

    cpu.Execute(4, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
}

TEST_CASE("LDA_ABS can Load ")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ABS;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;
    memory[0x0201] = 0x33;

    cpu.Execute(4, memory);

    REQUIRE(cpu.Acc == 0x33);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);

}

TEST_CASE("LDA_ABS_X can Load address offset by x")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_LDA_ABS_X;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;
    cpu.X = 0x01;
    memory[0x202] = 0x44;

    cpu.Execute(5, memory);

    REQUIRE(cpu.Acc == 0x44);
    REQUIRE(cpu.X == 0x01);
    REQUIRE(cpu.Z == 0);
    REQUIRE(cpu.N == 0);
}

TEST_CASE("INS_JMP_ABS can change PC")
{
    cpu.Reset(memory);
    memory[0xFFFC] = CPU::INS_JMP_ABS;
    memory[0xFFFD] = 0x01;
    memory[0xFFFE] = 0x02;

    cpu.Execute(4, memory);

    REQUIRE(cpu.PC == 0x0201);

}


