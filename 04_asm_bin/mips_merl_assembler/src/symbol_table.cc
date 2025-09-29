#include "symbol_table.h"
#include <iostream>
#include <iomanip>

SymbolTableManager::SymbolTableManager() {
    // Constructor - initialize empty maps
}

void SymbolTableManager::processFirstPass(const std::vector<std::vector<Token>>& assemblyProgram) {
    uint32_t pc = 0xC; // Start at 0xC because we need to consider the header
    
    for (const auto& line : assemblyProgram) {
        if (line.empty()) continue;
        
        int ind = 0;
        
        // Process import directives
        if (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::IMPORT) {
            processImportDirective(line);
            continue;
        }
        
        // Process export directives
        if (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::EXPORT) {
            processExportDirective(line);
            continue;
        }
        
        // Process labels
        processLabel(line, pc);
        
        // Process instructions
        processInstruction(line, pc);
    }
    
    // Update export table with actual addresses for locally defined symbols
    for (auto& pair : exportTable_) {
        const std::string& symbol = pair.first;
        if (symbolTable_.count(symbol)) {
            exportTable_[symbol] = symbolTable_[symbol];
        }
    }
}

void SymbolTableManager::processImportDirective(const std::vector<Token>& line) {
    for (size_t i = 1; i < line.size(); i++) {
        if (line[i].getKind() == Token::LABEL) {
            std::string symbolName = line[i].getLexeme();
            // Remove the colon from the label name
            if (!symbolName.empty() && symbolName.back() == ':') {
                symbolName.pop_back();
            }
            importTable_[symbolName] = 0; // Imported symbols have address 0 initially
        }
    }
}

void SymbolTableManager::processExportDirective(const std::vector<Token>& line) {
    for (size_t i = 1; i < line.size(); i++) {
        if (line[i].getKind() == Token::LABEL) {
            std::string symbolName = line[i].getLexeme();
            // Remove the colon from the label name
            if (!symbolName.empty() && symbolName.back() == ':') {
                symbolName.pop_back();
            }
            exportTable_[symbolName] = 0; // Will be updated when symbol is defined
        }
    }
}

void SymbolTableManager::processLabel(const std::vector<Token>& line, uint32_t& pc) {
    int ind = 0;
    
    // Process labels
    while (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::LABEL) {
        std::string labelName = line[ind].getLexeme();
        // Remove the colon from the label name
        if (!labelName.empty() && labelName.back() == ':') {
            labelName.pop_back();
        }
        symbolTable_[labelName] = pc;
        ind++;
    }
}

void SymbolTableManager::processInstruction(const std::vector<Token>& line, uint32_t& pc) {
    int ind = 0;
    
    // Skip labels
    while (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::LABEL) {
        ind++;
    }
    if (ind >= static_cast<int>(line.size())) return;
    
    // Process instruction
    if (line[ind].getKind() == Token::WORD) {
        pc += 4;
    } else {
        pc += 4; // All other instructions are 4 bytes
    }
}

bool SymbolTableManager::isLocalSymbol(const std::string& symbol) const {
    return symbolTable_.count(symbol) > 0;
}

bool SymbolTableManager::isImportedSymbol(const std::string& symbol) const {
    return importTable_.count(symbol) > 0;
}

bool SymbolTableManager::isExportedSymbol(const std::string& symbol) const {
    return exportTable_.count(symbol) > 0;
}

uint32_t SymbolTableManager::getSymbolAddress(const std::string& symbol) const {
    if (symbolTable_.count(symbol)) {
        return symbolTable_.at(symbol);
    }
    if (importTable_.count(symbol)) {
        return 0; // Imported symbols have placeholder address
    }
    return 0; // Symbol not found
}

void SymbolTableManager::printTables() const {
    // Symbol table output removed - only MERL entries table should be shown
}
