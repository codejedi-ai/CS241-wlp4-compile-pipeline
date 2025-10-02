#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "merl.h"
using namespace std;

// Function to read MERL file and convert to uint32 array
vector<uint32_t> read_merl_file(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Cannot open MERL file " << filename << endl;
        exit(1);
    }
    
    // Read entire file into vector<char> first
    vector<char> rawData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    // Convert to vector<uint32_t> (assuming big-endian)
    vector<uint32_t> data;
    for (size_t i = 0; i < rawData.size(); i += 4) {
        if (i + 3 < rawData.size()) {
            uint32_t word = (rawData[i] << 24) | (rawData[i + 1] << 16) | 
                           (rawData[i + 2] << 8) | rawData[i + 3];
            data.push_back(word);
        }
    }
    
    return data;
}

// Function to output uint32 array as binary
void output_binary(const vector<uint32_t>& memory) {
    for (uint32_t word : memory) {
        cout << (char)(word >> 24);
        cout << (char)(word >> 16);
        cout << (char)(word >> 8);
        cout << (char)(word);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <merl_file> <load_address>" << endl;
        return 1;
    }
    
    string merlFile = argv[1];
    uint32_t loadAddress = stoul(argv[2], nullptr, 0); // Parse as hex if starts with 0x
    
    // Read MERL file as uint32 array
    vector<uint32_t> merlData = read_merl_file(merlFile);
    
    // Load MERL data using the merl function
    vector<uint32_t> memory = load_merl(merlData, loadAddress);
    
    // Output the final binary
    output_binary(memory);
    
    return 0;
}