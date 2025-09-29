#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <iostream>
#include <cstdint>

// MERL constants
const uint32_t MERL_MAGIC = 0x10000002;
const uint32_t MERL_END = 0x10000001;

// MERL header structure
struct MerlHeader {
    uint32_t cookie;        // 0x10000002 (MERL magic number)
    uint32_t endOfModule;   // End of module offset
    uint32_t endOfCode;     // End of code offset
};

// Essential utility function for output
void printInstruction(uint32_t instr);

#endif // INSTRUCTIONS_H
