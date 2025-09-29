#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "scanner.h"

class SymbolTableManager {
public:
    // Constructor
    SymbolTableManager();
    
    // First pass methods
    void processFirstPass(const std::vector<std::vector<Token>>& assemblyProgram);
    
    // Symbol table access
    const std::map<std::string, uint32_t>& getSymbolTable() const { return symbolTable_; }
    const std::map<std::string, uint32_t>& getImportTable() const { return importTable_; }
    const std::map<std::string, uint32_t>& getExportTable() const { return exportTable_; }
    
    // Symbol lookup methods
    bool isLocalSymbol(const std::string& symbol) const;
    bool isImportedSymbol(const std::string& symbol) const;
    bool isExportedSymbol(const std::string& symbol) const;
    uint32_t getSymbolAddress(const std::string& symbol) const;
    
    // Debug output
    void printTables() const;

private:
    std::map<std::string, uint32_t> symbolTable_;  // Local symbols and their addresses
    std::map<std::string, uint32_t> importTable_;  // Imported symbols
    std::map<std::string, uint32_t> exportTable_;  // Exported symbols
    
    // Helper methods
    void processImportDirective(const std::vector<Token>& line);
    void processExportDirective(const std::vector<Token>& line);
    void processLabel(const std::vector<Token>& line, uint32_t& pc);
    void processInstruction(const std::vector<Token>& line, uint32_t& pc);
};

#endif // SYMBOL_TABLE_H
