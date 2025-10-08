#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function to decode instruction based on compiler encoding functions
const char* decode_instruction_from_compiler(uint32_t word);

// Function to decode instruction from char array (4 bytes)
const char* decode_instruction_from_bytes(const char* bytes);

// Example function to print decoded instruction
void print_decoded_instruction(const char* bytes);

// Function to decode a sequence of bytes
void decode(const char* bytes, int num_bytes);

#ifdef __cplusplus
}
#endif

#endif
