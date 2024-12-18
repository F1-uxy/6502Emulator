# 6502 Emulator

For first time builds:
Run `cmake ..` in the root directory to initialise the build folder

To build:
Run `cmake --build .` in the `build` directory

To run main:
Run `6502Emulator` in `bin` directory

To run tests:
Run `6502EmulatorTests` in `bin` directory

The complete instruction set has been implemented but not tested

To do:
- Update and complete testing to LUT version
- Simplify instruction architecture to instruction + addressing mode instead of function for each
- Implement a PPU - this most likely to happen before the testing... Procrastinating conversating with GTest :P