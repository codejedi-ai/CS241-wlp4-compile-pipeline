#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct MERLFile {
    vector<char> data;
    int codeSize;
    int totalSize;
};

MERLFile readMERLFile(const string& filename) {
    MERLFile file;
    ifstream in(filename, ios::binary);
    
    if (!in) {
        cerr << "Error: Cannot open " << filename << endl;
        exit(1);
    }
    
    // Read entire file
    char byte;
    while (in.get(byte)) {
        file.data.push_back(byte);
    }
    
    // Parse MERL header to get code size
    if (file.data.size() >= 8) {
        file.codeSize = (file.data[4] << 24) | (file.data[5] << 16) | 
                       (file.data[6] << 8) | file.data[7];
    }
    
    file.totalSize = file.data.size();
    return file;
}

void writeLinkedMERL(const vector<MERLFile>& files) {
    // Write combined MERL file
    // This is a simplified version - real linker would handle relocations
    
    for (const auto& file : files) {
        // Write each file's data
        for (char byte : file.data) {
            cout << byte;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " file1.merl file2.merl ..." << endl;
        return 1;
    }
    
    vector<MERLFile> files;
    for (int i = 1; i < argc; i++) {
        files.push_back(readMERLFile(argv[i]));
    }
    
    writeLinkedMERL(files);
    return 0;
}

