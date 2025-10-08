#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include "mipscpu.h"

// Function declarations (matching C version)
extern "C" {
    MipsCPU* mips_cpu_create(void);
    void mips_cpu_destroy(MipsCPU* cpu);
    void mips_cpu_run(MipsCPU* cpu);
    void mips_cpu_load_instructions(MipsCPU* cpu, uint32_t* instructions, int count);
    void mips_cpu_print_registers(MipsCPU* cpu);
    const char* decode_instruction_from_compiler(uint32_t word);
    const char* decode_instruction_from_bytes(const char* bytes);
    void print_decoded_instruction(const char* bytes);
    void decode(const char* bytes, int num_bytes);
    
    // Main execution functions
    int read_instruction(MipsCPU* cpu, char* instruction_bytes);
    const char* decode_instruction(const char* instruction_bytes);
    void run_instruction(MipsCPU* cpu, const char* instruction_bytes);
}

int main() {
    std::cout << "MIPS CPU Emulator with C++ File Reader" << std::endl;
    std::cout << "======================================" << std::endl;
    
    // Open the binary file
    std::ifstream file("correct_program.bin", std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file correct_program.bin" << std::endl;
        return 1;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::cout << "File size: " << file_size << " bytes" << std::endl;
    std::cout << "Number of 32-bit words: " << file_size / 4 << std::endl;
    std::cout << std::endl;
    
    // Read all bytes from file
    std::vector<uint8_t> file_bytes;
    uint8_t byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        file_bytes.push_back(byte);
    }
    
    std::cout << "Reading instructions from correct_program.bin:" << std::endl;
    std::cout << "=================================" << std::endl;
    
    // Use the new decode function
    decode(reinterpret_cast<const char*>(file_bytes.data()), file_bytes.size());
    
    // Convert bytes to instructions for CPU loading
    std::vector<uint32_t> instructions;
    for (size_t i = 0; i < file_bytes.size(); i += INSTRUCTION_SIZE) {
        if (i + INSTRUCTION_SIZE <= file_bytes.size()) {
            uint32_t word = (file_bytes[i] << 24) | 
                           (file_bytes[i + 1] << 16) | 
                           (file_bytes[i + 2] << 8) | 
                           file_bytes[i + 3];
            instructions.push_back(word);
        }
    }
    
    file.close();
    
    std::cout << std::endl;
    std::cout << "Total instructions loaded: " << instructions.size() << std::endl;
    std::cout << std::endl;
    
    // Create MIPS CPU
    MipsCPU* cpu = mips_cpu_create();
    if (!cpu) {
        std::cerr << "Error: Failed to create MIPS CPU" << std::endl;
        return 1;
    }
    
    // Load instructions into CPU
    mips_cpu_load_instructions(cpu, instructions.data(), instructions.size());
    
    std::cout << "Starting MIPS CPU execution..." << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Execute instructions using the three main functions
    int execution_count = 0;
    char instruction_bytes[INSTRUCTION_SIZE];
    
    while (read_instruction(cpu, instruction_bytes)) {
        std::cout << "Execution step " << execution_count + 1 << ":" << std::endl;
        
        // Step 1: Read instruction (already done above)
        
        // Step 2: Decode instruction
        const char* decoded = decode_instruction(instruction_bytes);
        std::cout << "Decoded: " << decoded << std::endl;
        
        // Step 3: Run instruction
        run_instruction(cpu, instruction_bytes);
        
        execution_count++;
        
        // Print register state every few steps
        if (execution_count % 5 == 0) {
            std::cout << "Register state after " << execution_count << " steps:" << std::endl;
            mips_cpu_print_registers(cpu);
            std::cout << std::endl;
        }
        
        // Safety limit
        if (execution_count > 100) {
            std::cout << "Safety limit reached, stopping execution" << std::endl;
            break;
        }
    }
    
    std::cout << "Execution completed after " << execution_count << " steps" << std::endl;
    std::cout << "Final register state:" << std::endl;
    mips_cpu_print_registers(cpu);
    
    // Clean up
    mips_cpu_destroy(cpu);
    
    std::cout << "MIPS CPU Emulator finished" << std::endl;
    return 0;
}