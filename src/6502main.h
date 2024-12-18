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