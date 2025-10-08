#include "decoder.h"
#include "mipscpu.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

// Static buffer for instruction string
static char instruction_buffer[128];

// Function to decode instruction based on compiler encoding functions
const char* decode_instruction_from_compiler(uint32_t word) {
    uint32_t opcode = word >> 26;
    
    if (opcode == 0) {
        // R-format instruction
        uint32_t function_code = word & 0x3F;  // bits 5-0
        int s = (word >> 21) & 0x1F;           // bits 25-21
        int t = (word >> 16) & 0x1F;           // bits 20-16
        int d = (word >> 11) & 0x1F;           // bits 15-11
        
        switch (function_code) {
            case 32: // add
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "add $%d, $%d, $%d", d, s, t);
                break;
            case 34: // sub
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "sub $%d, $%d, $%d", d, s, t);
                break;
            case 24: // mult
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mult $%d, $%d", s, t);
                break;
            case 25: // multu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "multu $%d, $%d", s, t);
                break;
            case 26: // div
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "div $%d, $%d", s, t);
                break;
            case 27: // divu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "divu $%d, $%d", s, t);
                break;
            case 16: // mfhi
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mfhi $%d", d);
                break;
            case 18: // mflo
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mflo $%d", d);
                break;
            case 20: // lis
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "lis $%d", d);
                break;
            case 8: // jr
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "jr $%d", s);
                break;
            case 9: // jalr
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "jalr $%d", s);
                break;
            case 42: // slt
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "slt $%d, $%d, $%d", d, s, t);
                break;
            case 43: // sltu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "sltu $%d, $%d, $%d", d, s, t);
                break;
            default:
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        ".word %u", word);
                break;
        }
    } else if (opcode == 4) { // beq
        int s = (word >> 21) & 0x1F;
        int t = (word >> 16) & 0x1F;
        int16_t i = word & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "beq $%d, $%d, %d", s, t, i);
    } else if (opcode == 5) { // bne
        int s = (word >> 21) & 0x1F;
        int t = (word >> 16) & 0x1F;
        int16_t i = word & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "bne $%d, $%d, %d", s, t, i);
    } else if (opcode == 35) { // lw
        int s = (word >> 21) & 0x1F;
        int t = (word >> 16) & 0x1F;
        int16_t i = word & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "lw $%d, %d($%d)", t, i, s);
    } else if (opcode == 43) { // sw
        int s = (word >> 21) & 0x1F;
        int t = (word >> 16) & 0x1F;
        int16_t i = word & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "sw $%d, %d($%d)", t, i, s);
    } else if (opcode == 2) { // j
        uint32_t address = word & 0x3FFFFFF;
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "j 0x%08x", address << 2);
    } else if (opcode == 3) { // jal
        uint32_t address = word & 0x3FFFFFF;
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "jal 0x%08x", address << 2);
    } else {
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "unknown (opcode=%d)", opcode);
    }
    
    return instruction_buffer;
}

// Function to decode instruction from char array (INSTRUCTION_SIZE bytes)
const char* decode_instruction_from_bytes(const char* bytes) {
    // Construct 32-bit word in big-endian format (MIPS standard)
    uint32_t word = ((unsigned char)bytes[0] << 24) |
                    ((unsigned char)bytes[1] << 16) |
                    ((unsigned char)bytes[2] << 8) |
                    (unsigned char)bytes[3];
    
    // Use the existing decoder function
    return decode_instruction_from_compiler(word);
}

// Example function showing how to use the decoder
void print_decoded_instruction(const char* bytes) {
    printf("Bytes: ");
    for (int i = 0; i < INSTRUCTION_SIZE; i++) {
        printf("0x%02x ", (unsigned char)bytes[i]);
    }
    printf("-> %s\n", decode_instruction_from_bytes(bytes));
}
void decode(const char* bytes, int num_bytes) {
    int pos = 0;
    bool previous_was_lis = false;
    
    while (pos < num_bytes) {
        if (pos + INSTRUCTION_SIZE <= num_bytes) {
            // Extract 4 bytes for the instruction
            char instruction_bytes[INSTRUCTION_SIZE];
            for (int i = 0; i < INSTRUCTION_SIZE; i++) {
                instruction_bytes[i] = bytes[pos + i];
            }
            
            // Construct 32-bit word in big-endian format (MIPS standard)
            uint32_t word = ((unsigned char)instruction_bytes[0] << 24) |
                           ((unsigned char)instruction_bytes[1] << 16) |
                           ((unsigned char)instruction_bytes[2] << 8) |
                           (unsigned char)instruction_bytes[3];
            
            const char* decoded;
            if (previous_was_lis) {
                // After lis, treat the next instruction as .word
                decoded = ".word";
                previous_was_lis = false;
            } else {
                decoded = decode_instruction_from_compiler(word);
                // Check if this is a lis instruction
                uint32_t opcode = word >> 26;
                if (opcode == 0) {
                    uint32_t function_code = word & 0x3F;
                    if (function_code == 20) { // lis
                        previous_was_lis = true;
                    }
                }
            }
            
            printf("Instruction %2d: 0x%08x (%s)\n", pos / INSTRUCTION_SIZE, word, decoded);
            
            pos += INSTRUCTION_SIZE;
        } else {
            // Handle remaining bytes that don't form a complete instruction
            printf("Warning: %d bytes remaining, not enough for complete instruction\n", num_bytes - pos);
            break;
        }
    }
}