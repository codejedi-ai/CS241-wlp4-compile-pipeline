#include "code_generator.h"
#include "mips_instructions.h"
#include <iostream>
#include <stdexcept>

CodeGenerator::CodeGenerator(const SymbolTableManager& symbolManager) 
    : symbolManager_(symbolManager) {
}

void CodeGenerator::generateCode(const std::vector<std::vector<Token>>& assemblyProgram) {
    generatedCode_.clear();
    
    for (const auto& line : assemblyProgram) {
        if (line.empty()) continue;
        
        int ind = 0;
        
        // Skip import/export directives
        if (ind < static_cast<int>(line.size()) && 
            (line[ind].getKind() == Token::IMPORT || line[ind].getKind() == Token::EXPORT)) {
            continue;
        }
        
        // Skip labels
        while (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::LABEL) {
            ind++;
        }
        if (ind >= static_cast<int>(line.size())) continue;
        
        processInstruction(line);
    }
}

void CodeGenerator::addInstruction(uint32_t instr) {
    generatedCode_.push_back(instr);
}

void CodeGenerator::processInstruction(const std::vector<Token>& line) {
    try {
        // Use the factory to create the instruction
        auto instruction = InstructionFactory::createInstruction(line, symbolManager_);
        
        // Encode and add the instruction
        uint32_t encoded = instruction->encode();
        addInstruction(encoded);
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Error processing instruction: " + std::string(e.what()));
    }
}
