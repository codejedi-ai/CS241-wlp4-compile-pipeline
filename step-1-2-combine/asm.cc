#include "scanner.h"
#include "decoder.h"
#include "wlp4compile.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "asm.h"
#define INSTRUCTION_SIZE 4
// const char opcode = 6; // 6 bits for opcode 32 - 6 = 26
//const char sreg = 5 + opcode; // 5 bits for sreg 26 - 5 = 21
//const char treg = 5 + sreg; // 5 bits for treg 21 - 5 = 16
//const char dreg = 5 + treg; // 5 bits for dreg 16 - 5 = 11
using namespace std;

// Helper function to write a word as binary data (4 bytes)
void write_word_as_binary(uint32_t word, ostream &out) {
  for (int i = 0; i < INSTRUCTION_SIZE; i++) {
    unsigned char c = (word >> (24 - i * 8)) & 0xFF;
    out << c;
  }
}
namespace asm_compiler {
std::vector<uint32_t> assembly_binary_code;
std::map<std::string, uint32_t> symbolTable;
std::map<std::string, vector<uint32_t>> label_pc_map;
std::map<std::string, vector<uint32_t>> branch_reference_map;
std::vector<std::string> asm_files;
//typedef word_cont uint32_t;
// Instruction size constant (can be changed for different architectures)
// static constexpr uint32_t INSTRUCTION_SIZE = 4;

void writebin(uint32_t instr) { assembly_binary_code.push_back(instr); }
void coutmult(uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 24; // add $3, $2, $4
  writebin(instr);
}
void coutmultu(uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 25; // add $3, $2, $4
  writebin(instr);
}
void coutdiv(uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 26; // add $3, $2, $4
  writebin(instr);
}
void coutdivu(uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 27; // add $3, $2, $4
  writebin(instr);
}
void coutAdd(uint32_t d, uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 32; // add $3, $2, $4
  writebin(instr);
}
void coutSub(uint32_t d, uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 34; // add $3, $2, $4
  writebin(instr);
}
void coutSlt(uint32_t d, uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 42; // add $3, $2, $4
  writebin(instr);
}
void coutSltu(uint32_t d, uint32_t s, uint32_t t) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 43; // add $3, $2, $4
  writebin(instr);
}
void coutBeq(uint32_t s, uint32_t t, uint32_t i) {
  i = i & (0xffff);
  uint32_t instr = (4 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  writebin(instr);
}
void coutBne(uint32_t s, uint32_t t, uint32_t i) {
  i = i & (0xffff);
  uint32_t instr = (5 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  writebin(instr);
}

void jr(uint32_t s) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 8; // add $3, $2, $4
  writebin(instr);
}
void jalr(uint32_t s) {
  uint32_t instr =
      (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 9; // add $3, $2, $4
  writebin(instr);
}
void mfhi(uint32_t d) {
  uint32_t instr =
      (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 16; // add $3, $2, $4
  writebin(instr);
}
void mflo(uint32_t d) {
  uint32_t instr =
      (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 18; // add $3, $2, $4
  writebin(instr);
}
void lis(uint32_t d) {
  uint32_t instr =
      (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 20; // add $3, $2, $4
  writebin(instr);
}
void coutSw(uint32_t t, uint32_t i, uint32_t s) {
  i = i & (0xffff);
  uint32_t instr = (43 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  writebin(instr);
}
void coutLw(uint32_t t, uint32_t i, uint32_t s) {
  i = i & (0xffff);
  uint32_t instr = (35 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  writebin(instr);
}
/*
add $1, $2, $3
Token(ID, add) Token(REG, $1) Token(COMMA, ,) Token(REG, $2) Token(COMMA, ,)
Token(REG, $3)
*/
bool add_sub_slt_sltu(uint32_t ind, std::vector<Token> &vecref) {

  bool sizeMeet = (ind + 6 == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = vecref[ind].getKind() == Token::ID &&
                       vecref[ind + 1].getKind() == Token::REG &&
                       vecref[ind + 2].getKind() == Token::COMMA &&
                       vecref[ind + 3].getKind() == Token::REG &&
                       vecref[ind + 4].getKind() == Token::COMMA &&
                       vecref[ind + 5].getKind() == Token::REG;
  if (!structureMeet)
    return false;
  bool instructionValid =
      vecref[ind].getLexeme() == "add" || vecref[ind].getLexeme() == "sub" ||
      vecref[ind].getLexeme() == "slt" || vecref[ind].getLexeme() == "sltu";
  if (!instructionValid)
    return false;
  return true;
}
bool mult_multu_div_divu(uint32_t ind, std::vector<Token> &vecref) {
  vector<Token> buf;
  try {
    buf = scan("mult $4, $3");
  } catch (ScanningFailure &f) {
    return false;
  }
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = true;

  for (uint32_t i = 0; i < buf.size(); i++) {
    structureMeet =
        structureMeet && (vecref[ind + i].getKind() == buf[i].getKind());
  }

  if (!structureMeet)
    return false;
  bool instructionValid =
      vecref[ind].getLexeme() == "mult" || vecref[ind].getLexeme() == "multu" ||
      vecref[ind].getLexeme() == "div" || vecref[ind].getLexeme() == "divu";
  if (!instructionValid)
    return false;
  return true;
}
// Token(ID, lis) Token(REG, $4)
bool mfhi_mflo_lis(uint32_t ind, std::vector<Token> &vecref) {

  bool sizeMeet = (ind + 2 == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = vecref[ind].getKind() == Token::ID &&
                       vecref[ind + 1].getKind() == Token::REG;
  if (!structureMeet)
    return false;
  bool instructionValid = vecref[ind].getLexeme() == "mfhi" ||
                          vecref[ind].getLexeme() == "mflo" ||
                          vecref[ind].getLexeme() == "lis";
  if (!instructionValid)
    return false;
  return true;
}
// Token(ID, lw) Token(REG, $4) Token(COMMA, ,) Token(INT, 1) Token(LPAREN, ()
// Token(REG, $3) Token(RPAREN, ))
bool lw_sw(uint32_t ind, std::vector<Token> &vecref) {
  vector<Token> buf;
  try {
    buf = scan("lw $4, 1($3)");
  } catch (ScanningFailure &f) {
    return false;
  }
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (uint32_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;
  try {
    buf = scan("lw $4, 0x1($3)");
  } catch (ScanningFailure &f) {
    return false;
  }
  bufTemp = true;
  for (uint32_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;
  if (!structureMeet)
    return false;
  bool instructionValid =
      vecref[ind].getLexeme() == "lw" || vecref[ind].getLexeme() == "sw";
  if (!instructionValid)
    return false;
  return true;
}
bool beq_bne(uint32_t ind, std::vector<Token> &vecref) {
  vector<Token> buf;
  try {
    buf = scan("beq $4, $0, i");
  } catch (ScanningFailure &f) {
    return false;
  }
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (uint32_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  try {
    buf = scan("beq $4, $0, 0x0");
  } catch (ScanningFailure &f) {
    return false;
  }
  bufTemp = true;
  for (uint32_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  try {
    buf = scan("beq $4, $0, 1");
  } catch (ScanningFailure &f) {
    return false;
  }
  bufTemp = true;
  for (uint32_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  if (!structureMeet)
    return false;

  bool instructionValid =
      vecref[ind].getLexeme() == "beq" || vecref[ind].getLexeme() == "bne";
  if (!instructionValid)
    return false;
  return true;
}
bool jr_jalr(uint32_t ind, std::vector<Token> &vecref) {

  bool sizeMeet = (ind + 2 == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = vecref[ind].getKind() == Token::ID &&
                       vecref[ind + 1].getKind() == Token::REG;
  if (!structureMeet)
    return false;
  bool instructionValid =
      vecref[ind].getLexeme() == "jr" || vecref[ind].getLexeme() == "jalr";
  if (!instructionValid)
    return false;
  return true;
}

bool is_word_cont(uint32_t ind, std::vector<Token> &vecref) {
  vector<Token> buf;
  try {
    buf = scan(".word i");
  } catch (ScanningFailure &f) {
    return false;
  }
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (size_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  try {
    buf = scan(".word 0x0");
  } catch (ScanningFailure &f) {
    return false;
  }
  bufTemp = true;
  for (size_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  try {
    buf = scan(".word 1");
  } catch (ScanningFailure &f) {
    return false;
  }
  bufTemp = true;
  for (size_t i = 0; i < buf.size(); i++) {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  bool instructionValid = vecref[ind].getKind() == Token::WORD;
  if (!instructionValid)
    return false;
  return true;
}
/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */

struct AsmReturn{
  std::vector<uint32_t> assembly_binary_code;
  std::map<std::string, uint32_t> symbolTable;
  std::map<std::string, vector<uint32_t>> label_pc_map;
  std::map<std::string, vector<uint32_t>> branch_reference_map;
  std::vector<std::string> asm_files;
  bool error = false;
};
void addSourceLine(const std::string &line) { asm_files.push_back(line); }
void addImportedLabel(const std::string &label) { symbolTable[label] = 0; }
AsmReturn assemble(uint32_t pc_start) {
  AsmReturn ret;
  std::vector<std::vector<Token>> assemblyProgram;
  vector<string> instructions{"add",  "sub",  "mult", "multu", "div", "divu",
                              "mfhi", "mflo", "lis",  "lw",    "sw",  "slt",
                              "sltu", "beq",  "bne",  "jr",    "jalr"};

  for (std::string file : asm_files) {
    try {
      assemblyProgram.push_back(scan(file));
    } catch (ScanningFailure &f) {
      std::cerr << f.what() << std::endl;
      ret.error = true;
      return ret;
    }
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.

  //__________________________________

  uint32_t pc = pc_start;
  for (std::vector<Token> line : assemblyProgram) {
    if (line.empty())
      continue;

    uint32_t ind = 0;
    // I CANNOT USE LOOPS OR ELSE I WOULD GET ERROR
    /*
    Not using the while loop I cannot have infinite labels infront of the
    instruction

    */
    while (ind < line.size() && line[ind].getKind() == Token::LABEL) {
      string buf = line[ind].getLexeme();
      buf.resize(buf.size() - 1);
      if (symbolTable.count(buf)) {
        std::cerr << "ERROR: Duplicate Labels" << std::endl;
        ret.error = true;
        return ret;
      }
      symbolTable[buf] = pc;
      ind++;
    }
    if (ind == line.size())
      continue;
    // at this point the following MUST be valid instructions if not then the
    // code is invalid
    bool found = lw_sw(ind, line) || mult_multu_div_divu(ind, line) ||
                 mfhi_mflo_lis(ind, line) || beq_bne(ind, line) ||
                 is_word_cont(ind, line) || jr_jalr(ind, line) ||
                 add_sub_slt_sltu(ind, line);

    if (!found) {
      std::cerr << "ERROR: Invalid instruction or parameters" << std::endl;
      ret.error = true;
      return ret;
    } else {
      pc += INSTRUCTION_SIZE;
    }
  }

  // ------------------------------------------------------------------------------------------------------------------
  pc = pc_start;
  for (std::vector<Token> line : assemblyProgram) {
    if (line.empty())
      continue;
    uint32_t ind = 0;
    while (ind < line.size() && line[ind].getKind() == Token::LABEL) {
      ind++;
    }
    // token is not a label
    if (ind == line.size())
      continue;
    
    // Increment PC BEFORE processing instruction
    pc += INSTRUCTION_SIZE;
    uint32_t instr = 0;
    
    if (ind < line.size() && line[ind].getKind() == Token::WORD) {
      ind++;
      string str = line[ind].getLexeme();
      if (line[ind].getKind() == Token::ID) {
        if (symbolTable.count(str) == 0) {
          std::cerr << "ERROR: Invalid labell:" << str << std::endl;
          ret.error = true;
          return ret;
        }
        instr = symbolTable[str];
        // the string need the pc
        // this is for the MERRL file generation
        // For every .word label we need to record its pc
        label_pc_map[str].push_back(pc);
      } else {
        instr = line[ind].toNumber();
      }
      /*
      if (line[ind].getKind() == Token::INT && instr < 0)
      {

          std::cerr << "ERROR: No negative numbers for .word_cont " << str <<
      std::endl; return 1;
      }
      */
      writebin(instr);
    } else if (line[ind].getKind() == Token::ID) {

      if (line[ind].getLexeme() == "jr") {
        instr = line[ind + 1].toNumber();
        jr(instr);
      }
      if (line[ind].getLexeme() == "jalr") {
        instr = line[ind + 1].toNumber();
        jalr(instr);
      }
      if (line[ind].getLexeme() == "add") {
        uint32_t d = line[ind + 1].toNumber();
        uint32_t s = line[ind + 3].toNumber();
        uint32_t t = line[ind + 5].toNumber();
        coutAdd(d, s, t);
      }
      if (line[ind].getLexeme() == "sub") {
        uint32_t d = line[ind + 1].toNumber();
        uint32_t s = line[ind + 3].toNumber();
        uint32_t t = line[ind + 5].toNumber();
        coutSub(d, s, t);
      }
      if (line[ind].getLexeme() == "slt") {
        uint32_t d = line[ind + 1].toNumber();
        uint32_t s = line[ind + 3].toNumber();
        uint32_t t = line[ind + 5].toNumber();
        coutSlt(d, s, t);
      }
      if (line[ind].getLexeme() == "sltu") {
        uint32_t d = line[ind + 1].toNumber();
        uint32_t s = line[ind + 3].toNumber();
        uint32_t t = line[ind + 5].toNumber();
        coutSltu(d, s, t);
      }

      // Question 6
      if (line[ind].getLexeme() == "beq") {
        uint32_t s = line[ind + 1].toNumber();
        uint32_t t = line[ind + 3].toNumber();
        uint32_t i = 0;
        if (line[ind + 5].getKind() == Token::ID) {
          if (symbolTable.count(line[ind + 5].getLexeme()) == 0) {
            std::cerr << "ERROR: " << line[ind + 5].getLexeme()
                      << " is an invalid token" << std::endl;
            ret.error = true;
            return ret;
          }
          i = symbolTable[line[ind + 5].getLexeme()] - pc;
          i = i / INSTRUCTION_SIZE;
          // Track branch reference
          branch_reference_map[line[ind + 5].getLexeme()].push_back(pc - INSTRUCTION_SIZE);
        } else {
          i = line[ind + 5].toNumber();
          if ((line[ind + 5].getKind() == Token::INT) &&
              !(-32768 <= (int32_t)i && (int32_t)i <= 32767)) {
            std::cerr << "ERROR: Step count out of range. must be -32768 <= i "
                         "<= 32767"
                      << std::endl;
            ret.error = true;
            return ret;
          }
          if ((line[ind + 5].getKind() == Token::HEXINT) && i > 0xffff) {
            std::cerr << "ERROR: Step count out of range. must be i <= 0xffff"
                      << std::endl;
            ret.error = true;
            return ret;
          }
        }
        coutBeq(s, t, i);
      }
      if (line[ind].getLexeme() == "bne") {
        uint32_t s = line[ind + 1].toNumber();
        uint32_t t = line[ind + 3].toNumber();
        uint32_t i = 0;
        if (line[ind + 5].getKind() == Token::ID) {
          if (symbolTable.count(line[ind + 5].getLexeme()) == 0) {
            std::cerr << "ERROR: " << line[ind + 5].getLexeme()
                      << " is an invalid token" << std::endl;
            ret.error = true;
            return ret;
          }
          i = symbolTable[line[ind + 5].getLexeme()] - pc;
          i = i / INSTRUCTION_SIZE;
          // Track branch reference
          branch_reference_map[line[ind + 5].getLexeme()].push_back(pc - INSTRUCTION_SIZE);
        } else {
          i = line[ind + 5].toNumber();
          if ((line[ind + 5].getKind() == Token::INT) &&
              !(-32768 <= (int32_t)i && (int32_t)i <= 32767)) {
            std::cerr << "ERROR: Step count out of range. must be -32768 <= i "
                         "<= 32767"
                      << std::endl;
            ret.error = true;
            return ret;
          }
          if ((line[ind + 5].getKind() == Token::HEXINT) && i > 0xffff) {
            std::cerr << "ERROR: Step count out of range. must be i <= 0xffff"
                      << std::endl;
            ret.error = true;
            return ret;
          }
        }
        coutBne(s, t, i);
      }
      // Problem 7 ughhh completed?? Question 8
      if (line[ind].getLexeme() == "lis") {
        instr = line[ind + 1].toNumber();
        lis(instr);
      }
      if (line[ind].getLexeme() == "mflo") {
        instr = line[ind + 1].toNumber();
        mflo(instr);
      }
      if (line[ind].getLexeme() == "mfhi") {
        instr = line[ind + 1].toNumber();
        mfhi(instr);
      }
    }
    // Question 9
    if (line[ind].getLexeme() == "mult") {
      uint32_t s = line[ind + 1].toNumber();
      uint32_t t = line[ind + 3].toNumber();
      coutmult(s, t);
    }
    if (line[ind].getLexeme() == "multu") {
      uint32_t s = line[ind + 1].toNumber();
      uint32_t t = line[ind + 3].toNumber();
      coutmultu(s, t);
    }
    if (line[ind].getLexeme() == "div") {
      uint32_t s = line[ind + 1].toNumber();
      uint32_t t = line[ind + 3].toNumber();
      coutdiv(s, t);
    }
    if (line[ind].getLexeme() == "divu") {
      uint32_t s = line[ind + 1].toNumber();
      uint32_t t = line[ind + 3].toNumber();
      coutdivu(s, t);
    }
    // Problem 10
    if (line[ind].getLexeme() == "sw") {
      uint32_t t = line[ind + 1].toNumber();
      uint32_t i = line[ind + 3].toNumber();
      uint32_t s = line[ind + 5].toNumber();
      coutSw(t, i, s);
    }
    if (line[ind].getLexeme() == "lw") {
      uint32_t t = line[ind + 1].toNumber();
      uint32_t i = line[ind + 3].toNumber();
      uint32_t s = line[ind + 5].toNumber();
      coutLw(t, i, s);
    }
  }
  ret.assembly_binary_code = assembly_binary_code;
  ret.symbolTable = symbolTable;
  ret.label_pc_map = label_pc_map;
  ret.branch_reference_map = branch_reference_map;
  ret.asm_files = asm_files;
  return ret;
}
vector<uint32_t> get_assembly_binary_code() {
  return assembly_binary_code;
}



// struct REL_ENTRY
struct REL_ENTRY {
  uint32_t address;
  REL_ENTRY(uint32_t address) : address(address) {}
};
// struct ESR_ENTRY
struct ESR_ENTRY {
  uint32_t address;
  std::string label;
  ESR_ENTRY(uint32_t address, std::string label)
      : address(address), label(label) {}
};
// struct ESD_ENTRY
struct ESD_ENTRY {
  uint32_t address;
  std::string label;
  ESD_ENTRY(uint32_t address, std::string label)
      : address(address), label(label) {}
};

vector<uint32_t>
get_entries_binary(std::set<std::string> export_labels,
                   std::set<std::string> import_labels,
                   std::map<std::string, uint32_t> symbolTable,
                   std::map<std::string, vector<uint32_t>> label_pc_map) {
  // print the tables to cerr
  // vector<uint32_t> rel_enteries;
  // ESR entries
  // vector<uint32_t> esr_entries;
  vector<REL_ENTRY> rel_enteries;
  vector<ESR_ENTRY> esr_entries;
  vector<ESD_ENTRY> esd_entries;
  // for each export
  for (auto const &x : export_labels) {
    esd_entries.push_back(ESD_ENTRY{symbolTable[x], x});
  }
  for (auto const &x : label_pc_map) {

    for (uint32_t pc : x.second) {
      if (import_labels.count(x.first) == 0) {
        rel_enteries.push_back(REL_ENTRY{pc});
      } else {
        esr_entries.push_back(ESR_ENTRY{pc, x.first});
      }
    }
  }
  vector<uint32_t> entries_binary;
  for (auto const &x : rel_enteries) {
    std::cerr << "Rel entry: " << x.address << endl;
    entries_binary.push_back(0x00000001);
    entries_binary.push_back(x.address);
  }
  for (auto const &x : esr_entries) {
    entries_binary.push_back(0x00000011);
    entries_binary.push_back(x.address);
    entries_binary.push_back(x.label.length());
    for (char c : x.label) {
      entries_binary.push_back(c);
    }
  }
  for (auto const &x : esd_entries) {
    entries_binary.push_back(0x00000005);
    entries_binary.push_back(x.address);
    entries_binary.push_back(x.label.length());
    for (char c : x.label) {
      entries_binary.push_back(c);
    }
  }
  return entries_binary;
}
vector<uint32_t> get_merl_file(vector<uint32_t> assembly_binary_code, vector<uint32_t> entries_binary) {
  // get the cookie
  uint32_t cookie = 0x10000002;
  // get the end of file
  uint32_t end_of_code = assembly_binary_code.size() * INSTRUCTION_SIZE + INSTRUCTION_SIZE * 3; // Note: 4 is word_cont size, 12 is MERL header size
  uint32_t end_of_file = entries_binary.size() * 4 + end_of_code; // Note: 4 is word_cont size
  // print the assembly_binary_code

  vector<uint32_t> merl_file;
  merl_file.push_back(cookie);
  merl_file.push_back(end_of_file);
  merl_file.push_back(end_of_code);
  merl_file.insert(merl_file.end(), assembly_binary_code.begin(),
                   assembly_binary_code.end());
  merl_file.insert(merl_file.end(), entries_binary.begin(),
                   entries_binary.end());
  // print the merl_file

  return merl_file;
}

// Helper function to write binary data to file in big-endian format
bool write_binary_file(const std::string& filename, const vector<uint32_t>& data) {
  std::ofstream outfile(filename, std::ios::binary);
  if (!outfile) {
    std::cerr << "ERROR: Cannot open output file: " << filename << std::endl;
    return false;
  }
  
  // Write data in big-endian format
  for (uint32_t word_cont : data) {
    unsigned char c = word_cont >> 24;
    outfile << c;
    c = word_cont >> 16;
    outfile << c;
    c = word_cont >> 8;
    outfile << c;
    c = word_cont;
    outfile << c;
  }
  
  outfile.close();
  std::cerr << "Successfully wrote to file: " << filename << std::endl;
  return true;
}

// Helper function to determine output filename based on file suffix
std::string get_output_filename(const std::string& file_suffix) {
  return (file_suffix == ".merl") ? "output.merl" : "output.bin";
}

vector<word> to_merl(string input_asm_contents){
  vector<word> ret;
  // import label from .import make set
  std::set<std::string> import_labels;
  // export label from .export make set
  std::set<std::string> export_labels;
  string file_suffix = ".bin";
  uint32_t pc_start = 0;
  
  // string stream from input_asm_contents
  std::stringstream input_asm_stream(input_asm_contents);
  try {
    std::string line;
    while (getline(input_asm_stream, line)) {
      // Tokenize each input line and treat .import/.export as commands
      std::vector<Token> toks;
      try {
        toks = scan(line);
      } catch (ScanningFailure &f) {
        std::cerr << f.what() << std::endl;
        return ret;
      }
      if (!toks.empty()) {
        if (toks[0].getKind() == Token::IMPORT) {
          if (toks.size() >= 2 && toks[1].getKind() == Token::ID) {
            std::string label = toks[1].getLexeme();
            import_labels.insert(label);
            asm_compiler::addImportedLabel(label);
            file_suffix = ".merl";
            pc_start = 0xc;
            continue;
          }
        } else if (toks[0].getKind() == Token::EXPORT) {
          if (toks.size() >= 2 && toks[1].getKind() == Token::ID) {
            std::string label = toks[1].getLexeme();
            export_labels.insert(label);
            file_suffix = ".merl";
            pc_start = 0xc;
            continue;
          }
        }
      }
      // Not an import/export directive: treat as assembly source
      asm_compiler::addSourceLine(line);
    }
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return ret;
  }
  asm_compiler::AsmReturn result = asm_compiler::assemble(pc_start);
  if (result.error) return ret;
  
  // Prepare data for writing
  vector<uint32_t> data_to_write;
  
  if (file_suffix == ".merl") {
    // Generate MERL file
    vector<uint32_t> entries_binary = get_entries_binary(export_labels, import_labels, result.symbolTable, result.label_pc_map);
    data_to_write = get_merl_file(result.assembly_binary_code, entries_binary);
  } else {
    // Use assembly binary code directly
    data_to_write = result.assembly_binary_code;
  }
  
  // Add data to return vector
  ret.insert(ret.end(), data_to_write.begin(), data_to_write.end());
  
  return ret;
}
// Structure to hold analysis data
struct AnalysisData {
  std::map<std::string, uint32_t> symbolTable;
  std::map<std::string, vector<uint32_t>> label_pc_map;
  std::map<std::string, vector<uint32_t>> branch_reference_map;
  std::set<std::string> import_labels;
  std::set<std::string> export_labels;
  vector<uint32_t> binary_output;
};

// Function to get analysis data
AnalysisData get_analysis_data(std::string input_asm_contents) {
  AnalysisData data;
  

  std::stringstream input_asm_stream(input_asm_contents);
  uint32_t pc_start = 0;
  
  try {
    std::string line;
    while (getline(input_asm_stream, line)) {
      std::vector<Token> tokens;
      try {
        tokens = scan(line);
      } catch (ScanningFailure &f) {
        continue; // Skip invalid lines
      }
      if (!tokens.empty()) {
        if (tokens[0].getKind() == Token::IMPORT && tokens.size() >= 2) {
          std::string label = tokens[1].getLexeme();
          data.import_labels.insert(label);
          asm_compiler::addImportedLabel(label);
        } else if (tokens[0].getKind() == Token::EXPORT && tokens.size() >= 2) {
          std::string label = tokens[1].getLexeme();
          data.export_labels.insert(label);
        } else {
          asm_compiler::addSourceLine(line);
        }
      }
    }

    asm_compiler::AsmReturn result = asm_compiler::assemble(pc_start);
    if (!result.error) {
      data.symbolTable = result.symbolTable;
      data.label_pc_map = result.label_pc_map;
      data.branch_reference_map = result.branch_reference_map;
      
      // Generate binary output
      if (data.import_labels.empty() && data.export_labels.empty()) {
        data.binary_output = result.assembly_binary_code;
      } else {
        vector<uint32_t> entries_binary = get_entries_binary(data.export_labels, data.import_labels, result.symbolTable, result.label_pc_map);
        data.binary_output = get_merl_file(result.assembly_binary_code, entries_binary);
      }
    }
  } catch (...) {
    // Handle errors gracefully
  }
  
  return data;
}

// Helper function to decode a single instruction
string decode_instruction(uint32_t word) {
  uint32_t opcode = word >> 26;
  
  if (opcode == 0) {
    // R-format instruction
    uint32_t function_code = word & 0x3F;  // bits 5-0
    int s = (word >> 21) & 0x1F;           // bits 25-21
    int t = (word >> 16) & 0x1F;           // bits 20-16
    int d = (word >> 11) & 0x1F;           // bits 15-11
    
    switch (function_code) {
      case 32: return "add $" + to_string(d) + ", $" + to_string(s) + ", $" + to_string(t);
      case 34: return "sub $" + to_string(d) + ", $" + to_string(s) + ", $" + to_string(t);
      case 24: return "mult $" + to_string(s) + ", $" + to_string(t);
      case 25: return "multu $" + to_string(s) + ", $" + to_string(t);
      case 26: return "div $" + to_string(s) + ", $" + to_string(t);
      case 27: return "divu $" + to_string(s) + ", $" + to_string(t);
      case 16: return "mfhi $" + to_string(d);
      case 18: return "mflo $" + to_string(d);
      case 20: return "lis $" + to_string(d);
      case 8: return "jr $" + to_string(s);
      case 9: return "jalr $" + to_string(s);
      case 42: return "slt $" + to_string(d) + ", $" + to_string(s) + ", $" + to_string(t);
      case 43: return "sltu $" + to_string(d) + ", $" + to_string(s) + ", $" + to_string(t);
      default: return ".word " + to_string(word);
    }
  } else if (opcode == 4) { // beq
    int s = (word >> 21) & 0x1F;
    int t = (word >> 16) & 0x1F;
    int16_t i = word & 0xFFFF; // sign-extend immediate
    return "beq $" + to_string(s) + ", $" + to_string(t) + ", " + to_string(i);
  } else if (opcode == 5) { // bne
    int s = (word >> 21) & 0x1F;
    int t = (word >> 16) & 0x1F;
    int16_t i = word & 0xFFFF; // sign-extend immediate
    return "bne $" + to_string(s) + ", $" + to_string(t) + ", " + to_string(i);
  } else if (opcode == 35) { // lw
    int s = (word >> 21) & 0x1F;
    int t = (word >> 16) & 0x1F;
    int16_t i = word & 0xFFFF; // sign-extend immediate
    return "lw $" + to_string(t) + ", " + to_string(i) + "($" + to_string(s) + ")";
  } else if (opcode == 43) { // sw
    int s = (word >> 21) & 0x1F;
    int t = (word >> 16) & 0x1F;
    int16_t i = word & 0xFFFF; // sign-extend immediate
    return "sw $" + to_string(t) + ", " + to_string(i) + "($" + to_string(s) + ")";
  } else if (opcode == 2) { // j
    uint32_t address = word & 0x3FFFFFF;
    return "j 0x" + to_string(address << 2);
  } else if (opcode == 3) { // jal
    uint32_t address = word & 0x3FFFFFF;
    return "jal 0x" + to_string(address << 2);
  } else {
    return "unknown (opcode=" + to_string(opcode) + ")";
  }
}

// Main function to test the assembler
vector<word> main_asm(std::string input_asm_contents) {
  // Process the input through the assembler
  vector<word> result = to_merl(input_asm_contents);
  
  return result;
}
};




vector<word> asmAssemble(string asm_input = "") {
  // Read assembly input from stdin
  
  string line;


  // Compile Assembly to Binary
  vector<word> binary_output = asm_compiler::main_asm(asm_input);
  
  // Convert binary output to char array for decoder
  vector<char> binary_data;
  for (word w : binary_output) {
    // Convert word to bytes in big-endian format
    binary_data.push_back((w >> 24) & 0xFF);
    binary_data.push_back((w >> 16) & 0xFF);
    binary_data.push_back((w >> 8) & 0xFF);
    binary_data.push_back(w & 0xFF);
  }
  
  // Get analysis data
  asm_compiler::AnalysisData analysis = asm_compiler::get_analysis_data(asm_input);
  
  // Decode and print instructions using cerr
  cerr << "=== ASSEMBLY ANALYSIS ===" << endl;
  cerr << "Binary data size: " << binary_data.size() << " bytes" << endl;
  
  // Print Symbol Table
  cerr << endl << "=== SYMBOL TABLE ===" << endl;
  cerr << "Label Name          | Address" << endl;
  cerr << "--------------------|--------" << endl;
  for (const auto& entry : analysis.symbolTable) {
    cerr << left << setw(19) << entry.first << "| 0x" << hex << setw(6) << entry.second << dec << endl;
  }
  
  // Print Import/Export Labels
  cerr << endl << "=== IMPORT/EXPORT LABELS ===" << endl;
  cerr << "Import Labels: ";
  for (const auto& label : analysis.import_labels) {
    cerr << label << " ";
  }
  cerr << endl;
  cerr << "Export Labels: ";
  for (const auto& label : analysis.export_labels) {
    cerr << label << " ";
  }
  cerr << endl;
  
  // Print Branch Reference Map
  cerr << endl << "=== BRANCH REFERENCE MAP ===" << endl;
  cerr << "Label Name          | PC Addresses" << endl;
  cerr << "--------------------|-------------" << endl;
  for (const auto& entry : analysis.branch_reference_map) {
    cerr << left << setw(19) << entry.first << "| ";
    for (uint32_t pc : entry.second) {
      cerr << "0x" << hex << pc << " ";
    }
    cerr << dec << endl;
  }
  
  // Print Label PC Map
  cerr << endl << "=== LABEL PC MAP ===" << endl;
  cerr << "Label Name          | PC Addresses" << endl;
  cerr << "--------------------|-------------" << endl;
  for (const auto& entry : analysis.label_pc_map) {
    cerr << left << setw(19) << entry.first << "| ";
    for (uint32_t pc : entry.second) {
      cerr << "0x" << hex << pc << " ";
    }
    cerr << dec << endl;
  }
  
  // Print ESD (External Symbol Dictionary) and ERL (External Reference List)
  if (!analysis.import_labels.empty() || !analysis.export_labels.empty()) {
    cerr << endl << "=== ESD (External Symbol Dictionary) ===" << endl;
    cerr << "Type | Address | Label Name" << endl;
    cerr << "-----|---------|-----------" << endl;
    
    // ESD entries for exports
    for (const auto& label : analysis.export_labels) {
      if (analysis.symbolTable.count(label)) {
        cerr << "ESD  | 0x" << hex << setw(6) << analysis.symbolTable[label] << dec << " | " << label << endl;
      }
    }
    
    cerr << endl << "=== ERL (External Reference List) ===" << endl;
    cerr << "Type | Address | Label Name" << endl;
    cerr << "-----|---------|-----------" << endl;
    
    // ERL entries for imports
    for (const auto& entry : analysis.label_pc_map) {
      if (analysis.import_labels.count(entry.first)) {
        for (uint32_t pc : entry.second) {
          cerr << "ERL  | 0x" << hex << setw(6) << pc << dec << " | " << entry.first << endl;
        }
      }
    }
    
    cerr << endl << "=== RELOCATION ENTRIES ===" << endl;
    cerr << "Type | Address" << endl;
    cerr << "-----|--------" << endl;
    
    // Relocation entries for internal references
    for (const auto& entry : analysis.label_pc_map) {
      if (!analysis.import_labels.count(entry.first)) {
        for (uint32_t pc : entry.second) {
          cerr << "REL  | 0x" << hex << setw(6) << pc << dec << endl;
        }
      }
    }
  }
  
  // Check if this is MERL format
  bool is_merl = false;
  if (binary_data.size() >= 4) {
    uint32_t cookie = ((unsigned char)binary_data[0] << 24) |
                      ((unsigned char)binary_data[1] << 16) |
                      ((unsigned char)binary_data[2] << 8) |
                      (unsigned char)binary_data[3];
    is_merl = (cookie == 0x10000002);
  }
  
  if (is_merl) {
    cerr << "MERL Format Detected" << endl;
    // Parse MERL header
    uint32_t end_of_file = ((unsigned char)binary_data[4] << 24) |
                           ((unsigned char)binary_data[5] << 16) |
                           ((unsigned char)binary_data[6] << 8) |
                           (unsigned char)binary_data[7];
    uint32_t end_of_code = ((unsigned char)binary_data[8] << 24) |
                           ((unsigned char)binary_data[9] << 16) |
                           ((unsigned char)binary_data[10] << 8) |
                           (unsigned char)binary_data[11];
    
    cerr << "MERL Header:" << endl;
    cerr << "  Cookie: 0x10000002" << endl;
    cerr << "  End of file: " << end_of_file << endl;
    cerr << "  End of code: " << end_of_code << endl;
    cerr << endl;
    
    // Decode instructions (skip header, decode until end_of_code)
    int pos = 12; // Skip MERL header
    bool previous_was_lis = false;
    int instruction_count = 0;
    
    while (pos < (int)end_of_code && pos + INSTRUCTION_SIZE <= (int)binary_data.size()) {
      uint32_t word = ((unsigned char)binary_data[pos] << 24) |
                      ((unsigned char)binary_data[pos + 1] << 16) |
                      ((unsigned char)binary_data[pos + 2] << 8) |
                      (unsigned char)binary_data[pos + 3];
      
      string decoded;
      if (previous_was_lis) {
        decoded = ".word";
        previous_was_lis = false;
      } else {
        decoded = asm_compiler::decode_instruction(word);
        uint32_t opcode = word >> 26;
        if (opcode == 0) {
          uint32_t function_code = word & 0x3F;
          if (function_code == 20) { // lis
            previous_was_lis = true;
          }
        }
      }
      
      cerr << "Instruction " << instruction_count << ": 0x" << hex << word << dec << " (" << decoded << ")" << endl;
      
      pos += INSTRUCTION_SIZE;
      instruction_count++;
    }
    
    // Decode relocation entries if any
    if (pos < (int)binary_data.size()) {
      cerr << endl << "Relocation Entries:" << endl;
      int entry_count = 0;
      while (pos + 4 <= (int)binary_data.size()) {
        uint32_t entry = ((unsigned char)binary_data[pos] << 24) |
                        ((unsigned char)binary_data[pos + 1] << 16) |
                        ((unsigned char)binary_data[pos + 2] << 8) |
                        (unsigned char)binary_data[pos + 3];
        
        cerr << "Entry " << entry_count << ": 0x" << hex << entry << dec << endl;
        pos += 4;
        entry_count++;
      }
    }
  } else {
    cerr << "Binary Format Detected" << endl;
    // Decode as plain binary
    int pos = 0;
    bool previous_was_lis = false;
    int instruction_count = 0;
    
    while (pos < (int)binary_data.size()) {
      if (pos + INSTRUCTION_SIZE <= (int)binary_data.size()) {
        uint32_t word = ((unsigned char)binary_data[pos] << 24) |
                        ((unsigned char)binary_data[pos + 1] << 16) |
                        ((unsigned char)binary_data[pos + 2] << 8) |
                        (unsigned char)binary_data[pos + 3];
        
        string decoded;
        if (previous_was_lis) {
          decoded = ".word";
          previous_was_lis = false;
        } else {
          decoded = asm_compiler::decode_instruction(word);
          uint32_t opcode = word >> 26;
          if (opcode == 0) {
            uint32_t function_code = word & 0x3F;
            if (function_code == 20) { // lis
              previous_was_lis = true;
            }
          }
        }
        
        cerr << "Instruction " << instruction_count << ": 0x" << hex << word << dec << " (" << decoded << ")" << endl;
        
        pos += INSTRUCTION_SIZE;
        instruction_count++;
      } else {
        break;
      }
    }
  }
  

  return binary_output;
}


