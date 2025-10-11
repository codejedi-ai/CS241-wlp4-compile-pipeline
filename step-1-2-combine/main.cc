#include "asm.h"   
#include "wlp4compile.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>


int main(){
  string wlp4input = "";
  string line;
  while (getline(cin, line)) {
    wlp4input += line + "\n";
  }
  string asm_input = wlp4_to_asm(wlp4input);
  cout << asm_input;
  // Assemble the input assembly code
  vector<word> binary_output = asmAssemble(asm_input);
  // Output binary data to stdout
  for (word w : binary_output) {
    write_word_as_binary(w, cout);
   }
  
}