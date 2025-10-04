#include "merl.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;


// Function to output uint32 array as binary
void output_binary(const vector<uint32_t>& memory) {
    for (uint32_t word : memory) {
        cout << (char)(word >> 24);
        cout << (char)(word >> 16);
        cout << (char)(word >> 8);
        cout << (char)(word);
    }
}


int main() {
    std::vector<uint32_t> data;

    // Data from the "MERL file" column in the image
    data.push_back(0x10000002);
    data.push_back(0x0000003C);
    data.push_back(0x0000002C);
    data.push_back(0x000000814); // Note: 0x814 is 0x00000814
    data.push_back(0x0000001C);
    data.push_back(0x00200008);
    data.push_back(0x03E00008);
    data.push_back(0x00001820);
    data.push_back(0x00000814);
    data.push_back(0x00000018);
    data.push_back(0x00200008);
    data.push_back(0x00000001); // Highlighted row (0x2C offset)
    data.push_back(0x00000010);
    data.push_back(0x00000001);
    data.push_back(0x00000024);

    // Optional: Print the contents to verify
    std::cout << "Data array contents:" << std::endl;
    for (uint32_t val : data) {
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << val << std::endl;
    }
    std::cout << "Relocated memory:" << std::endl;
    vector<uint32_t> memory = relocate_merl(data, 0);
    for (uint32_t val : memory) {
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << val << std::endl;
    }
    return 0;
}