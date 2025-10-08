#ifndef MIPS_CPU_H
#define MIPS_CPU_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INSTRUCTION_SIZE 4 // Size of instruction in bytes (can be changed to 16 for 128-bit)
#define TERMINATE_MEMORY_ADDRESS 1023 * INSTRUCTION_SIZE
#define INSTRUCTION_MEMORY_ADDRESS 0
#define STACK_MEMORY_ADDRESS 1024 * 1024 - 1 // the bottom of the stack
#define PC_INCREMENT INSTRUCTION_SIZE // Program counter increment value

typedef struct {
    // register file
    uint32_t registers[32];
    // program counter
    uint32_t hi;
    uint32_t lo;
    uint32_t pc;
    // memory
    // need to define instruction memory and the stack memory location 
    // make the memory size a million bytes

    uint8_t* memory; 
} MipsCPU;

// Function declarations
MipsCPU* mips_cpu_create(void);
void mips_cpu_destroy(MipsCPU* cpu);
void mips_cpu_run(MipsCPU* cpu);
void mips_cpu_load_instructions(MipsCPU* cpu, uint32_t* instructions, int count);
void mips_cpu_print_registers(MipsCPU* cpu);
uint32_t mips_cpu_get_pc(MipsCPU* cpu);
uint32_t mips_cpu_get_instruction(MipsCPU* cpu, uint32_t address);

// Main execution functions
int read_instruction(MipsCPU* cpu, char* instruction_bytes);
const char* decode_instruction(const char* instruction_bytes);
void run_instruction(MipsCPU* cpu, const char* instruction_bytes);

// Print wrapper functions
void mips_printf(const char *format, ...);
void mips_eprintf(const char *format, ...);

// Private function declarations
void mips_cpu_run_r_instruction(MipsCPU* cpu, uint32_t word);
void mips_cpu_run_i_instruction(MipsCPU* cpu, uint32_t word);
void mips_cpu_run_branch_instruction(MipsCPU* cpu, uint32_t word);
void mips_cpu_run_system_call_instruction(MipsCPU* cpu, uint32_t word);

#ifdef __cplusplus
}
#endif

#endif