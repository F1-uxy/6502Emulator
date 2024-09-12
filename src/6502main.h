#ifndef CPUEMULATOR_6502MAIN_H
#define CPUEMULATOR_6502MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "CPU.h"
#include "Memory.h"
#include "types.h"

class Memory;


static constexpr Byte FLAG_DEBUG    = 0x01;
static constexpr Byte FLAG_INFINITE = 0x02;
static constexpr Byte FLAG_IGNORE   = 0x04;
static constexpr Byte FLAG_STEP     = 0x08;
static constexpr Byte FLAG_FLAGS    = 0x10;





#endif CPUEMULATOR_6502MAIN_H

/*

struct Settings {
    Byte flags = 0;

    void setFlag(Byte flag)
    {
        flags |= flag;
    }

    void clearFlag(Byte flag)
    {
        flag &= ~flag;
    }

    bool hasFlag(Byte flag)
    {
        return (flag & flags) != 0;
    }

    bool operator|(const Byte other) const {
        return this->flags | other;
    }

    Settings& operator|=(const Byte other) {
        this->flags |= other;
        return *this;
    }
};


// Opcodes
static constexpr Byte
    INS_LDA_IM      = 0xA9,
    INS_LDA_ZP      = 0xA5,
    INS_LDA_ZP_X    = 0xB5,
    INS_LDA_ABS     = 0xAD,
    INS_LDA_ABS_X   = 0xBD,
    INS_LDA_ABS_Y   = 0xB9,
    INS_LDA_IND_X   = 0xA1,
    INS_LDA_IND_Y   = 0XB1,
    INS_JMP_ABS     = 0x4C,
    INS_JSR         = 0x20,
    INS_JMP_IND     = 0x6C,
    INS_LDX_IM      = 0xA2,
    INS_LDX_ZP      = 0xA6,
    INS_LDX_ZP_Y    = 0xB6,
    INS_LDX_ABS     = 0xAE,
    INS_LDX_ABS_Y   = 0xBE,
    INS_TAX         = 0xAA,
    INS_TAY         = 0xA8,
    INS_TSX         = 0xBA,
    INS_TXA         = 0x8A,
    INS_TXS         = 0x9A,
    INS_TYA         = 0x98,
    INS_AND_IM      = 0x29,
    INS_AND_ZP      = 0x25,
    INS_AND_ZP_X    = 0x35,
    INS_AND_ABS     = 0x2D,
    INS_AND_ABS_X   = 0x3D,
    INS_AND_ABS_Y   = 0x39,
    INS_AND_IND_X   = 0x21,
    INS_AND_IND_Y   = 0x31,
    INS_XOR_IM      = 0x49,
    INS_XOR_ZP      = 0x45,
    INS_XOR_ZP_X    = 0x55,
    INS_XOR_ABS     = 0x4D,
    INS_XOR_ABS_X   = 0x5D,
    INS_XOR_ABS_Y   = 0x59,
    INS_XOR_IND_X   = 0x41,
    INS_XOR_IND_Y   = 0x51,
    INS_IOR_IM      = 0x09,
    INS_IOR_ZP      = 0x05,
    INS_IOR_ZP_X    = 0x15,
    INS_IOR_ABS     = 0x0D,
    INS_IOR_ABS_X   = 0x1D,
    INS_IOR_ABS_Y   = 0x19,
    INS_IOR_IND_X   = 0x01,
    INS_IOR_IND_Y   = 0x11,
    INS_BIT_ZP      = 0x24,
    INS_BIT_ABS     = 0x2C;
*/
  
	        
    /*
    switch (Ins)
    {
    case INS_LDA_IM:
    {

    } break;
    // Zero Page Access Mode
    case INS_LDA_ZP:
    {
    } break;
    case INS_LDA_ZP_X:
    {

    } break;
    case INS_LDA_ABS:
    {

    } break;
    case INS_LDA_ABS_X:
    {

    }break;
    case INS_LDA_ABS_Y:
    {


    }break;
    //  6502 Cannot obtain address if indirect vector falls on a page boundary
    case INS_LDA_IND_X:
    {

    }break;
    case INS_LDX_IM:
    {

    }break;
    case INS_LDX_ZP:
    {

    }break;
    case INS_LDX_ZP_Y:
    {

    }break;
    case INS_LDX_ABS:
    {

    }break;
    case INS_LDX_ABS_Y:
    {

    }break;
    case INS_JMP_ABS:
    {

    } break;
    case INS_JMP_IND:
    {

    }break;
    case INS_JSR:
    {

    } break;
    case INS_TAX:
    {

    } break;
    case INS_TAY:
    {

    } break;
    case INS_TSX:
    {

    } break;
    case INS_TXA:
    {

    } break;
    case INS_TXS:
    {

    } break;
    case INS_TYA:
    {

    } break;
    case INS_AND_IM:
    {

    }break;
    case INS_AND_ZP:
    {

    }break;
    case INS_AND_ZP_X:
    {

    }break;
    case INS_AND_ABS:
    {

    }break;
    case INS_AND_ABS_X:
    {

    }break;
    case INS_AND_ABS_Y:
    {

    }break;
    case INS_AND_IND_X:
    {

    }break;
    case INS_AND_IND_Y:
    {

    }break;
    case INS_XOR_IM:
    {

    }break;
    case INS_XOR_ZP:
    {

    }break;
    case INS_XOR_ZP_X:
    {

    }break;
    case INS_XOR_ABS:
    {

    }break;
    case INS_XOR_ABS_X:
    {

    }break;
    case INS_XOR_ABS_Y:
    {

    }break;
    case INS_XOR_IND_X:
    {

    }break;
    case INS_XOR_IND_Y:
    {

    }break;
    case INS_IOR_IM:
    {

    }break;
    case INS_IOR_ZP:
    {

    }break;
    case INS_IOR_ZP_X:
    {

    }break;
    case INS_IOR_ABS:
    {

    }break;
    case INS_IOR_ABS_X:
    {

    }break;
    case INS_IOR_ABS_Y:
    {

    }break;
    case INS_IOR_IND_X:
    {

    }break;
    case INS_IOR_IND_Y:
    {

    }break;
    case INS_BIT_ZP:
    {

    }break;
    case INS_BIT_ABS:
    {

    }break;
    default:
    {
    printf("Instruction not found: %d \n", Ins);
    printf("PC value: %d \n", PC);

    if(!(settings & FLAG_IGNORE)) return -1;
    } break;
    }

*/

