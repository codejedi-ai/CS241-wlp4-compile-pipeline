#ifndef MERL_ENTRIES_H
#define MERL_ENTRIES_H

#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include "scanner.h"

class MerlEntryGenerator {
public:
    // Constructor
    MerlEntryGenerator();
    
    // Entry generation methods
    void generateRelEntries(const std::vector<std::vector<Token>>& assemblyProgram, 
                           const std::map<std::string, uint32_t>& symbolTable,
                           const std::map<std::string, uint32_t>& importTable);
    
    void generateEsrEntries(const std::vector<std::vector<Token>>& assemblyProgram,
                           const std::map<std::string, uint32_t>& importTable);
    
    void generateEsdEntries(const std::map<std::string, uint32_t>& exportTable,
                           const std::map<std::string, uint32_t>& symbolTable);
    
    // Output methods
    void printEntries() const;
    void printEntries(const std::vector<std::vector<Token>>& assemblyProgram) const;
    void writeEntries() const;
    
    // Getters
    const std::vector<uint32_t>& getRelEntries() const { return relEntries_; }
    const std::vector<uint32_t>& getEsrEntries() const { return esrEntries_; }
    const std::vector<uint32_t>& getEsdEntries() const { return esdEntries_; }
    
    // Calculate total relocation table size
    uint32_t getRelocTableSize() const;

private:
    std::vector<uint32_t> relEntries_;  // REL entries (0x00000001)
    std::vector<uint32_t> esrEntries_; // ESR entries (0x00000011)
    std::vector<uint32_t> esdEntries_; // ESD entries (0x00000005)
    
    // Helper methods
    uint32_t findSymbolReferenceAddress(const std::string& symbol, 
                                       const std::vector<std::vector<Token>>& assemblyProgram) const;
    std::string findLabelAtAddress(uint32_t address, 
                                   const std::vector<std::vector<Token>>& assemblyProgram) const;
};

#endif // MERL_ENTRIES_H
