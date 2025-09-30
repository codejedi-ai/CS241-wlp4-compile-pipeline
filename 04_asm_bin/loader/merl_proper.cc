#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

// Helper function to read a 32-bit word from data
int read_word(const vector<char>& data, int& pos) {
    if (pos + 4 > (int)data.size()) {
        cerr << "Error: Unexpected end of file while reading word" << endl;
        exit(1);
    }
    
    int word = (data[pos] << 24) | (data[pos + 1] << 16) | 
               (data[pos + 2] << 8) | data[pos + 3];
    pos += 4;
    return word;
}

// Helper function to write a 32-bit word to output
void write_word(int word) {
    cout << (char)(word >> 24);
    cout << (char)(word >> 16);
    cout << (char)(word >> 8);
    cout << (char)(word);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <load_address>" << endl;
        return 1;
    }
    
    int loadAddress = atoi(argv[1]);
    
    // Read MERL file from stdin
    vector<char> data;
    char byte;
    while (cin.get(byte)) {
        data.push_back(byte);
    }
    
    if (data.size() < 8) {
        cerr << "Error: Invalid MERL file - too short" << endl;
        return 1;
    }
    
    int pos = 0;
    
    // Read MERL magic number
    int magic = read_word(data, pos);
    if (magic != 0x10000002) {
        cerr << "Error: Not a valid MERL file (magic: 0x" << hex << magic << ")" << endl;
        return 1;
    }
    
    // Read the next word to determine format
    int nextWord = read_word(data, pos);
    
    // Check if this is the simplified format (codeSize) or full format (endMod)
    bool hasRelocationRecords = false;
    int codeSize, endMod;
    
    // Check if the file ends with MERL end marker
    bool hasEndMarker = (data.size() >= 4 && 
                        data[data.size() - 4] == 0x10 && data[data.size() - 3] == 0x00 && 
                        data[data.size() - 2] == 0x00 && data[data.size() - 1] == 0x01);
    
    if (hasEndMarker) {
        // This is the simplified format: [Magic][CodeSize][Code][EndMarker]
        // Calculate actual code size from file structure
        codeSize = (int)data.size() - 12; // Total - header(8) - endmarker(4)
        endMod = data.size(); // End of file
        hasRelocationRecords = false;
        cerr << "Detected simplified MERL format (no relocation records)" << endl;
        cerr << "File reports codeSize: " << nextWord << ", actual codeSize: " << codeSize << endl;
    } else {
        // This is the full format: [Magic][EndMod][CodeSize][Code][RelocationRecords][EndMarker]
        endMod = nextWord;
        codeSize = read_word(data, pos) - 12; // codeSize = endMod - 12
        hasRelocationRecords = true;
        cerr << "Detected full MERL format with relocation records" << endl;
    }
    
    if (codeSize < 0 || codeSize > (int)data.size() - 12) {
        cerr << "Error: Invalid code size: " << codeSize << endl;
        return 1;
    }
    
    // α ← findFreeRAM(codeSize) - use load address as base
    int alpha = loadAddress;
    
    // Allocate memory for code (simulate RAM) - round up to word boundary
    int wordCount = (codeSize + 3) / 4; // Round up to word boundary
    vector<int> memory(wordCount, 0);
    
    // 1. Load the Code into RAM
    for (int i = 0; i < codeSize; i += 4) {
        int word = read_word(data, pos);
        memory[i / 4] = word;
    }
    
    // 2. Process Relocation Records (if present)
    if (hasRelocationRecords) {
        int i = codeSize + 12; // Start after code section
        while (i < endMod) {
            if (i + 8 > (int)data.size()) {
                cerr << "Error: Unexpected end of file in relocation section" << endl;
                return 1;
            }
            
            int format = read_word(data, pos);
            if (format == 1) {
                int rel = read_word(data, pos);
                
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
                return 1;
            }
            
            i += 8; // Advance past the 'format' word (4 bytes) and the 'rel' word (4 bytes)
        }
    } else {
        cerr << "No relocation records to process" << endl;
    }
    
    // Output the relocated code
    for (int word : memory) {
        write_word(word);
    }
    
    cerr << "Loaded " << memory.size() << " words at address 0x" << hex << alpha << dec << endl;
    
    return 0;
}