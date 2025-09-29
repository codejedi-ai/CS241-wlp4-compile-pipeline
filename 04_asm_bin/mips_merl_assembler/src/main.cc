#include <iostream>
#include <vector>
#include <string>
#include "scanner.h"
#include "assembler.h"

int main() {
    std::vector<std::vector<Token>> assemblyProgram;
    
    // Read and tokenize each line individually
    try {
        std::string line;
        while (std::getline(std::cin, line)) {
            assemblyProgram.push_back(scan(line));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    // Assemble the program
    Assembler assembler;
    int result = assembler.assemble(assemblyProgram);
    
    if (result == 0) {
        // Print MERL entries table to stderr
        assembler.getEntryGenerator().printEntries(assemblyProgram);
        
        // Output the MERL file to stdout
        assembler.getMerlFile().outputToStdout();
    }
    
    return result;
}
