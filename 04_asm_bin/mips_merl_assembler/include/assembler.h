#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include "scanner.h"
#include "symbol_table.h"
#include "merl_entries.h"
#include "code_generator.h"
#include "merl_file.h"

class Assembler {
public:
    // Constructor
    Assembler();
    
    // Main assembly method
    int assemble(const std::vector<std::vector<Token>>& assemblyProgram);
    
    // Debug methods
    void printDebugInfo(const std::vector<std::vector<Token>>& assemblyProgram) const;
    
    // Get the generated MERL file
    const MerlFile& getMerlFile() const { return merlFile_; }
    
    // Get MERL entries for printing
    const MerlEntryGenerator& getEntryGenerator() const { return entryGenerator_; }

private:
    SymbolTableManager symbolManager_;
    MerlEntryGenerator entryGenerator_;
    MerlFile merlFile_;
    
    // Assembly phases
    void firstPass(const std::vector<std::vector<Token>>& assemblyProgram);
    void secondPass(const std::vector<std::vector<Token>>& assemblyProgram);
};

#endif // ASSEMBLER_H
