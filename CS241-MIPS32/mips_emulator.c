#include "mipscpu.h"
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
// define word
typedef uint32_t word;
// using word = uint32_t;

// Print wrapper with timestamp and formatting
void mips_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[MIPS] ");
    vprintf(format, args);
    va_end(args);
}

// Error print wrapper
void mips_eprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "[MIPS ERROR] ");
    vfprintf(stderr, format, args);
    va_end(args);
}

// Constructor
MipsCPU *mips_cpu_create(void) {
  MipsCPU *cpu = (MipsCPU *)malloc(sizeof(MipsCPU));
  if (!cpu) {
    return NULL;
  }

  // Initialize registers
  for (int i = 0; i < 32; i++) {
    cpu->registers[i] = 0;
  }
  cpu->pc = 0;
  cpu->hi = 0;
  cpu->lo = 0;

  // Set register 31 to the exit memory address
  cpu->registers[31] = 1023 * INSTRUCTION_SIZE; // PC_INCREMENT = INSTRUCTION_SIZE

  // Allocate memory
  cpu->memory = (uint8_t *)malloc(sizeof(uint8_t) * 1024 * 1024);
  if (!cpu->memory) {
    free(cpu);
    return NULL;
  }

  return cpu;
}

// Destructor
void mips_cpu_destroy(MipsCPU *cpu) {
  if (cpu) {
    free(cpu->memory);
    free(cpu);
  }
}

void mips_cpu_load_instructions(MipsCPU *cpu, word *instructions, int count) {
  for (int i = 0; i < count; i++) {
    word ret_word = instructions[i];
    word address = INSTRUCTION_MEMORY_ADDRESS + i * INSTRUCTION_SIZE;
    // Store in big-endian format
    cpu->memory[address] = (ret_word >> 24) & 0xFF;
    cpu->memory[address + 1] = (ret_word >> 16) & 0xFF;
    cpu->memory[address + 2] = (ret_word >> 8) & 0xFF;
    cpu->memory[address + 3] = ret_word & 0xFF;
  }
}

word mips_cpu_get_instruction(MipsCPU *cpu, word address) {
  // Read INSTRUCTION_SIZE bytes in big-endian format
  word ret_word = (cpu->memory[address] << 24) |
                  (cpu->memory[address + 1] << 16) |
                  (cpu->memory[address + 2] << 8) |
                  cpu->memory[address + 3];
  return ret_word;
}

word mips_cpu_get_pc(MipsCPU *cpu) { return cpu->pc; }

