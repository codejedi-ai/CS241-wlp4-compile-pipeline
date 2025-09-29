#include "instructions.h"
#include <iostream>

void printInstruction(uint32_t instr)
{
  unsigned char c = instr >> 24;
  std::cout << c;
  c = instr >> 16;
  std::cout << c;
  c = instr >> 8;
  std::cout << c;
  c = instr;
  std::cout << c;
}
