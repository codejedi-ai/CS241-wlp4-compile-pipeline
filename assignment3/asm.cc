#include <iostream>
#include <string>
#include <vector>
#include "scanner.h"
#include <map>
#include <algorithm>
using namespace std;

void coutword(int a)
{
  int instr = a;
  unsigned char c = instr;
  cout << c;
}
void printInstruction(int instr)
{
  unsigned char c = instr >> 24;
  cout << c;
  c = instr >> 16;
  cout << c;
  c = instr >> 8;
  cout << c;
  c = instr;
  cout << c;
}
void coutmult(int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 24; // add $3, $2, $4
  printInstruction(instr);
}
void coutmultu(int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 25; // add $3, $2, $4
  printInstruction(instr);
}
void coutdiv(int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 26; // add $3, $2, $4
  printInstruction(instr);
}
void coutdivu(int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | 27; // add $3, $2, $4
  printInstruction(instr);
}
void coutAdd(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 32; // add $3, $2, $4
  printInstruction(instr);
}
void coutSub(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 34; // add $3, $2, $4
  printInstruction(instr);
}
void coutSlt(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 42; // add $3, $2, $4
  printInstruction(instr);
}
void coutSltu(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 43; // add $3, $2, $4
  printInstruction(instr);
}
void coutBeq(int s, int t, int i)
{
  i = i & (0xffff);
  int instr = (4 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  printInstruction(instr);
}
void coutBne(int s, int t, int i)
{
  i = i & (0xffff);
  int instr = (5 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  printInstruction(instr);
}

void jr(int s)
{
  int instr = (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 8; // add $3, $2, $4
  printInstruction(instr);
}
void jalr(int s)
{
  int instr = (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 9; // add $3, $2, $4
  printInstruction(instr);
}
void mfhi(int d)
{
  int instr = (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 16; // add $3, $2, $4
  printInstruction(instr);
}
void mflo(int d)
{
  int instr = (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 18; // add $3, $2, $4
  printInstruction(instr);
}
void lis(int d)
{
  int instr = (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | 20; // add $3, $2, $4
  printInstruction(instr);
}
void coutSw(int t, int i, int s)
{
  i = i & (0xffff);
  int instr = (43 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  printInstruction(instr);
}
void coutLw(int t, int i, int s)
{
  i = i & (0xffff);
  int instr = (35 << 26) | (s << 21) | (t << 16) | i; // add $3, $2, $4
  printInstruction(instr);
}
/*
add $1, $2, $3
Token(ID, add) Token(REG, $1) Token(COMMA, ,) Token(REG, $2) Token(COMMA, ,) Token(REG, $3)
*/
bool add_sub_slt_sltu(int ind, std::vector<Token> &vecref)
{

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
  bool instructionValid = vecref[ind].getLexeme() == "add" ||
                          vecref[ind].getLexeme() == "sub" ||
                          vecref[ind].getLexeme() == "slt" ||
                          vecref[ind].getLexeme() == "sltu";
  if (!instructionValid)
    return false;
  return true;
}
bool mult_multu_div_divu(int ind, std::vector<Token> &vecref)
{
  vector<Token> buf = scan("mult $4, $3");
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = true;

  for (int i = 0; i < buf.size(); i++)
  {
    structureMeet = structureMeet && (vecref[ind + i].getKind() == buf[i].getKind());
  }

  if (!structureMeet)
    return false;
  bool instructionValid = vecref[ind].getLexeme() == "mult" ||
                          vecref[ind].getLexeme() == "multu" ||
                          vecref[ind].getLexeme() == "div" ||
                          vecref[ind].getLexeme() == "divu";
  if (!instructionValid)
    return false;
  return true;
}
// Token(ID, lis) Token(REG, $4)
bool mfhi_mflo_lis(int ind, std::vector<Token> &vecref)
{

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
// Token(ID, lw) Token(REG, $4) Token(COMMA, ,) Token(INT, 1) Token(LPAREN, () Token(REG, $3) Token(RPAREN, ))
bool lw_sw(int ind, std::vector<Token> &vecref)
{
  vector<Token> buf = scan("lw $4, 1($3)");
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;
  buf = scan("lw $4, 0x1($3)");
  bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;
  if (!structureMeet)
    return false;
  bool instructionValid = vecref[ind].getLexeme() == "lw" ||
                          vecref[ind].getLexeme() == "sw";
  if (!instructionValid)
    return false;
  return true;
}
bool beq_bne(int ind, std::vector<Token> &vecref)
{
  vector<Token> buf = scan("beq $4, $0, i");
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  buf = scan("beq $4, $0, 0x0");
  bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  buf = scan("beq $4, $0, 1");
  bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  if (!structureMeet)
    return false;

  bool instructionValid = vecref[ind].getLexeme() == "beq" ||
                          vecref[ind].getLexeme() == "bne";
  if (!instructionValid)
    return false;
  return true;
}
bool jr_jalr(int ind, std::vector<Token> &vecref)
{

  bool sizeMeet = (ind + 2 == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = vecref[ind].getKind() == Token::ID &&
                       vecref[ind + 1].getKind() == Token::REG;
  if (!structureMeet)
    return false;
  bool instructionValid = vecref[ind].getLexeme() == "jr" ||
                          vecref[ind].getLexeme() == "jalr";
  if (!instructionValid)
    return false;
  return true;
}

bool word(int ind, std::vector<Token> &vecref)
{
  vector<Token> buf = scan(".word i");
  bool sizeMeet = (ind + buf.size() == vecref.size());
  if (!sizeMeet)
    return false;
  bool structureMeet = false;
  bool bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  buf = scan(".word 0x0");
  bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
    bufTemp = bufTemp && (vecref[ind + i].getKind() == buf[i].getKind());
  }
  structureMeet = structureMeet || bufTemp;

  buf = scan(".word 1");
  bufTemp = true;
  for (int i = 0; i < buf.size(); i++)
  {
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
int main()
{
  std::string line;
  std::vector<std::vector<Token>> assemblyProgram;
  vector<string> instructions{"add", "sub", "mult", "multu", "div", "divu", "mfhi", "mflo", "lis", "lw", "sw", "slt", "sltu", "beq", "bne", "jr", "jalr"};
  std::map<std::string, int> symbolTable;
  try
  {
    while (getline(std::cin, line))
    {
      // This example code just prints the scanned tokens on each line.
      assemblyProgram.push_back(scan(line));
    }
  }
  catch (ScanningFailure &f)
  {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.

  //__________________________________

  int pc = 0;
  for (std::vector<Token> line : assemblyProgram)
  {
    if (line.empty())
      continue;

    long unsigned int ind = 0;
    // I CANNOT USE LOOPS OR ELSE I WOULD GET ERROR
    /*
    Not using the while loop I cannot have infinite lables infront of the instruction

    */
    while (ind < line.size() && line[ind].getKind() == Token::LABEL)
    {
      string buf = line[ind].getLexeme();
      buf.resize(buf.size() - 1);
      if (symbolTable.count(buf))
      {
        std::cerr << "ERROR: Duplicate Labels" << std::endl;
        return 1;
      }
      symbolTable[buf] = pc;
      ind++;
    }
    if (ind == line.size())
      continue;
    // at this point the following MUST be valid instructions if not then the code is invalid
    bool found = lw_sw(ind, line) || mult_multu_div_divu(ind, line) || mfhi_mflo_lis(ind, line) || beq_bne(ind, line) || word(ind, line) || jr_jalr(ind, line) || add_sub_slt_sltu(ind, line);

    if (!found)
    {
      // return error
      std::cerr << "ERROR: Invalid instruction or parameters" << std::endl;
      return 1;
    }
    else
    {
      pc += 4;
    }
  }

  // ------------------------------------------------------------------------------------------------------------------
  pc = 0;
  for (std::vector<Token> line : assemblyProgram)
  {
    if (line.empty())
      continue;
    int ind = 0;
    while (ind < line.size() && line[ind].getKind() == Token::LABEL)
    {
      ind++;
    }
    // token is not a label
    if (ind == line.size())
      continue;
    int64_t instr = 0;
    if (ind < line.size() && line[ind].getKind() == Token::WORD)
    {
      pc += 4;
      ind++;
      string str = line[ind].getLexeme();
      if (line[ind].getKind() == Token::ID)
      {
        if (symbolTable.count(str) == 0)
        {
          std::cerr << "ERROR: Invalid Lable :" << str << std::endl;
          return 1;
        }
        instr = symbolTable[str];
      }
      else
      {
        instr = line[ind].toNumber();
      }
      /*
      if (line[ind].getKind() == Token::INT && instr < 0)
      {

          std::cerr << "ERROR: No negative numbers for .word " << str << std::endl;
          return 1;
      }
      */
      printInstruction(instr);
    }
    else if (line[ind].getKind() == Token::ID)
    {
      pc += 4;
      if (line[ind].getLexeme() == "jr")
      {
        instr = line[ind + 1].toNumber();
        jr(instr);
      }
      if (line[ind].getLexeme() == "jalr")
      {
        instr = line[ind + 1].toNumber();
        jalr(instr);
      }
      if (line[ind].getLexeme() == "add")
      {
        int d = line[ind + 1].toNumber();
        int s = line[ind + 3].toNumber();
        int t = line[ind + 5].toNumber();
        coutAdd(d, s, t);
      }
      if (line[ind].getLexeme() == "sub")
      {
        int d = line[ind + 1].toNumber();
        int s = line[ind + 3].toNumber();
        int t = line[ind + 5].toNumber();
        coutSub(d, s, t);
      }
      if (line[ind].getLexeme() == "slt")
      {
        int d = line[ind + 1].toNumber();
        int s = line[ind + 3].toNumber();
        int t = line[ind + 5].toNumber();
        coutSlt(d, s, t);
      }
      if (line[ind].getLexeme() == "sltu")
      {
        int d = line[ind + 1].toNumber();
        int s = line[ind + 3].toNumber();
        int t = line[ind + 5].toNumber();
        coutSltu(d, s, t);
      }

      // Question 6
      if (line[ind].getLexeme() == "beq")
      {
        int s = line[ind + 1].toNumber();
        int t = line[ind + 3].toNumber();
        int64_t i = 0;
        if (line[ind + 5].getKind() == Token::ID)
        {
          if (symbolTable.count(line[ind + 5].getLexeme()) == 0)
          {
            std::cerr << "ERROR: " << line[ind + 5].getLexeme() << " is an invalid token" << std::endl;
            return 1;
          }
          i = symbolTable[line[ind + 5].getLexeme()] - pc;
          i = i / 4;
        }
        else
        {
          i = line[ind + 5].toNumber();
          if ((line[ind + 5].getKind() == Token::INT) && !(-32768 <= i && i <= 32767))
          {
            std::cerr << "ERROR: Step count out of range. must be -32768 <= i <= 32767" << std::endl;
            return 1;
          }
          if ((line[ind + 5].getKind() == Token::HEXINT) && i > 0xffff)
          {
            std::cerr << "ERROR: Step count out of range. must be i <= 0xffff" << std::endl;
            return 1;
          }
        }
        coutBeq(s, t, i);
      }
      if (line[ind].getLexeme() == "bne")
      {
        int s = line[ind + 1].toNumber();
        int t = line[ind + 3].toNumber();
        int64_t i = 0;
        if (line[ind + 5].getKind() == Token::ID)
        {
          if (symbolTable.count(line[ind + 5].getLexeme()) == 0)
          {
            std::cerr << "ERROR: " << line[ind + 5].getLexeme() << " is an invalid token" << std::endl;
            return 1;
          }
          i = symbolTable[line[ind + 5].getLexeme()] - pc;
          i = i / 4;
        }
        else
        {
          i = line[ind + 5].toNumber();
          if ((line[ind + 5].getKind() == Token::INT) && !(-32768 <= i && i <= 32767))
          {
            std::cerr << "ERROR: Step count out of range. must be -32768 <= i <= 32767" << std::endl;
            return 1;
          }
          if ((line[ind + 5].getKind() == Token::HEXINT) && i > 0xffff)
          {
            std::cerr << "ERROR: Step count out of range. must be i <= 0xffff" << std::endl;
            return 1;
          }
        }
        coutBne(s, t, i);
      }
      // Problem 7 ughhh completed?? Question 8
      if (line[ind].getLexeme() == "lis")
      {
        instr = line[ind + 1].toNumber();
        lis(instr);
      }
      if (line[ind].getLexeme() == "mflo")
      {
        instr = line[ind + 1].toNumber();
        mflo(instr);
      }
      if (line[ind].getLexeme() == "mfhi")
      {
        instr = line[ind + 1].toNumber();
        mfhi(instr);
      }
    }
    // Question 9
    if (line[ind].getLexeme() == "mult")
    {
      int s = line[ind + 1].toNumber();
      int t = line[ind + 3].toNumber();
      coutmult(s, t);
    }
    if (line[ind].getLexeme() == "multu")
    {
      int s = line[ind + 1].toNumber();
      int t = line[ind + 3].toNumber();
      coutmultu(s, t);
    }
    if (line[ind].getLexeme() == "div")
    {
      int s = line[ind + 1].toNumber();
      int t = line[ind + 3].toNumber();
      coutdiv(s, t);
    }
    if (line[ind].getLexeme() == "divu")
    {
      int s = line[ind + 1].toNumber();
      int t = line[ind + 3].toNumber();
      coutdivu(s, t);
    }
    // Problem 10
    if (line[ind].getLexeme() == "sw")
    {
      int t = line[ind + 1].toNumber();
      int i = line[ind + 3].toNumber();
      int s = line[ind + 5].toNumber();
      coutSw(t, i, s);
    }
    if (line[ind].getLexeme() == "lw")
    {
      int t = line[ind + 1].toNumber();
      int i = line[ind + 3].toNumber();
      int s = line[ind + 5].toNumber();
      coutLw(t, i, s);
    }
  }

  for (auto const &x : symbolTable)
  {
    std::cerr << x.first << " " << x.second << endl;
  }
  return 0;
}
