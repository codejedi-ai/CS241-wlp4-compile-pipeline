#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <address>" << endl;
        return 1;
    }
    
    int loadAddress = atoi(argv[1]);
    
    // Read MERL file from stdin
    vector<char> data;
    char byte;
    while (cin.get(byte)) {
        data.push_back(byte);
    }
    
    // Simple MERL processor - strips metadata and outputs MIPS binary
    // This is a simplified version
    
    if (data.size() < 8) {
        cerr << "Error: Invalid MERL file" << endl;
        return 1;
    }
    
    // Check for MERL magic number
    int magic = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    if (magic != 0x10000002) {
        cerr << "Error: Not a valid MERL file" << endl;
        return 1;
    }
    
    // Get code size
    int codeSize = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
    
    // Output the code section
    for (int i = 8; i < 8 + codeSize && i < data.size(); i++) {
        cout << data[i];
    }
    
    return 0;
}

