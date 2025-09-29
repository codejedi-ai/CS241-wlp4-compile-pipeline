#ifndef MERL_FILE_H
#define MERL_FILE_H

#include <vector>
#include <cstdint>
#include <string>
#include <cstddef>

// Forward declaration
struct MerlHeader;

// MERL constants
const uint32_t MERL_MAGIC = 0x10000002;
const uint32_t MERL_END = 0x10000001;

// MERL header structure
struct MerlHeader {
    uint32_t cookie;        // 0x10000002 (MERL magic number)
    uint32_t endOfModule;   // End of module offset
    uint32_t endOfCode;     // End of code offset
};

class MerlFile {
public:
    // Constructor
    MerlFile();
    
    // MERL file building methods
    void writeHeader(uint32_t codeSize, uint32_t relocTableSize);
    void writeCode(const std::vector<uint32_t>& code);
    void writeRelocationTable(const std::vector<uint32_t>& relEntries,
                             const std::vector<uint32_t>& esrEntries,
                             const std::vector<uint32_t>& esdEntries);
    void writeEnd();
    
    // Output methods
    void outputToStdout() const;
    void outputToFile(const std::string& filename) const;
    
    // Getters
    const std::vector<uint32_t>& getData() const { return data_; }
    size_t getSize() const { return data_.size(); }
    
    // Debug methods
    void printHexDump() const;
    void printInfo() const;

private:
    std::vector<uint32_t> data_;
    
    // Helper methods
    void appendWord(uint32_t word);
    void appendWords(const std::vector<uint32_t>& words);
    void outputWord(uint32_t word) const;
};

#endif // MERL_FILE_H
