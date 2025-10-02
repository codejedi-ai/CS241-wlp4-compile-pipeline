#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
using namespace std;

// Helper function to read a 32-bit word from uint32 data
uint32_t read_word(const vector<uint32_t>& data, int& pos) {
    if (pos >= (int)data.size()) {
        cerr << "Error: Unexpected end of file while reading word" << endl;
        exit(1);
    }
    
    uint32_t word = data[pos];
    pos++;
    return word;
}

// Function to load MERL data and return uint32 array
vector<uint32_t> load_merl(const vector<uint32_t>& data, uint32_t loadAddress) {
    if (data.size() < 2) {
        cerr << "Error: Invalid MERL file - too short" << endl;
        exit(1);
    }
    
    int pos = 0;
    
    // Read MERL magic number
    uint32_t magic = read_word(data, pos);
    if (magic != 0x10000002) {
        cerr << "Error: Not a valid MERL file (magic: 0x" << hex << magic << ")" << endl;
        exit(1);
    }
    
    // Read the next word to determine format
    uint32_t nextWord = read_word(data, pos);
    
    // Check if this is the simplified format (codeSize) or full format (endMod)
    bool hasRelocationRecords = false;
    int codeSize, endMod;
    
    // Check if the file ends with MERL end marker
    bool hasEndMarker = (data.size() >= 1 && data[data.size() - 1] == 0x10000001);
    
    if (hasEndMarker) {
        // This is the simplified format: [Magic][CodeSize][Code][EndMarker]
        // Calculate actual code size from file structure
        codeSize = (int)data.size() - 3; // Total - header(2) - endmarker(1)
        endMod = data.size(); // End of file
        hasRelocationRecords = false;
        cerr << "Detected simplified MERL format (no relocation records)" << endl;
        cerr << "File reports codeSize: " << nextWord << ", actual codeSize: " << codeSize << endl;
    } else {
        // This is the full format: [Magic][EndMod][CodeSize][Code][RelocationRecords][EndMarker]
        endMod = nextWord;
        codeSize = read_word(data, pos) - 3; // codeSize = endMod - 3
        hasRelocationRecords = true;
        cerr << "Detected full MERL format with relocation records" << endl;
    }
    
    if (codeSize < 0 || codeSize > (int)data.size() - 3) {
        cerr << "Error: Invalid code size: " << codeSize << endl;
        exit(1);
    }
    
    // α ← findFreeRAM(codeSize) - use load address as base
    uint32_t alpha = loadAddress;
    
    // Allocate memory for code (simulate RAM) - round up to word boundary
    int wordCount = (codeSize + 3) / 4; // Round up to word boundary
    vector<uint32_t> memory(wordCount, 0);
    
    // 1. Load the Code into RAM
    for (int i = 0; i < codeSize; i += 4) {
        uint32_t word = read_word(data, pos);
        memory[i / 4] = word;
    }
    
    // 2. Process Relocation Records (if present)
    if (hasRelocationRecords) {
        int i = codeSize + 3; // Start after code section
        while (i < endMod) {
            if (i + 2 > (int)data.size()) {
                cerr << "Error: Unexpected end of file in relocation section" << endl;
                exit(1);
            }
            
            uint32_t format = read_word(data, pos);
            if (format == 1) {
                uint32_t rel = read_word(data, pos);
                
                // Relocation step: Update an address inside the loaded code
                // MEM[α + rel - 12] is the memory location of the address to be fixed.
                // += (α - 12) is the value added to fix the address.
                
                int relIndex = (rel - 12) / 4;
                if (relIndex >= 0 && relIndex < (int)memory.size()) {
                    memory[relIndex] += alpha - 12;
                    cerr << "Applied relocation: address " << rel << " += " << (alpha - 12) << endl;
                } else {
                    cerr << "Warning: Relocation address out of bounds: " << rel << endl;
                }
            } else {
                cerr << "Error: Unknown relocation format: " << format << endl;
                exit(1);
            }
            
            i += 2; // Advance past the 'format' word and the 'rel' word
        }
    } else {
        cerr << "No relocation records to process" << endl;
    }
    
    cerr << "Loaded " << memory.size() << " words at address 0x" << hex << alpha << dec << endl;
    
    return memory;
}