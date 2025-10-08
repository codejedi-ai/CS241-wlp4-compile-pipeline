#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;
#define REL 0x00000001
#define ESR 0x00000011
#define ESD 0x00000005  
using word = uint32_t;
using MERLFile = vector<word>;
#define INSTRUCTION_SIZE 4

// Static buffer for instruction string
static char instruction_buffer[128];

// Function to decode instruction based on compiler encoding functions
const char* decode_instruction_from_compiler(word instruction) {
    word opcode = instruction >> 26;
    
    if (opcode == 0) {
        // R-format instruction
        word function_code = instruction & 0x3F;  // bits 5-0
        word s = (instruction >> 21) & 0x1F;           // bits 25-21
        word t = (instruction >> 16) & 0x1F;           // bits 20-16
        word d = (instruction >> 11) & 0x1F;           // bits 15-11
        
        switch (function_code) {
            case 32: // add
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "add $%d, $%d, $%d", d, s, t);
                break;
            case 34: // sub
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "sub $%d, $%d, $%d", d, s, t);
                break;
            case 24: // mult
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mult $%d, $%d", s, t);
                break;
            case 25: // multu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "multu $%d, $%d", s, t);
                break;
            case 26: // div
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "div $%d, $%d", s, t);
                break;
            case 27: // divu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "divu $%d, $%d", s, t);
                break;
            case 16: // mfhi
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mfhi $%d", d);
                break;
            case 18: // mflo
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "mflo $%d", d);
                break;
            case 20: // lis
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "lis $%d", d);
                break;
            case 8: // jr
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "jr $%d", s);
                break;
            case 9: // jalr
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "jalr $%d", s);
                break;
            case 42: // slt
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "slt $%d, $%d, $%d", d, s, t);
                break;
            case 43: // sltu
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        "sltu $%d, $%d, $%d", d, s, t);
                break;
            default:
                snprintf(instruction_buffer, sizeof(instruction_buffer), 
                        ".word %u", instruction);
                break;
        }
    } else if (opcode == 4) { // beq
        word s = (instruction >> 21) & 0x1F;
        word t = (instruction >> 16) & 0x1F;
        word i = instruction & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "beq $%d, $%d, %d", s, t, i);
    } else if (opcode == 5) { // bne
        word s = (instruction >> 21) & 0x1F;
        word t = (instruction >> 16) & 0x1F;
        word i = instruction & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "bne $%d, $%d, %d", s, t, i);
    } else if (opcode == 35) { // lw
        word s = (instruction >> 21) & 0x1F;
        word t = (instruction >> 16) & 0x1F;
        word i = instruction & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "lw $%d, %d($%d)", t, i, s);
    } else if (opcode == 43) { // sw
        word s = (instruction >> 21) & 0x1F;
        word t = (instruction >> 16) & 0x1F;
        word i = instruction & 0xFFFF; // sign-extend immediate
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "sw $%d, %d($%d)", t, i, s);
    } else if (opcode == 2) { // j
        word address = instruction & 0x3FFFFFF;
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "j 0x%08x", address << 2);
    } else if (opcode == 3) { // jal
        word address = instruction & 0x3FFFFFF;
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "jal 0x%08x", address << 2);
    } else {
        snprintf(instruction_buffer, sizeof(instruction_buffer), 
                "unknown (opcode=%d)", opcode);
    }
    
    return instruction_buffer;
}

// Function to decode instruction from char array (INSTRUCTION_SIZE bytes)
const char* decode_instruction_from_bytes(const char* bytes) {
    // Construct 32-bit word in big-endian format (MIPS standard)
    word instruction = ((unsigned char)bytes[0] << 0) |
                    ((unsigned char)bytes[1] << 8) |
                    ((unsigned char)bytes[2] << 16) |
                    ((unsigned char)bytes[3] << 24);
    // Use the existing decoder function
    return decode_instruction_from_compiler(instruction);
}

// Example function showing how to use the decoder
void print_decoded_instruction(const char* bytes) {
    printf("Bytes: ");
    for (int i = 0; i < INSTRUCTION_SIZE; i++) {
        printf("0x%02x ", (unsigned char)bytes[i]);
    }
    printf("-> %s\n", decode_instruction_from_bytes(bytes));
}

