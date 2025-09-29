#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include "scanner.h"
#include "symbol_table.h"
#include "mips_instructions.h"

class CodeGenerator {
public:
    // Constructor
    CodeGenerator(const SymbolTableManager& symbolManager);
    
    // Main generation method
    void generateCode(const std::vector<std::vector<Token>>& assemblyProgram);
    
    // Get generated code as vector
    const std::vector<uint32_t>& getGeneratedCode() const { return generatedCode_; }
    
    // Get generated code size
    uint32_t getCodeSize() const { return generatedCode_.size() * sizeof(uint32_t); }

private:
    const SymbolTableManager& symbolManager_;
    std::vector<uint32_t> generatedCode_;
    
    // Helper methods
    void addInstruction(uint32_t instr);
    void processInstruction(const std::vector<Token>& line);
};

#endif // CODE_GENERATOR_H
