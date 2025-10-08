#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
using namespace std;

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
  // Read WLP4 input from stdin
  string wlp4_input = "";
  string line;
  while (getline(cin, line)) {
    wlp4_input += line + "\n";
  }
  
  try {
    // Stage 1: WLP4 to Assembly
    string wlp4_cmd = "echo '" + wlp4_input + "' | ./wlp4-compiler/wlp4compiler";
    string asm_output = exec(wlp4_cmd.c_str());
    
    if (asm_output.empty()) {
      cerr << "ERROR: WLP4 compilation failed" << endl;
      return 1;
    }
    
    // Print the assembly output
    cout << asm_output << endl;
    
    // Stage 2: Assembly to Binary
    string asm_cmd = "echo '" + asm_output + "' | ./asm-compiler/asmcompiler";
    string binary_output = exec(asm_cmd.c_str());
    
    // Output binary (it should be written to stdout by the assembler)
    cout << binary_output;
    
  } catch (const exception& e) {
    cerr << "ERROR: " << e.what() << endl;
    return 1;
  }

  return 0;
}
    