// Function to load MERL data and return uint32 array
MERLFile relocate_merl(const MERLFile &data,
                               word loadAddress) {
  int pos = 0;
  vector<word> memory;
  word magicNumber = data[pos++];
  assert(magicNumber == 0x10000002);
  word endModAddress = data[pos++];
  word endModCodeAddress = data[pos++];
  for (int i = 3; i < endModCodeAddress / INSTRUCTION_SIZE; i++) {
    word value = data[i];
    memory.push_back(value);
  }
  pos = endModCodeAddress / INSTRUCTION_SIZE;
  while (pos < data.size()) {
    word recordType = data[pos++];
    if (recordType == REL) {
      word relAddress = data[pos++];
      word index = relAddress - 3 * INSTRUCTION_SIZE;
      index = index / INSTRUCTION_SIZE;
      memory[index] += (loadAddress - 3 * INSTRUCTION_SIZE);
    } else if (recordType == ESR) {
      word esrAddress = data[pos++];
      word esrSize = data[pos++];
      for (int i = 0; i < esrSize; i++) {
        word esrValue = data[pos++];
      }
    } else if (recordType == ESD) {
      word esdAddress = data[pos++];
      word esdSize = data[pos++];
      for (int i = 0; i < esdSize; i++) {
        word esdValue = data[pos++];
      }
    } else {
      cerr << "Error: Invalid record type" << endl;

      exit(1);
    }
  }
  return memory;
}



struct ESR_Entry {
  string symbol;
  uint32_t address;
};

// Read a 32-bit word from a byte stream
MERLFile readMerrlFile(const string &filename) {
  MERLFile data;
  ifstream file(filename, ios::binary);
  if (!file) {
    cerr << "Error: Could not open file " << filename << endl;
    exit(1);
  }

  // read 4 bytes at a time
  char bytes[INSTRUCTION_SIZE];
  while (file.read(bytes, INSTRUCTION_SIZE)) {
    word word_ = 0;
    for (int i = 0; i < INSTRUCTION_SIZE; i++) {
      word_ = (word_ << 8) | bytes[i];
    }
    data.push_back(word_);
  }

  return data;
}

struct SymbolTable {
  vector<ESR_Entry> esr_entries;  // vector of ESR entries
  map<string, word> esd_map;          // symbol -> address
  vector<word> rel_addrs;
};

