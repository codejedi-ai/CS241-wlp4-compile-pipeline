#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include <algorithm>
#include <cstdlib>

class MipsEmulator {
private:
    // MIPS registers ($0-$31)
    std::map<std::string, int> registers;
    
    // Memory simulation
    std::map<int, int> memory;
    
    // Program counter
    size_t pc;
    
    // Labels for jumps
    std::map<std::string, size_t> labels;
    
    // Program lines
    std::vector<std::string> program_lines;
    
    // Helper function to trim whitespace
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
    
    // Helper function to split string by delimiter
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }
    
    // Parse register name
    std::string parse_register(const std::string& reg_str) {
        std::string reg = trim(reg_str);
        if (reg.back() == ',') {
            reg = reg.substr(0, reg.length() - 1);
        }
        return trim(reg);
    }
    
    // Parse immediate value
    int parse_immediate(const std::string& imm_str) {
        std::string imm = trim(imm_str);
        if (imm.back() == ',') {
            imm = imm.substr(0, imm.length() - 1);
        }
        try {
            return std::stoi(trim(imm));
        } catch (const std::invalid_argument& e) {
            return 0;
        }
    }
    
    // Parse memory access (offset(register))
    std::pair<int, std::string> parse_memory_access(const std::string& mem) {
        std::regex mem_regex(R"((-?\d+)\(([^)]+)\))");
        std::smatch match;
        if (std::regex_match(mem, match, mem_regex)) {
            return {std::stoi(match[1].str()), match[2].str()};
        }
        return {0, mem};
    }
    
    // Initialize registers
    void init_registers() {
        for (int i = 0; i < 32; i++) {
            registers["$" + std::to_string(i)] = 0;
        }
        // Set up named registers
        registers["$zero"] = 0;
        registers["$at"] = 0;
        registers["$v0"] = 0;
        registers["$v1"] = 0;
        registers["$a0"] = 0;
        registers["$a1"] = 0;
        registers["$a2"] = 0;
        registers["$a3"] = 0;
        registers["$t0"] = 0;
        registers["$t1"] = 0;
        registers["$t2"] = 0;
        registers["$t3"] = 0;
        registers["$t4"] = 0;
        registers["$t5"] = 0;
        registers["$t6"] = 0;
        registers["$t7"] = 0;
        registers["$s0"] = 0;
        registers["$s1"] = 0;
        registers["$s2"] = 0;
        registers["$s3"] = 0;
        registers["$s4"] = 0;
        registers["$s5"] = 0;
        registers["$s6"] = 0;
        registers["$s7"] = 0;
        registers["$t8"] = 0;
        registers["$t9"] = 0;
        registers["$k0"] = 0;
        registers["$k1"] = 0;
        registers["$gp"] = 0;
        registers["$sp"] = 0;
        registers["$fp"] = 0;
        registers["$ra"] = 0;
    }
    
    // Execute instruction
    void execute_instruction(const std::string& line) {
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') {
            return;
        }
        
        // Skip directives
        if (trimmed[0] == '.') {
            return;
        }
        
        // Remove comments
        size_t comment_pos = trimmed.find(';');
        if (comment_pos != std::string::npos) {
            trimmed = trimmed.substr(0, comment_pos);
            trimmed = trim(trimmed);
        }
        
        if (trimmed.empty()) return;
        
        // Split into tokens
        std::vector<std::string> tokens = split(trimmed, ' ');
        if (tokens.empty()) return;
        
        std::string instruction = tokens[0];
        
        // Execute different instruction types
        if (instruction == "add") {
            execute_add(tokens);
        } else if (instruction == "addi") {
            execute_addi(tokens);
        } else if (instruction == "sub") {
            execute_sub(tokens);
        } else if (instruction == "mult") {
            execute_mult(tokens);
        } else if (instruction == "div") {
            execute_div(tokens);
        } else if (instruction == "mfhi") {
            execute_mfhi(tokens);
        } else if (instruction == "mflo") {
            execute_mflo(tokens);
        } else if (instruction == "lw") {
            execute_lw(tokens);
        } else if (instruction == "sw") {
            execute_sw(tokens);
        } else if (instruction == "lis") {
            execute_lis(tokens);
        } else if (instruction == "beq") {
            execute_beq(tokens);
        } else if (instruction == "bne") {
            execute_bne(tokens);
        } else if (instruction == "j") {
            execute_j(tokens);
        } else if (instruction == "jal") {
            execute_jal(tokens);
        } else if (instruction == "jr") {
            execute_jr(tokens);
        } else if (instruction == "jalr") {
            execute_jalr(tokens);
        } else if (instruction == "slt") {
            execute_slt(tokens);
        } else if (instruction == "slti") {
            execute_slti(tokens);
        } else if (instruction == "print") {
            execute_print(tokens);
        } else if (instruction == "println") {
            execute_println(tokens);
        } else if (instruction == "new") {
            execute_new(tokens);
        } else if (instruction == "delete") {
            execute_delete(tokens);
        } else if (instruction == "init") {
            execute_init(tokens);
        } else if (instruction == "syscall") {
            execute_syscall(tokens);
        } else if (instruction == "move") {
            execute_move(tokens);
        } else if (instruction == "nop") {
            // No operation
        }
    }
    
    // Arithmetic instructions
    void execute_add(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rd = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            std::string rt = parse_register(tokens[3]);
            registers[rd] = registers[rs] + registers[rt];
        }
    }
    
    void execute_addi(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rt = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            int immediate = parse_immediate(tokens[3]);
            registers[rt] = registers[rs] + immediate;
        }
    }
    
    void execute_sub(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rd = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            std::string rt = parse_register(tokens[3]);
            registers[rd] = registers[rs] - registers[rt];
        }
    }
    
    void execute_mult(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 3) {
            std::string rs = parse_register(tokens[1]);
            std::string rt = parse_register(tokens[2]);
            long long result = (long long)registers[rs] * (long long)registers[rt];
            registers["$hi"] = (result >> 32) & 0xFFFFFFFF;
            registers["$lo"] = result & 0xFFFFFFFF;
        }
    }
    
    void execute_div(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 3) {
            std::string rs = parse_register(tokens[1]);
            std::string rt = parse_register(tokens[2]);
            if (registers[rt] != 0) {
                registers["$lo"] = registers[rs] / registers[rt];
                registers["$hi"] = registers[rs] % registers[rt];
            }
        }
    }
    
    void execute_mfhi(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string rd = parse_register(tokens[1]);
            registers[rd] = registers["$hi"];
        }
    }
    
    void execute_mflo(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string rd = parse_register(tokens[1]);
            registers[rd] = registers["$lo"];
        }
    }
    
    // Memory instructions
    void execute_lw(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 3) {
            std::string rt = parse_register(tokens[1]);
            auto mem = parse_memory_access(tokens[2]);
            int offset = mem.first;
            std::string rs = parse_register(mem.second);
            int address = registers[rs] + offset;
            registers[rt] = memory[address];
        }
    }
    
    void execute_sw(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 3) {
            std::string rt = parse_register(tokens[1]);
            auto mem = parse_memory_access(tokens[2]);
            int offset = mem.first;
            std::string rs = parse_register(mem.second);
            int address = registers[rs] + offset;
            memory[address] = registers[rt];
        }
    }
    
    void execute_lis(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string rd = parse_register(tokens[1]);
            // lis instruction - next line should be .word
            if (pc + 1 < program_lines.size()) {
                std::string next_line = trim(program_lines[pc + 1]);
                if (next_line.find(".word") != std::string::npos) {
                    std::vector<std::string> word_tokens = split(next_line, ' ');
                    if (word_tokens.size() >= 2) {
                        try {
                            registers[rd] = std::stoi(word_tokens[1]);
                            pc++; // Skip the .word line
                        } catch (const std::invalid_argument& e) {
                            // Handle invalid number
                            registers[rd] = 0;
                        }
                    }
                }
            }
        }
    }
    
    // Branch instructions
    void execute_beq(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rs = parse_register(tokens[1]);
            std::string rt = parse_register(tokens[2]);
            std::string label = tokens[3];
            if (registers[rs] == registers[rt]) {
                if (labels.find(label) != labels.end()) {
                    pc = labels[label];
                }
            }
        }
    }
    
    void execute_bne(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rs = parse_register(tokens[1]);
            std::string rt = parse_register(tokens[2]);
            std::string label = tokens[3];
            if (registers[rs] != registers[rt]) {
                if (labels.find(label) != labels.end()) {
                    pc = labels[label];
                }
            }
        }
    }
    
    // Jump instructions
    void execute_j(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string label = tokens[1];
            if (labels.find(label) != labels.end()) {
                pc = labels[label];
            }
        }
    }
    
    void execute_jal(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            registers["$ra"] = pc + 1; // Save return address
            std::string label = tokens[1];
            if (labels.find(label) != labels.end()) {
                pc = labels[label];
            }
        }
    }
    
    void execute_jr(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string rs = parse_register(tokens[1]);
            pc = registers[rs];
        }
    }
    
    void execute_jalr(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string rs = parse_register(tokens[1]);
            registers["$ra"] = pc + 1; // Save return address
            pc = registers[rs];
        }
    }
    
    // Comparison instructions
    void execute_slt(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rd = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            std::string rt = parse_register(tokens[3]);
            registers[rd] = (registers[rs] < registers[rt]) ? 1 : 0;
        }
    }
    
    void execute_slti(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 4) {
            std::string rt = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            int immediate = parse_immediate(tokens[3]);
            registers[rt] = (registers[rs] < immediate) ? 1 : 0;
        }
    }
    
    // Runtime functions
    void execute_print(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string reg = parse_register(tokens[1]);
            std::cout << registers[reg];
        }
    }
    
    void execute_println(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string reg = parse_register(tokens[1]);
            std::cout << registers[reg] << std::endl;
        }
    }
    
    void execute_new(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string reg = parse_register(tokens[1]);
            // Simple malloc simulation - just return a memory address
            static int next_address = 1000;
            registers[reg] = next_address;
            next_address += 4;
        }
    }
    
    void execute_delete(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 2) {
            std::string reg = parse_register(tokens[1]);
            // Simple free simulation - do nothing
            (void)reg; // Suppress unused warning
        }
    }
    
    void execute_init(const std::vector<std::string>& tokens) {
        // Initialize memory allocator - do nothing in simple emulator
        (void)tokens; // Suppress unused parameter warning
    }
    
    void execute_syscall(const std::vector<std::string>& tokens) {
        // Handle syscalls based on $v0 value
        (void)tokens; // Suppress unused parameter warning
        int syscall_num = registers["$v0"];
        switch (syscall_num) {
            case 1: // print integer
                std::cout << registers["$a0"];
                break;
            case 2: // print float
                std::cout << registers["$a0"];
                break;
            case 3: // print double
                std::cout << registers["$a0"];
                break;
            case 4: // print string
                std::cout << registers["$a0"];
                break;
            case 5: // read integer
                std::cin >> registers["$v0"];
                break;
            case 10: // exit
                exit(0);
                break;
            default:
                break;
        }
    }
    
    void execute_move(const std::vector<std::string>& tokens) {
        if (tokens.size() >= 3) {
            std::string rd = parse_register(tokens[1]);
            std::string rs = parse_register(tokens[2]);
            registers[rd] = registers[rs];
        }
    }
    
public:
    MipsEmulator() : pc(0) {
        init_registers();
    }
    
    void load_program(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        
        while (std::getline(file, line)) {
            program_lines.push_back(line);
        }
        
        // First pass: collect labels
        for (size_t i = 0; i < program_lines.size(); i++) {
            std::string trimmed = trim(program_lines[i]);
            if (trimmed.back() == ':') {
                std::string label = trimmed.substr(0, trimmed.length() - 1);
                labels[label] = i;
            }
        }
    }
    
    void run() {
        pc = 0;
        while (pc < program_lines.size()) {
            execute_instruction(program_lines[pc]);
            pc++;
        }
    }
    
    void print_registers() {
        std::cout << "Registers:" << std::endl;
        for (const auto& reg : registers) {
            if (reg.second != 0) {
                std::cout << reg.first << " = " << reg.second << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <mips_file>" << std::endl;
        return 1;
    }
    
    MipsEmulator emulator;
    emulator.load_program(argv[1]);
    emulator.run();
    
    return 0;
}
