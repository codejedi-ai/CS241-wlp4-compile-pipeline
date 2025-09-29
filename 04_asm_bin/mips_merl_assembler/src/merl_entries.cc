#include "merl_entries.h"
#include <iostream>
#include <iomanip>

MerlEntryGenerator::MerlEntryGenerator() {
    // Constructor - initialize empty vectors
}

void MerlEntryGenerator::generateRelEntries(const std::vector<std::vector<Token>>& assemblyProgram, 
                                          const std::map<std::string, uint32_t>& symbolTable,
                                          const std::map<std::string, uint32_t>& importTable) {
    uint32_t pc = 0xC; // Start after header
    
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
        
        if (line[ind].getKind() == Token::WORD) {
            ind++;
            // Add REL entry only for .word directives that reference locally defined symbols
            if (ind < static_cast<int>(line.size()) && 
                (line[ind].getKind() == Token::LABEL || line[ind].getKind() == Token::ID)) {
                std::string symbolName = line[ind].getLexeme();
                // Only create REL entry if the symbol is locally defined AND not imported
                if (symbolTable.count(symbolName) > 0 && importTable.count(symbolName) == 0) {
                    relEntries_.push_back(0x00000001); // REL entry type
                    relEntries_.push_back(pc);         // Address of the word
                }
            }
            pc += 4;
        } else {
            pc += 4;
        }
    }
}

void MerlEntryGenerator::generateEsrEntries(const std::vector<std::vector<Token>>& assemblyProgram,
                                          const std::map<std::string, uint32_t>& importTable) {
    for (const auto& pair : importTable) {
        const std::string& symbol = pair.first;
        uint32_t refAddress = findSymbolReferenceAddress(symbol, assemblyProgram);
        
        if (refAddress != 0) {
            // ESR entry: [0x00000011][address][symbol_length][symbol_name]
            esrEntries_.push_back(0x00000011);           // ESR entry type
            esrEntries_.push_back(refAddress);           // Address where symbol is referenced
            esrEntries_.push_back(symbol.length());     // Symbol length
            // Add symbol name as individual characters
            for (char c : symbol) {
                esrEntries_.push_back(static_cast<uint32_t>(c));
            }
        }
    }
}

void MerlEntryGenerator::generateEsdEntries(const std::map<std::string, uint32_t>& exportTable,
                                           const std::map<std::string, uint32_t>& symbolTable) {
    for (const auto& pair : exportTable) {
        const std::string& symbol = pair.first;
        if (symbolTable.count(symbol)) {
            // ESD entry: [0x00000005][address][symbol_length][symbol_name]
            esdEntries_.push_back(0x00000005);           // ESD entry type
            esdEntries_.push_back(symbolTable.at(symbol)); // Address of symbol definition
            esdEntries_.push_back(symbol.length());     // Symbol length
            // Add symbol name as individual characters
            for (char c : symbol) {
                esdEntries_.push_back(static_cast<uint32_t>(c));
            }
        }
    }
}

uint32_t MerlEntryGenerator::findSymbolReferenceAddress(const std::string& symbol, 
                                                       const std::vector<std::vector<Token>>& assemblyProgram) const {
    uint32_t currentPc = 0xC; // Start after header
    
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
        
        if (line[ind].getKind() == Token::WORD) {
            ind++;
            if (ind < static_cast<int>(line.size()) && line[ind].getKind() == Token::ID && 
                line[ind].getLexeme() == symbol) {
                return currentPc;
            }
            currentPc += 4;
        } else {
            currentPc += 4;
        }
    }
    
    return 0; // Symbol not found
}

void MerlEntryGenerator::printEntries() const {
    std::cerr << "REL Entries:" << std::endl;
    for (size_t i = 0; i < relEntries_.size(); i += 2) {
        std::cerr << "  REL: 0x" << std::hex << relEntries_[i] 
                  << " Address: 0x" << relEntries_[i+1] << std::dec << std::endl;
    }
    
    std::cerr << "ESR Entries:" << std::endl;
    for (size_t i = 0; i < esrEntries_.size(); ) {
        uint32_t type = esrEntries_[i++];
        uint32_t address = esrEntries_[i++];
        uint32_t length = esrEntries_[i++];
        std::cerr << "  ESR: 0x" << std::hex << type << " Address: 0x" << address 
                  << " Length: " << std::dec << length << " Name: ";
        for (uint32_t j = 0; j < length; j++) {
            std::cerr << static_cast<char>(esrEntries_[i++]);
        }
        std::cerr << std::endl;
    }
    
    std::cerr << "ESD Entries:" << std::endl;
    for (size_t i = 0; i < esdEntries_.size(); ) {
        uint32_t type = esdEntries_[i++];
        uint32_t address = esdEntries_[i++];
        uint32_t length = esdEntries_[i++];
        std::cerr << "  ESD: 0x" << std::hex << type << " Address: 0x" << address 
                  << " Length: " << std::dec << length << " Name: ";
        for (uint32_t j = 0; j < length; j++) {
            std::cerr << static_cast<char>(esdEntries_[i++]);
        }
        std::cerr << std::endl;
    }
}

