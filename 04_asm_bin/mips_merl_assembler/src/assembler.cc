#include "assembler.h"
#include <iostream>

Assembler::Assembler() {
    // Constructor - initialize components
}

int Assembler::assemble(const std::vector<std::vector<Token>>& assemblyProgram) {
    // First pass: Build symbol tables
    firstPass(assemblyProgram);
    
    // Second pass: Generate MERL binary
    secondPass(assemblyProgram);
    
    return 0;
}

void Assembler::firstPass(const std::vector<std::vector<Token>>& assemblyProgram) {
    // Process symbol tables
    symbolManager_.processFirstPass(assemblyProgram);
    
    // Generate MERL entries
    entryGenerator_.generateRelEntries(assemblyProgram, 
                                     symbolManager_.getSymbolTable(), 
                                     symbolManager_.getImportTable());
    entryGenerator_.generateEsrEntries(assemblyProgram, 
                                      symbolManager_.getImportTable());
    entryGenerator_.generateEsdEntries(symbolManager_.getExportTable(), 
                                      symbolManager_.getSymbolTable());
    
    // Debug printing is now handled in main()
}

void Assembler::secondPass(const std::vector<std::vector<Token>>& assemblyProgram) {
    // Generate code first to calculate sizes
    CodeGenerator codeGen(symbolManager_);
    codeGen.generateCode(assemblyProgram);
    uint32_t codeSize = codeGen.getCodeSize();
    uint32_t relocTableSize = entryGenerator_.getRelocTableSize();
    
    // Build MERL file
    merlFile_.writeHeader(codeSize, relocTableSize);
    merlFile_.writeCode(codeGen.getGeneratedCode());
    merlFile_.writeRelocationTable(entryGenerator_.getRelEntries(),
                                  entryGenerator_.getEsrEntries(),
                                  entryGenerator_.getEsdEntries());
    merlFile_.writeEnd();
}

void Assembler::printDebugInfo(const std::vector<std::vector<Token>>& assemblyProgram) const {
    entryGenerator_.printEntries(assemblyProgram);
}
