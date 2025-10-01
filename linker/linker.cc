#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

using namespace std;

struct MERLFile {
    vector<uint32_t> header;
    vector<uint32_t> code;
    vector<uint32_t> table;
    int codeSize = 0;
    int tableSize = 0;
};

// Read a 32-bit word from a byte stream
uint32_t readWord(const vector<char>& data, size_t& pos) {
    uint32_t word = 0;
    word |= (data[pos++] << 24);
    word |= (data[pos++] << 16);
    word |= (data[pos++] << 8);
    word |= data[pos++];
    return word;
}

MERLFile parseMERLFile(const string& filename) {
    MERLFile file;
    ifstream in(filename, ios::binary);
    
    if (!in) {
        cerr << "Error: Cannot open " << filename << endl;
        exit(1);
    }
    
    // Read entire file
    vector<char> fileData((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    
    // Parse MERL header (first 3 words)
    size_t pos = 0;
    for (int i = 0; i < 3; ++i) {
        file.header.push_back(readWord(fileData, pos));
    }
    
    // Extract code size and table information
    int endCode = file.header[2];
    int endModule = file.header[1];
    
    // Read code segment
    while (pos < endCode) {
        file.code.push_back(readWord(fileData, pos));
    }
    file.codeSize = file.code.size();
    
    // Read table segment
    while (pos < endModule) {
        file.table.push_back(readWord(fileData, pos));
    }
    file.tableSize = file.table.size();
    
    return file;
}

// Linking algorithm implementation
MERLFile linkMERLFiles(const MERLFile& m1, const MERLFile& m2) {
    MERLFile linked;
    
    // Step 1: Check for duplicate exports
    for (size_t i = 0; i < m1.table.size(); i += 5) {
        if (m1.table[i] == 0x05) { // ESD entry
            for (size_t j = 0; j < m2.table.size(); j += 5) {
                if (m2.table[j] == 0x05 && 
                    equal(m1.table.begin()+i+1, m1.table.begin()+i+5, 
                          m2.table.begin()+j+1)) {
                    cerr << "Error: Duplicate export" << endl;
                    exit(1);
                }
            }
        }
    }
    
    // Step 2: Combine code segments
    linked.code = m1.code;
    linked.code.insert(linked.code.end(), m2.code.begin(), m2.code.end());
    
    // Step 3: Relocate m2's table entries
    int relocOffset = m1.code.size() * 4;
    vector<uint32_t> m2TableModified = m2.table;
    for (size_t i = 1; i < m2TableModified.size(); i += 5) {
        m2TableModified[i] += relocOffset;
    }
    
    // Step 4: Relocate m2's code using REL entries
    for (size_t i = 0; i < m2TableModified.size(); i += 5) {
        if (m2TableModified[i] == 0x01) { // REL entry
            int index = (m2TableModified[i+1] - 12) / 4;
            linked.code[m1.code.size() + index] += relocOffset;
        }
    }
    
    // Step 5: Resolve imports for m1
    auto linkedCodeModified = linked.code;
    auto m1TableModified = m1.table;
    for (size_t i = 0; i < m1.table.size(); i += 5) {
        if (m1.table[i] == 0x11) { // ESR entry
            for (size_t j = 0; j < m2.table.size(); j += 5) {
                if (m2.table[j] == 0x05 && // ESD entry
                    equal(m1.table.begin()+i+2, m1.table.begin()+i+6, 
                          m2.table.begin()+j+2)) {
                    // Resolve import
                    int index = (m1.table[i+1] - 12) / 4;
                    linkedCodeModified[index] = m2.table[j+1];
                    
                    // Change ESR to REL
                    m1TableModified[i] = 0x01;
                    m1TableModified[i+1] = m1.table[i+1];
                    break;
                }
            }
        }
    }
    
    // Step 6: Resolve imports for m2
    for (size_t i = 0; i < m2TableModified.size(); i += 5) {
        if (m2TableModified[i] == 0x11) { // ESR entry
            for (size_t j = 0; j < m1.table.size(); j += 5) {
                if (m1.table[j] == 0x05 && // ESD entry
                    equal(m2TableModified.begin()+i+2, m2TableModified.begin()+i+6, 
                          m1.table.begin()+j+2)) {
                    // Resolve import
                    int index = (m2TableModified[i+1] - 12) / 4 + m1.code.size();
                    linkedCodeModified[index] = m1.table[j+1];
                    
                    // Change ESR to REL
                    m2TableModified[i] = 0x01;
                    m2TableModified[i+1] = m2.table[i+1] + relocOffset;
                    break;
                }
            }
        }
    }
    
    // Step 7: Combine tables
    linked.table = m1TableModified;
    linked.table.insert(linked.table.end(), m2TableModified.begin(), m2TableModified.end());
    
    // Step 8: Compute header information
    uint32_t endCode = 12 + linked.code.size() * 4;
    uint32_t endModule = endCode + linked.table.size() * 4;
    
    // Construct new header
    linked.header = {
        0x10000002,  // MERL Cookie (beq $0, $0, 2)
        endModule,   // Module end address
        endCode      // Code end address
    };
    
    return linked;
}

void writeMERLFile(const MERLFile& file, ostream& out) {
    // Write header
    for (uint32_t word : file.header) {
        out.put((word >> 24) & 0xFF);
        out.put((word >> 16) & 0xFF);
        out.put((word >> 8) & 0xFF);
        out.put(word & 0xFF);
    }
    
    // Write code segment
    for (uint32_t word : file.code) {
        out.put((word >> 24) & 0xFF);
        out.put((word >> 16) & 0xFF);
        out.put((word >> 8) & 0xFF);
        out.put(word & 0xFF);
    }
    
    // Write table segment
    for (uint32_t word : file.table) {
        out.put((word >> 24) & 0xFF);
        out.put((word >> 16) & 0xFF);
        out.put((word >> 8) & 0xFF);
        out.put(word & 0xFF);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " file1.merl file2.merl" << endl;
        return 1;
    }
    
    // Read MERL files
    MERLFile m1 = parseMERLFile(argv[1]);
    MERLFile m2 = parseMERLFile(argv[2]);
    
    // Link files
    MERLFile linked = linkMERLFiles(m1, m2);
    
    // Write linked MERL file to stdout
    writeMERLFile(linked, cout);
    
    return 0;
}