void mips_cpu_run_r_instruction(MipsCPU *cpu, word instruction) {
  // function code (bits 5-0)
  word function_code = instruction & 0b111111;
  int s, t, d;

  switch (function_code) {
  // Add: add $d, $s, $t - $d = $s + $t
  // Binary: 0000 00ss ssst tttt dddd d000 0010 0000
  case 0b100000:
    d = (instruction >> 11) & 0x1F;
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: add $%d, $%d, $%d\n", d, s, t);
    cpu->registers[d] = cpu->registers[s] + cpu->registers[t];
    break;

  // Subtract: sub $d, $s, $t - $d = $s - $t
  // Binary: 0000 00ss ssst tttt dddd d000 0010 0010
  case 0b100010:
    d = (instruction >> 11) & 0x1F;
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: sub $%d, $%d, $%d\n", d, s, t);
    cpu->registers[d] = cpu->registers[s] - cpu->registers[t];
    break;

  // Multiply: mult $s, $t - $hi:$lo = $s * $t (signed)
  // Binary: 0000 00ss ssst tttt 0000 0000 0001 1000
  case 0b011000:
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: mult $%d, $%d\n", s, t);
    {
      int64_t result = (int64_t)(int32_t)cpu->registers[s] *
                       (int64_t)(int32_t)cpu->registers[t];
      cpu->hi = (word)(result >> 32);
      cpu->lo = (word)(result & 0xFFFFFFFF);
    }
    break;

  // Multiply Unsigned: multu $s, $t - $hi:$lo = $s * $t (unsigned)
  // Binary: 0000 00ss ssst tttt 0000 0000 0001 1001
  case 0b011001:
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: multu $%d, $%d\n", s, t);
    {
      uint64_t result =
          (uint64_t)cpu->registers[s] * (uint64_t)cpu->registers[t];
      cpu->hi = (word)(result >> 32);
      cpu->lo = (word)(result & 0xFFFFFFFF);
    }
    break;

  // Divide: div $s, $t - $lo = $s / $t; $hi = $s % $t (signed)
  // Binary: 0000 00ss ssst tttt 0000 0000 0001 1010
  case 0b011010:
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: div $%d, $%d\n", s, t);
    if (cpu->registers[t] != 0) {
      cpu->lo = (int32_t)cpu->registers[s] / (int32_t)cpu->registers[t];
      cpu->hi = (int32_t)cpu->registers[s] % (int32_t)cpu->registers[t];
    }
    break;

  // Divide Unsigned: divu $s, $t - $lo = $s / $t; $hi = $s % $t (unsigned)
  // Binary: 0000 00ss ssst tttt 0000 0000 0001 1011
  case 0b011011:
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    fprintf(stderr, "Executing: divu $%d, $%d\n", s, t);
    if (cpu->registers[t] != 0) {
      cpu->lo = cpu->registers[s] / cpu->registers[t];
      cpu->hi = cpu->registers[s] % cpu->registers[t];
    }
    break;

  // Move From High: mfhi $d - $d = $hi
  // Binary: 0000 0000 0000 0000 dddd d000 0001 0000
  case 0b010000: // hex 10
    d = (instruction >> 11) & 0x1F;
    fprintf(stderr, "Executing: mfhi $%d\n", d);
    cpu->registers[d] = cpu->hi;
    break;

  // Move From Low: mflo $d - $d = $lo
  // Binary: 0000 0000 0000 0000 dddd d000 0001 0010
  case 0b010010: // hex 12
    d = (instruction >> 11) & 0x1F;
    fprintf(stderr, "Executing: mflo $%d\n", d);
    cpu->registers[d] = cpu->lo;
    break;

  // Set Less Than: slt $d, $s, $t - $d = 1 if $s < $t; 0 otherwise (signed)
  // Binary: 0000 00ss ssst tttt dddd d000 0010 1010
  case 0b101010: // hex 26
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    d = (instruction >> 11) & 0x1F;
    fprintf(stderr, "Executing: slt $%d, $%d, $%d\n", d, s, t);
    cpu->registers[d] =
        (int32_t)cpu->registers[s] < (int32_t)cpu->registers[t] ? 1 : 0;
    break;

  // Set Less Than Unsigned: sltu $d, $s, $t - $d = 1 if $s < $t; 0 otherwise
  // (unsigned) Binary: 0000 00ss ssst tttt dddd d000 0010 1011
  case 0b101011: // hex 27
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    d = (instruction >> 11) & 0x1F;
    fprintf(stderr, "Executing: sltu $%d, $%d, $%d\n", d, s, t);
    cpu->registers[d] = cpu->registers[s] < cpu->registers[t] ? 1 : 0;
    break;

  // Load Immediate And Skip: lis $d - $d = MEM[pc]; pc = pc + 4
  // Binary: 0000 0000 0000 0000 dddd d000 0001 0100
  case 0b010100: // hex 20
    d = (instruction >> 11) & 0x1F;
    // executed lis set the register to the instruction at the pc
    {
      word loaded_value = mips_cpu_get_instruction(cpu, cpu->pc);
      fprintf(stderr, "Executing: lis $%d at pc: %u loading value: %u\n", d,
              cpu->pc, loaded_value);
      cpu->registers[d] = loaded_value; // PC was already incremented
    }
    cpu->pc += PC_INCREMENT; // Skip the next instruction
    break;

  // Jump Register: jr $s - pc = $s
  // Binary: 0000 00ss sss0 0000 0000 0000 0000 1000
  case 0b001000: // hex 8
    s = (instruction >> 21) & 0x1F;
    fprintf(stderr, "Executing: jr $%d\n", s);
    cpu->pc = cpu->registers[s];
    break;

  // Jump And Link Register: jalr $s - temp = $s; $31 = pc; pc = temp
  // Binary: 0000 00ss sss0 0000 0000 0000 0000 1001
  case 0b001001: // hex 9
    s = (instruction >> 21) & 0x1F;
    fprintf(stderr, "Executing: jalr $%d\n", s);
    {
      word temp = cpu->registers[s];
      cpu->registers[31] = cpu->pc; // PC was already incremented
      cpu->pc = temp;
    }
    break;
  }
}

