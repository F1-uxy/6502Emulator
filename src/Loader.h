#ifndef LOADER_H
#define LOADER_H

#include "Memory.h"
#include <string>

class Loader {
public:
    static void loadProgram(const std::string& filename, Memory& memory, Word memoffset);
};

#endif
