#include "asm.h"
#include "wlp4compile.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    string wlp4input;
    string line;

    // Read full WLP4 program from stdin
    while (getline(cin, line)) {
        wlp4input += line + "\n";
    }

    // Step 1: Compile WLP4 -> Assembly
    string asm_input = wlp4_to_asm(wlp4input);

    // Step 2: Output the generated assembly to a file
    ofstream asm_file("output.asm");
    if (!asm_file) {
        cerr << "Error: Could not open output.asm for writing." << endl;
        return 1;
    }
    asm_file << asm_input;
    asm_file.close();

    // Step 3: Assemble -> Binary
    vector<word> binary_output = asmAssemble(asm_input);

    // Step 4: Output binary data to both stdout and file
    ofstream bin_file("output.mips", ios::binary);
    if (!bin_file) {
        cerr << "Error: Could not open output.mips for writing." << endl;
        return 1;
    }

    for (word w : binary_output) {
        write_word_as_binary(w, cout);      // Write to stdout
        write_word_as_binary(w, bin_file);  // Write to file
    }

    bin_file.close();

    cerr << "Compilation successful! Assembly written to output.asm and binary to output.mips." << endl;
    return 0;
}
