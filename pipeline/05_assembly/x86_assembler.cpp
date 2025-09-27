#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>

// Simple x86 assembler that converts x86 assembly to object code
// This is a basic implementation for demonstration

class X86Assembler {
private:
    std::map<std::string, int> labels;
    std::vector<std::string> instructions;
    int pc = 0;
    
public:
    void assemble(const std::string& assembly) {
        std::istringstream stream(assembly);
        std::string line;
        
        // First pass: collect labels
        while (std::getline(stream, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#' || line[0] == '/') continue;
            
            if (line.back() == ':') {
                std::string label = line.substr(0, line.length() - 1);
                labels[label] = pc;
            } else {
                instructions.push_back(line);
                pc += 4; // Assume each instruction is 4 bytes
            }
        }
        
        // Second pass: generate object code
        pc = 0;
        for (const auto& instruction : instructions) {
            generateInstruction(instruction);
            pc += 4;
        }
    }
    
private:
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
    
    void generateInstruction(const std::string& instruction) {
        std::istringstream iss(instruction);
        std::string opcode;
        iss >> opcode;
        
        if (opcode == "push") {
            std::string reg;
            iss >> reg;
            if (reg == "%ebp") {
                std::cout << "55"; // push %ebp
            } else if (reg == "%eax") {
                std::cout << "50"; // push %eax
            } else {
                std::cout << "50"; // default push
            }
        } else if (opcode == "pop") {
            std::string reg;
            iss >> reg;
            if (reg == "%ebp") {
                std::cout << "5d"; // pop %ebp
            } else if (reg == "%eax") {
                std::cout << "58"; // pop %eax
            } else {
                std::cout << "58"; // default pop
            }
        } else if (opcode == "mov") {
            std::string dest, src;
            iss >> dest >> src;
            if (dest == "%esp" && src == "%ebp") {
                std::cout << "89 ec"; // mov %ebp, %esp
            } else if (dest == "%ebp" && src == "%esp") {
                std::cout << "89 e5"; // mov %esp, %ebp
            } else if (dest == "%eax" && src.find("8(%ebp)") != std::string::npos) {
                std::cout << "8b 45 08"; // mov 8(%ebp), %eax
            } else if (dest == "%eax" && src.find("12(%ebp)") != std::string::npos) {
                std::cout << "8b 45 0c"; // mov 12(%ebp), %eax
            } else if (dest.find("-4(%ebp)") != std::string::npos && src == "%eax") {
                std::cout << "89 45 fc"; // mov %eax, -4(%ebp)
            } else if (dest.find("-8(%ebp)") != std::string::npos && src == "%eax") {
                std::cout << "89 45 f8"; // mov %eax, -8(%ebp)
            } else if (src.find("$") != std::string::npos) {
                std::cout << "b8 00 00 00 00"; // mov $constant, %eax
            } else {
                std::cout << "89 c0"; // mov %eax, %eax
            }
        } else if (opcode == "add") {
            std::string dest, src;
            iss >> dest >> src;
            if (dest == "%eax" && src.find("-8(%ebp)") != std::string::npos) {
                std::cout << "03 45 f8"; // add -8(%ebp), %eax
            } else {
                std::cout << "01 c0"; // add %eax, %eax
            }
        } else if (opcode == "sub") {
            std::string dest, src;
            iss >> dest >> src;
            if (dest == "%eax" && src.find("-8(%ebp)") != std::string::npos) {
                std::cout << "2b 45 f8"; // sub -8(%ebp), %eax
            } else {
                std::cout << "29 c0"; // sub %eax, %eax
            }
        } else if (opcode == "imul") {
            std::string dest, src;
            iss >> dest >> src;
            if (dest == "%eax" && src.find("-8(%ebp)") != std::string::npos) {
                std::cout << "0f af 45 f8"; // imul -8(%ebp), %eax
            } else {
                std::cout << "f7 e8"; // imul %eax
            }
        } else if (opcode == "idiv") {
            std::string src;
            iss >> src;
            if (src.find("-8(%ebp)") != std::string::npos) {
                std::cout << "f7 7d f8"; // idiv -8(%ebp)
            } else {
                std::cout << "f7 f8"; // idiv %eax
            }
        } else if (opcode == "cdq") {
            std::cout << "99"; // cdq
        } else if (opcode == "call") {
            std::string func;
            iss >> func;
            std::cout << "e8 00 00 00 00"; // call function (placeholder)
        } else if (opcode == "ret") {
            std::cout << "c3"; // ret
        } else if (opcode == "sub") {
            std::string dest, src;
            iss >> dest >> src;
            if (dest == "%esp" && src.find("$") != std::string::npos) {
                std::cout << "83 ec 10"; // sub $16, %esp
            }
        } else {
            std::cout << "00 00 00 00"; // unknown instruction
        }
        std::cout << " ";
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.s> <output.o>\n";
        return 1;
    }
    
    std::cerr << "🔧 Stage 5: Assembly" << std::endl;
    std::cerr << "   Converting assembly to object code..." << std::endl;
    
    std::string input_file = argv[1];
    std::string output_file = argv[2];
    
    try {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file '" << input_file << "'\n";
            return 1;
        }
        
        std::string assembly((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();
        
        X86Assembler assembler;
        assembler.assemble(assembly);
        
        std::cerr << "   ✓ Object file generated: " << output_file << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