void mips_cpu_run_i_instruction(MipsCPU *cpu, word instruction) {
  // opcode (bits 31-26)
  word opcode = instruction >> 26;
  int s, t;
  int16_t i; // immediate value (signed)

  switch (opcode) {
  // Load Word: lw $t, i($s) - $t = MEM[$s + i]
  // Binary: 1000 11ss ssst tttt iiii iiii iiii iiii
  case 0b100011: // hex 35
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    i = instruction & 0xFFFF; // sign-extend immediate
    fprintf(stderr, "Executing: lw $%d, %d($%d)\n", t, i, s);
    {
      word address = cpu->registers[s] + i;
      // Check bounds
      if (address + 3 < (1024 * 1024)) {
        // Read INSTRUCTION_SIZE bytes in big-endian format
        cpu->registers[t] = (cpu->memory[address] << 24) |
                           (cpu->memory[address + 1] << 16) |
                           (cpu->memory[address + 2] << 8) |
                           cpu->memory[address + 3];
      } else {
        fprintf(stderr, "Error: Memory access out of bounds at address %u\n", address);
        cpu->registers[t] = 0; // Set to 0 on out of bounds access
      }
    }
    break;

  // Store Word: sw $t, i($s) - MEM[$s + i] = $t
  // Binary: 1010 11ss ssst tttt iiii iiii iiii iiii
  case 0b101011: // hex 43
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    i = instruction & 0xFFFF; // sign-extend immediate
    fprintf(stderr, "Executing: sw $%d, %d($%d)\n", t, i, s);
    {
      word address = cpu->registers[s] + i;
      // Check bounds
      if (address + 3 < (1024 * 1024)) {
        // Store INSTRUCTION_SIZE bytes in big-endian format
        cpu->memory[address] = (cpu->registers[t] >> 24) & 0xFF;
        cpu->memory[address + 1] = (cpu->registers[t] >> 16) & 0xFF;
        cpu->memory[address + 2] = (cpu->registers[t] >> 8) & 0xFF;
        cpu->memory[address + 3] = cpu->registers[t] & 0xFF;
      } else {
        fprintf(stderr, "Error: Memory access out of bounds at address %u\n", address);
      }
    }
    break;
  }
}

void mips_cpu_run_system_call_instruction(MipsCPU *cpu, word instruction) {
  // opcode (bits 31-26)
  // putchar and getchar
  // rint the word 
  // he opcode for this would be 126
}

