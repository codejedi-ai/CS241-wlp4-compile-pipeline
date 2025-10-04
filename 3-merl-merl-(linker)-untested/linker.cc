#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
#define REL 0x00000001
#define ESR 0x00000011
#define ESD 0x00000005
using MERLFile = vector<uint32_t>;

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
  char bytes[4];
  while (file.read(bytes, 4)) {
    uint32_t word = (static_cast<uint8_t>(bytes[0]) << 24) |
                    (static_cast<uint8_t>(bytes[1]) << 16) |
                    (static_cast<uint8_t>(bytes[2]) << 8) |
                    static_cast<uint8_t>(bytes[3]);
    data.push_back(word);
  }

  return data;
}

struct SymbolTable {
  vector<ESR_Entry> esr_entries;  // vector of ESR entries
  map<string, uint32_t> esd_map;          // symbol -> address
  vector<uint32_t> rel_addrs;
};

SymbolTable parseSymbolTable(const MERLFile &file, size_t start_idx, size_t end_idx, uint32_t offset = 0, const string &prefix = "") {
  SymbolTable table;
  size_t i = start_idx;
  
  while (i < end_idx) {
    const uint32_t CODE = file[i];
    if (CODE != ESR && CODE != ESD && CODE != REL) {
      break;
    }
    i++;
    
    if (CODE == ESR) {
      uint32_t symbol_address = file[i] + offset;
      i++;
      uint32_t symbol_length = file[i];
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
      uint32_t symbol_address = file[i] + offset;
      i++;
      uint32_t symbol_length = file[i];
      i++;
      string symbol = "";
      for (size_t j = 0; j < symbol_length; j++) {
        symbol += static_cast<char>(file[i]);
        i++;
      }
      table.esd_map[symbol] = symbol_address;
      cerr << prefix << "ESD[" << symbol << "] = " << hex << '0' << setw(8) << setfill('0') << symbol_address << dec << endl;
    } else if (CODE == REL) {
      uint32_t symbol_address = file[i] + offset;
      i++;
      table.rel_addrs.push_back(symbol_address);
      cerr << prefix << "REL address: " << hex << '0' << setw(8) << setfill('0') << symbol_address << dec << endl;
    }
  }
  return table;
}
vector<uint32_t> makeTableFooter(vector<uint32_t> rel_addrs, vector<ESR_Entry> esr_entries, map<string, uint32_t> esd_map){
  // Add REL entries from m1
  vector<uint32_t> table_section;
  for (uint32_t rel_addr : rel_addrs) {
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
    uint32_t address = it->second;
    table_section.push_back(ESD);
    table_section.push_back(address);
    table_section.push_back(symbol.size());
    for (size_t i = 0; i < symbol.size(); i++) {
      table_section.push_back(symbol[i]);
    }
  }

  return table_section;

}
void resolveESDImport(vector<uint32_t> &linked, SymbolTable &table_src, SymbolTable &table_dest){
    for (const ESR_Entry &esr_entry : table_dest.esr_entries) {
        if (table_src.esd_map.find(esr_entry.symbol) != table_src.esd_map.end()) {
          uint32_t esd_address = table_src.esd_map[esr_entry.symbol];
          uint32_t code_index = (esr_entry.address - 12) / 4;
            linked[code_index] = esd_address;
          
        }
      }
}
// Linking algorithm implementation
MERLFile linkMERLFiles(MERLFile m1, MERLFile m2) {
  MERLFile linked;
  
  uint32_t m1_end_code = m1[2] / 4;
  uint32_t m1_end = m1[1] / 4;
  uint32_t m2_end_code = m2[2] / 4;
  uint32_t m2_end = m2[1] / 4;
  
  cerr << "m1_end_code: " << m1_end_code << ", m1_end: " << m1_end << endl;
  cerr << "m2_end_code: " << m2_end_code << ", m2_end: " << m2_end << endl;
  // Step 0: Parse symbol tables
  // Parse m1 symbol table
  SymbolTable m1_table = parseSymbolTable(m1, m1_end_code, m1_end, 0, "m1_");
  
  // Calculate offset for m2
  uint32_t OFFSET = m1_end_code * 4 - 12;
  
  // Parse m2 symbol table with offset
  SymbolTable m2_table = parseSymbolTable(m2, m2_end_code, m2_end, OFFSET, "m2_");
  
  // Step 1: Combine code sections of m1 and m2
  MERLFile m1_code, m2_code;
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
  // print the size of linked
  cout << "Size of linked: " << linked.size() << endl;
  // Step 3: Resolve REL entries in m2
  for (uint32_t rel_addr : m2_table.rel_addrs) {
    uint32_t code_index = (rel_addr - 12) / 4;
    if (code_index < m2_code.size()) {
      linked[code_index] += OFFSET;
    }
  }
  
  // Step 5: Resolve ESR imports in m1 by finding matching ESD in m2
  resolveESDImport(linked, m1_table, m2_table);
  
  // Step 6: Resolve ESR imports in m2 by finding matching ESD in m1
  resolveESDImport(linked, m2_table, m1_table);

  // Step 7: Create table section with REL and ESD entries
  vector<uint32_t> table_section_m2 = makeTableFooter(m2_table.rel_addrs, m2_table.esr_entries, m2_table.esd_map);
  vector<uint32_t> table_section_m1 = makeTableFooter(m1_table.rel_addrs, m1_table.esr_entries, m1_table.esd_map);
  // Step 8: Create final MERL file with header
  vector<uint32_t> table_section;
  table_section.insert(table_section.end(), table_section_m1.begin(), table_section_m1.end());
  table_section.insert(table_section.end(), table_section_m2.begin(), table_section_m2.end());


  MERLFile result;
  result.push_back(0x10000002); // MERL cookie
  result.push_back(12 + linked.size() * 4 + table_section.size() * 4); // endModule
  result.push_back(12 + linked.size() * 4); // endCode
  result.insert(result.end(), linked.begin(), linked.end());
  result.insert(result.end(), table_section.begin(), table_section.end());
  assert(result.size() * 4 == result[1]);
  assert(linked.size() * 4 + 12== result[2]);
  return result;
}
void printMERLFile(const MERLFile &file) {
  for (uint32_t word : file) {
    cerr << hex << '0' << setw(8) << setfill('0') << word << dec << endl;
  }
}
void writeMERLFile(const MERLFile &file, ostream &out) {
  // Write all words
  for (uint32_t word : file) {
    out.put((word >> 24) & 0xFF);
    out.put((word >> 16) & 0xFF);
    out.put((word >> 8) & 0xFF);
    out.put(word & 0xFF);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " file1.merl file2.merl" << endl;
    return 1;
  }

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
  MERLFile linked = linkMERLFiles(m1, m2);
  printMERLFile(linked);
  // Write linked MERL file to stdout
  writeMERLFile(linked, cout);

  return 0;
}