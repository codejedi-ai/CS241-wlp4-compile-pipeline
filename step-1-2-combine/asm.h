#ifndef ASM_H
#define ASM_H
#include <string>
#include <vector>
#include <cstdint>
#include <vector>
using namespace std;
using word = uint32_t;

vector<word> asmAssemble(std::string input_asm_contents);
void write_word_as_binary(uint32_t word, ostream &out);
#endif