void mips_cpu_run_branch_instruction(MipsCPU *cpu, word instruction) {
  // opcode (bits 31-26)
  word opcode = instruction >> 26;
  int s, t;
  int16_t i; // immediate value (signed)

  switch (opcode) {
  // Branch On Equal: beq $s, $t, i - if ($s == $t) pc += i * 4
  // Binary: 0001 00ss ssst tttt iiii iiii iiii iiii
  case 0b000100: // hex 4
      s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    i = instruction & 0xFFFF; // sign-extend immediate
    fprintf(stderr, "Executing: beq $%d, $%d, %d\n", s, t, i);
    fprintf(stderr, "PC: %u i * PC_INCREMENT = %d\n", cpu->pc,
            i * PC_INCREMENT);
    if (cpu->registers[s] == cpu->registers[t]) {
      cpu->pc += i * PC_INCREMENT; // -1 because PC was already incremented
    }
    fprintf(stderr, "PC: %u i * PC_INCREMENT = %d\n", cpu->pc,
            i * PC_INCREMENT);
    break;

  // Branch On Not Equal: bne $s, $t, i - if ($s != $t) pc += i * 4
  // Binary: 0001 01ss ssst tttt iiii iiii iiii iiii
  case 0b000101: // hex 5
    s = (instruction >> 21) & 0x1F;
    t = (instruction >> 16) & 0x1F;
    i = instruction & 0xFFFF; // sign-extend immediate
    fprintf(stderr, "Executing: bne $%d, $%d, %d\n", s, t, i);
    if (cpu->registers[s] != cpu->registers[t]) {
      cpu->pc += i * PC_INCREMENT; // -1 because PC was already incremented
    }
    break;
  }
}

void mips_cpu_print_registers(MipsCPU *cpu) {
  mips_printf("Register dump:\n");
  for (int i = 0; i < 32; i++) {
    mips_printf("  Register %d: %u\n", i, cpu->registers[i]);
  }
}

// Read instruction from memory at current PC
int read_instruction(MipsCPU *cpu, char* instruction_bytes) {
  if (cpu->pc + (INSTRUCTION_SIZE - 1) >= (1024 * 1024)) {
    return 0; // End of memory
  }
  
  // Read INSTRUCTION_SIZE bytes from memory
  instruction_bytes[0] = cpu->memory[cpu->pc];
  instruction_bytes[1] = cpu->memory[cpu->pc + 1];
  instruction_bytes[2] = cpu->memory[cpu->pc + 2];
  instruction_bytes[3] = cpu->memory[cpu->pc + 3];
  
  return 1; // Success
}

// Decode instruction from byte array
const char* decode_instruction(const char* instruction_bytes) {
  return decode_instruction_from_bytes(instruction_bytes);
}

// Run instruction from byte array
void run_instruction(MipsCPU *cpu, const char* instruction_bytes) {
  // Convert bytes to 32-bit word
  word ret_word = ((unsigned char)instruction_bytes[0] << 24) |
                  ((unsigned char)instruction_bytes[1] << 16) |
                  ((unsigned char)instruction_bytes[2] << 8) |
                  (unsigned char)instruction_bytes[3];
  
  // get the opcode (bits 31-26)
  word opcode = ret_word >> 26;

  // Print binary representation
  fprintf(stderr, "Fetched instruction: 0b");
  for (int i = 31; i >= 0; i--) {
    fprintf(stderr, "%d", (ret_word >> i) & 1);
  }
  fprintf(stderr, ", opcode: %u, PC: %u\n", opcode, cpu->pc);

  // Increment PC before executing instruction
  cpu->pc += PC_INCREMENT;

  // Route instruction based on opcode
  if (opcode == 0) {
    // R-format instruction (opcode = 0)
    mips_cpu_run_r_instruction(cpu, ret_word);
  } else if (opcode == 0b000100 || opcode == 0b000101) {
    // Branch instructions
    mips_cpu_run_branch_instruction(cpu, ret_word);
  } else if (opcode == 0b100011 || opcode == 0b101011) {
    // Load/Store instructions
    mips_cpu_run_i_instruction(cpu, ret_word);
  }else if (opcode == 0b00001) {
    // System call instruction
    mips_cpu_run_system_call_instruction(cpu, ret_word);
  }
}

void mips_cpu_run(MipsCPU *cpu) {
  char instruction_bytes[INSTRUCTION_SIZE];
  if (read_instruction(cpu, instruction_bytes)) {
    const char* decoded = decode_instruction(instruction_bytes);
    printf("Executing: %s\n", decoded);
    run_instruction(cpu, instruction_bytes);
  }
}

