#include "merl_file.h"
#include <iostream>
#include <fstream>
#include <iomanip>

MerlFile::MerlFile() {
    data_.clear();
}

void MerlFile::writeHeader(uint32_t codeSize, uint32_t relocTableSize) {
    uint32_t endOfCode = sizeof(MerlHeader) + codeSize;
    uint32_t endOfModule = endOfCode + relocTableSize + sizeof(uint32_t); // +4 for MERL_END
    
    // Write MERL header
    MerlHeader header;
    header.cookie = MERL_MAGIC;
    header.endOfModule = endOfModule;
    header.endOfCode = endOfCode;
    
    // Append header words
    appendWord(header.cookie);
    appendWord(header.endOfModule);
    appendWord(header.endOfCode);
}

void MerlFile::writeCode(const std::vector<uint32_t>& code) {
    appendWords(code);
}

void MerlFile::writeRelocationTable(const std::vector<uint32_t>& relEntries,
                                   const std::vector<uint32_t>& esrEntries,
                                   const std::vector<uint32_t>& esdEntries) {
    // Write REL entries
    appendWords(relEntries);
    
    // Write ESR entries  
    appendWords(esrEntries);
    
    // Write ESD entries
    appendWords(esdEntries);
}

void MerlFile::writeEnd() {
    appendWord(MERL_END);
}

void MerlFile::outputToStdout() const {
    for (uint32_t word : data_) {
        outputWord(word);
    }
}

void MerlFile::outputToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    for (uint32_t word : data_) {
        // Write in big-endian format
        unsigned char c = word >> 24;
        file << c;
        c = word >> 16;
        file << c;
        c = word >> 8;
        file << c;
        c = word;
        file << c;
    }
    
    file.close();
}

void MerlFile::printHexDump() const {
    std::cout << "MERL File Hex Dump:" << std::endl;
    for (size_t i = 0; i < data_.size(); i++) {
        if (i % 4 == 0) {
            std::cout << std::setfill('0') << std::setw(8) << std::hex << (i * 4) << ": ";
        }
        std::cout << std::setfill('0') << std::setw(8) << std::hex << data_[i] << " ";
        if ((i + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }
    if (data_.size() % 4 != 0) {
        std::cout << std::endl;
    }
    std::cout << std::dec;
}

void MerlFile::printInfo() const {
    std::cout << "MERL File Info:" << std::endl;
    std::cout << "  Total size: " << data_.size() * sizeof(uint32_t) << " bytes" << std::endl;
    std::cout << "  Word count: " << data_.size() << std::endl;
    
    if (data_.size() >= 3) {
        std::cout << "  Magic number: 0x" << std::hex << data_[0] << std::dec << std::endl;
        std::cout << "  End of module: 0x" << std::hex << data_[1] << std::dec << std::endl;
        std::cout << "  End of code: 0x" << std::hex << data_[2] << std::dec << std::endl;
    }
}

void MerlFile::appendWord(uint32_t word) {
    data_.push_back(word);
}

void MerlFile::appendWords(const std::vector<uint32_t>& words) {
    data_.insert(data_.end(), words.begin(), words.end());
}

void MerlFile::outputWord(uint32_t word) const {
    unsigned char c = word >> 24;
    std::cout << c;
    c = word >> 16;
    std::cout << c;
    c = word >> 8;
    std::cout << c;
    c = word;
    std::cout << c;
}