void MerlEntryGenerator::printEntries(const std::vector<std::vector<Token>>& assemblyProgram) const {
    // Print REL entries table
    std::cerr << "REL Entries:" << std::endl;
    std::cerr << "Address    Length    Name" << std::endl;
    std::cerr << "------------------------" << std::endl;
    for (size_t i = 0; i < relEntries_.size(); i += 2) {
        uint32_t address = relEntries_[i+1];
        std::string labelName = findLabelAtAddress(address, assemblyProgram);
        std::cerr << "0x" << std::setfill('0') << std::setw(8) << std::hex << address 
                  << "   " << std::setfill(' ') << std::setw(6) << std::dec << labelName.length()
                  << "    " << labelName << std::endl;
    }
    if (relEntries_.empty()) {
        std::cerr << "(no REL entries)" << std::endl;
    }
    std::cerr << std::endl;
    
    // Print ESR entries table
    std::cerr << "ESR Entries:" << std::endl;
    std::cerr << "Address    Length    Name" << std::endl;
    std::cerr << "------------------------" << std::endl;
    for (size_t i = 0; i < esrEntries_.size(); ) {
        esrEntries_[i++]; // Skip type
        uint32_t address = esrEntries_[i++];
        uint32_t length = esrEntries_[i++];
        std::string name;
        for (uint32_t j = 0; j < length; j++) {
            name += static_cast<char>(esrEntries_[i++]);
        }
        std::cerr << "0x" << std::setfill('0') << std::setw(8) << std::hex << address 
                  << "   " << std::setfill(' ') << std::setw(6) << std::dec << length
                  << "    " << name << std::endl;
    }
    if (esrEntries_.empty()) {
        std::cerr << "(no ESR entries)" << std::endl;
    }
    std::cerr << std::endl;
    
    // Print ESD entries table
    std::cerr << "ESD Entries:" << std::endl;
    std::cerr << "Address    Length    Name" << std::endl;
    std::cerr << "------------------------" << std::endl;
    for (size_t i = 0; i < esdEntries_.size(); ) {
        esdEntries_[i++]; // Skip type
        uint32_t address = esdEntries_[i++];
        uint32_t length = esdEntries_[i++];
        std::string name;
        for (uint32_t j = 0; j < length; j++) {
            name += static_cast<char>(esdEntries_[i++]);
        }
        std::cerr << "0x" << std::setfill('0') << std::setw(8) << std::hex << address 
                  << "   " << std::setfill(' ') << std::setw(6) << std::dec << length
                  << "    " << name << std::endl;
    }
    if (esdEntries_.empty()) {
        std::cerr << "(no ESD entries)" << std::endl;
    }
}

void MerlEntryGenerator::writeEntries() const {
    // This method is no longer needed since entries are handled by MerlFile
    // The entries are now returned as vectors and written by MerlFile::writeRelocationTable
}

uint32_t MerlEntryGenerator::getRelocTableSize() const {
    return (relEntries_.size() + esrEntries_.size() + esdEntries_.size()) * sizeof(uint32_t);
}

std::string MerlEntryGenerator::findLabelAtAddress(uint32_t address, 
                                                   const std::vector<std::vector<Token>>& assemblyProgram) const {
    uint32_t currentPc = 0xC; // Start after header
    
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
        
        if (line[ind].getKind() == Token::WORD) {
            ind++;
            if (ind < static_cast<int>(line.size()) && 
                (line[ind].getKind() == Token::LABEL || line[ind].getKind() == Token::ID) && 
                currentPc == address) {
                return line[ind].getLexeme();
            }
            currentPc += 4;
        } else {
            currentPc += 4;
        }
    }
    
    return "unknown"; // Label not found
}