SymbolTable parseSymbolTable(const vector<word> &file, size_t start_idx, size_t end_idx, size_t offset = 0, const string &prefix = "") {
  SymbolTable table;
  size_t i = start_idx;
  
  while (i < end_idx) {
    const word CODE = file[i];
    if (CODE != ESR && CODE != ESD && CODE != REL) {
      break;
    }
    i++;
    
    if (CODE == ESR) {
      word symbol_address = file[i] + offset;
      i++;
      word symbol_length = file[i];
      i++;
      string symbol = "";
      for (size_t j = 0; j < symbol_length; j++) {
        symbol += static_cast<char>(file[i]);
        i++;
      }
      ESR_Entry esr_entry;
      esr_entry.symbol = symbol;
      esr_entry.address = symbol_address;
      table.esr_entries.push_back(esr_entry);
      cerr << prefix << "ESR[" << symbol << "] = " << hex << '0' << setw(8) << setfill('0') << symbol_address << dec << endl;
    } else if (CODE == ESD) {
      word symbol_address = file[i] + offset;
      i++;
      word symbol_length = file[i];
      i++;
      string symbol = "";
      for (size_t j = 0; j < symbol_length; j++) {
        symbol += static_cast<char>(file[i]);
        i++;
      }
      table.esd_map[symbol] = symbol_address;
      cerr << prefix << "ESD[" << symbol << "] = " << hex << '0' << setw(8) << setfill('0') << symbol_address << dec << endl;
    } else if (CODE == REL) {
      word symbol_address = file[i] + offset;
      i++;
      table.rel_addrs.push_back(symbol_address);
      cerr << prefix << "REL address: " << hex << '0' << setw(8) << setfill('0') << symbol_address << dec << endl;
    }
  }
  return table;
}
vector<word> makeTableFooter(vector<word> rel_addrs, vector<ESR_Entry> esr_entries, map<string, word> esd_map){
  // Add REL entries from m1
  vector<word> table_section;
  for (word rel_addr : rel_addrs) {
    table_section.push_back(REL);
    table_section.push_back(rel_addr);
  }
  // convert the ESR entries of m1 to REL entries
  for (const ESR_Entry &esr_entry : esr_entries) {
    table_section.push_back(REL);
    table_section.push_back(esr_entry.address);
  }
  

  // Add ESD entries from m1
  for (auto it = esd_map.begin(); it != esd_map.end(); ++it) {
    const string &symbol = it->first;
    word address = it->second;
    table_section.push_back(ESD);
    table_section.push_back(address);
    table_section.push_back(symbol.size());
    for (size_t i = 0; i < symbol.size(); i++) {
      table_section.push_back(symbol[i]);
    }
  }

  return table_section;

}
  void resolveESDImport(vector<word> &linked, SymbolTable &table_src, SymbolTable &table_dest){
    for (const ESR_Entry &esr_entry : table_dest.esr_entries) {
        if (table_src.esd_map.find(esr_entry.symbol) != table_src.esd_map.end()) {
          word esd_address = table_src.esd_map[esr_entry.symbol];
          word code_index = (esr_entry.address - 3 * INSTRUCTION_SIZE) / INSTRUCTION_SIZE;
            linked[code_index] = esd_address;
          
        }
      }
}
// Linking algorithm implementation
vector<word> linkMERLFiles(vector<word> m1, vector<word> m2) {
  MERLFile linked;
  
  word m1_end_code = m1[2] / INSTRUCTION_SIZE;
  word m1_end = m1[1] / INSTRUCTION_SIZE;
  word m2_end_code = m2[2] / INSTRUCTION_SIZE;
  word m2_end = m2[1] / INSTRUCTION_SIZE;
  
  cerr << "m1_end_code: " << m1_end_code << ", m1_end: " << m1_end << endl;
  cerr << "m2_end_code: " << m2_end_code << ", m2_end: " << m2_end << endl;
  // Step 0: Parse symbol tables
  // Parse m1 symbol table
  SymbolTable m1_table = parseSymbolTable(m1, m1_end_code, m1_end, 0, "m1_");
  
  // Calculate offset for m2
  word OFFSET = m1_end_code * INSTRUCTION_SIZE - 3 * INSTRUCTION_SIZE;
  
  // Parse m2 symbol table with offset
  SymbolTable m2_table = parseSymbolTable(m2, m2_end_code, m2_end, OFFSET, "m2_");
  
  // Step 1: Combine code sections of m1 and m2
  vector<word> m1_code, m2_code;
  for (size_t i = 3; i < m1_end_code; i++) {
    m1_code.push_back(m1[i]);
  }
  for (size_t i = 3; i < m2_end_code; i++) {
    m2_code.push_back(m2[i]);
  }
  cout << "Size of m1_code: " << m1_code.size() << endl;
  // Step 2: Insert code sections of m1 and m2 into linked
  linked.insert(linked.end(), m1_code.begin(), m1_code.end());
  cout << "Size of m2_code: " << m2_code.size() << endl;
  linked.insert(linked.end(), m2_code.begin(), m2_code.end());

  for (word rel_addr : m2_table.rel_addrs) {
    word code_index = (rel_addr - 3 * INSTRUCTION_SIZE) / INSTRUCTION_SIZE;
    if (code_index < m2_code.size()) {
      linked[code_index] += OFFSET;
    }
  }
  
  // Step 5: Resolve ESR imports in m1 by finding matching ESD in m2
  resolveESDImport(linked, m1_table, m2_table);
  
  // Step 6: Resolve ESR imports in m2 by finding matching ESD in m1
  resolveESDImport(linked, m2_table, m1_table);

  // Step 7: Create table section with REL and ESD entries
  vector<word> table_section_m2 = makeTableFooter(m2_table.rel_addrs, m2_table.esr_entries, m2_table.esd_map);
  vector<word> table_section_m1 = makeTableFooter(m1_table.rel_addrs, m1_table.esr_entries, m1_table.esd_map);
  // Step 8: Create final MERL file with header
  vector<word> table_section;
  table_section.insert(table_section.end(), table_section_m1.begin(), table_section_m1.end());
  table_section.insert(table_section.end(), table_section_m2.begin(), table_section_m2.end());


  vector<word> result;
  result.push_back(0x10000002); // MERL cookie
  result.push_back(3 * INSTRUCTION_SIZE + linked.size() * INSTRUCTION_SIZE + table_section.size() * INSTRUCTION_SIZE); // endModule
  result.push_back(3 * INSTRUCTION_SIZE + linked.size() * INSTRUCTION_SIZE); // endCode
  result.insert(result.end(), linked.begin(), linked.end());
  result.insert(result.end(), table_section.begin(), table_section.end());
  assert(result.size() * INSTRUCTION_SIZE == result[1]);
  assert(linked.size() * INSTRUCTION_SIZE + 3 * INSTRUCTION_SIZE== result[2]);
  return result;
}
void printMERLFile(const vector<word> &file) {
  for (word word : file) {
    cerr << hex << '0' << setw(8) << setfill('0') << word << dec << endl;
  }
}
void writeMERLFile(const vector<word> &file, ostream &out) {
  // Write all words
  for (word word : file) {
    for (int i = 0; i < INSTRUCTION_SIZE; i++) {
      out.put((word >> (24 - i * 8)) & 0xFF);
    }
  }
}
void decode(const char* bytes, int num_bytes) {
  int pos = 0;
  bool previous_was_lis = false;
  
  while (pos < num_bytes) {
      if (pos + INSTRUCTION_SIZE <= num_bytes) {
          // Extract 4 bytes for the instruction
          char instruction_bytes[INSTRUCTION_SIZE];
          for (int i = 0; i < INSTRUCTION_SIZE; i++) {
              instruction_bytes[i] = bytes[pos + i];
          }
          
          // Construct 32-bit word in big-endian format (MIPS standard)
          uint32_t word = ((unsigned char)instruction_bytes[0] << 24) |
                         ((unsigned char)instruction_bytes[1] << 16) |
                         ((unsigned char)instruction_bytes[2] << 8) |
                         (unsigned char)instruction_bytes[3];
          
          const char* decoded;
          if (previous_was_lis) {
              // After lis, treat the next instruction as .word
              decoded = ".word";
              previous_was_lis = false;
          } else {
              decoded = decode_instruction_from_compiler(word);
              // Check if this is a lis instruction
              uint32_t opcode = word >> 26;
              if (opcode == 0) {
                  uint32_t function_code = word & 0x3F;
                  if (function_code == 20) { // lis
                      previous_was_lis = true;
                  }
              }
          }
          
          printf("Instruction %2d: 0x%08x (%s)\n", pos / INSTRUCTION_SIZE, word, decoded);
          
          pos += INSTRUCTION_SIZE;
      } else {
          // Handle remaining bytes that don't form a complete instruction
          printf("Warning: %d bytes remaining, not enough for complete instruction\n", num_bytes - pos);
          break;
      }
  }
}
//int main(int argc, char *argv[]) {
int main() {
//if (argc < 3) {
  //  cerr << "Usage: " << argv[0] << " file1.merl file2.merl" << endl;
  //  return 1;
  //}

  // Read MERL files
  MERLFile m1 = {0x10000002, 0x0000006C, 0x00000034, 0xAFDFFFFC, 0x0000E814, 0x00000000, 0x03A00009, 0x0001814, 0x00000030, 0x8C630000, 0x8FDFFFFC, 0x03E00008, 0x00000000, 0x00000001, 0x00000020, 0x00000011, 0x00000014, 0x00000003, 0x00000066, 0x0000006F, 0x0000006F, 0x00000005, 0x00000030, 0x00000003, 0x00000062, 0x00000061, 0x00000072  };
  MERLFile m2 = {0x10000002, 0x00000078, 0x00000040, 0x00001014, 0xFFFFFFFF, 0x0000E014, 0x00000024, 0x0000E814, 0x00000000, 0x8FA30000, 0x00231820, 0xAFA30000, 0x00220820, 0x14200001, 0x03E00008, 0x03800008, 0x00000001, 0x00000018, 0x00000011, 0x00000020, 0x00000003, 0x00000062, 0x00000061, 0x00000072, 0x00000005, 0x0000000C, 0x00000003, 0x00000066, 0x0000006F, 0x0000006F};
  // print the size of m1
  cout << "Size of m1: " << m1.size() << endl;
  for (size_t i = 0; i < m1.size(); i++) {
    cerr << hex << '0' << setw(8) << setfill('0') << m1[i] << dec << endl;
  }
  
  // print the size of m2
  cout << "Size of m2: " << m2.size() << endl;
  for (size_t i = 0; i < m2.size(); i++) {
    cerr << hex << '0' << setw(8) << setfill('0') << m2[i] << dec << endl;
  }
  // Link the files
  vector<word> linked = linkMERLFiles(m1, m2);
  printMERLFile(linked);
  // Write linked MERL file to stdout
  // writeMERLFile(linked, cout);

  // write the relocated merl file to code relocate to 0
  vector<word> relocated = relocate_merl(linked, 0);
  printMERLFile(relocated);
  // write to file output.merl
  ofstream output_file("output.bin");
  writeMERLFile(relocated, output_file);
  output_file.close();
  cerr << "Done" << endl;
  // turn relocated to vector<char>
  vector<char> relocated_vec;
  for (word word : relocated) {
    char ch1 = (word >> 24) & 0xFF;
    char ch2 = (word >> 16) & 0xFF;
    char ch3 = (word >> 8) & 0xFF;
    char ch4 = word & 0xFF;
    relocated_vec.push_back(ch1);
    relocated_vec.push_back(ch2);
    relocated_vec.push_back(ch3);
    relocated_vec.push_back(ch4);
  }
  // print the size of relocated_vec
  cout << "Size of relocated_vec: " << relocated_vec.size() << endl;
  decode(relocated_vec.data(), relocated_vec.size());
  return 0;
}