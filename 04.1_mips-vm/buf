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
void coutAdd(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 32; // add $3, $2, $4
  printInstruction(instr);
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
      std::vector<Token> tokenLine = scan(line);
      assemblyProgram.push_back(tokenLine);
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

  int pc = 0;
  for (auto &line : assemblyProgram)
  {
    if (line.empty())
      continue;
    int ind = 0;
    auto &token = line[ind];
    while (token.getKind() == Token::LABEL)
    {
      std::cout << token << ' ';
      string buf = token.getLexeme();
      buf.resize(buf.size() - 1);
      symbolTable[buf] = pc;
      ind++;
      token = line[ind];
    }
    std::cout << std::endl;
    // token is not a label
    if (token.getKind() == Token::WORD)
    {
      pc++;
      token = line[ind];
      if (line[1].getKind() == Token::INT || line[1].getKind() == Token::HEXINT)
      {
        int64_t instr = line[1].toNumber();
        if (!(INT32_MIN <= instr && instr <= UINT32_MAX))
        {
          std::cerr << "ERROR: integer out of bounds" << std::endl;
          return 1;
        }
      }
    }
    if (token.getKind() == Token::ID)
    {
      string str = token.getLexeme();
      bool found = find(instructions.begin(), instructions.end(), str) != instructions.end();
      if (!found)
      {
        // return error
        std::cerr << "ERROR: Invalid instruction:" << str << std::endl;
        return 1;
      }
    }
  }

  return 0;
}














#include <iostream>
#include <string>
#include <vector>
#include "scanner.h"
#include "limits.h"
#include <map>
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
void coutAdd(int d, int s, int t)
{
  int instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | 32; // add $3, $2, $4
  printInstruction(instr);
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
  std::vector<std::vector<Token>> TokenizedInput;
  std::string line;
  std::map<string, int> symbolTable;
  try
  {
    while (getline(std::cin, line))
    {
      // This example code just prints the scanned tokens on each line.
      std::vector<Token> tokenLine = scan(line);

      // This code is just an example - you don't have to use a range-based
      // for loop in your actual assembler. You might find it easier to
      // use an index-based loop like this:
      // for(int i=0; i<tokenLine.size(); i++) { ... }
      TokenizedInput.push_back(tokenLine);
      
      // Remove this when you're done playing with the example program!
      // Printing a random newline character as part of your machine code
      // output will cause you to fail the Marmoset tests.
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
  int pc = 0;
  for (auto &line : TokenizedInput)
  {
    
    int ind = 0;
    if (line.empty())
      continue;
    auto &token = line[ind];

    if (token.getKind() == Token::WORD || token.getKind() == Token::ID)
    {
      pc += 4;
    }
    while (token.getKind() == Token::LABEL)
    {
      string tokenName = token.getLexeme();
      tokenName.resize(tokenName.size() - 1);
      symbolTable[tokenName] = pc;
      ind++;
      token = line[ind];
    }
  }
  pc = 0;
  for (auto &line : TokenizedInput)
  {
    
    if (line.empty())
      continue;

            for (auto &token : line) {
        std::cout << token << ' ';
      }


      /*
    int ind = 0;
    auto &token = line[ind];
    while (token.getKind() == Token::LABEL)
    {
      std::cout << token << ' ';
      ind++;
      token = line[ind];
    }
    
  */
 cout << endl;
    /*
    switch (token.getKind())
    {
    case Token::ID:
      // FOr instructions
      break;
    case Token::LABEL:
      break;
    case Token::WORD:
      pc += 4;
      ind++;
      if (line[ind].getKind() == Token::INT || line[ind].getKind() == Token::HEXINT)
      {
        int64_t instr = line[ind].toNumber();
        if (INT32_MIN <= instr && instr <= UINT32_MAX)
          printInstruction(instr);
        else
        {
          std::cout << token << ' ';
          std::cerr << "ERROR: integer out of bounds" << std::endl;
          return 1;
        }
      }
      else
      {
        std::cout << line[ind] << ' ';
        std::cerr << "ERROR: Expeceted integer after .word" << std::endl;
        return 1;
      }

      ind++;
      if (line.size() != ind)
      {
        std::cerr << "ERROR: Too Much Params after .word" << std::endl;
        return 1;
      }

      break;
    case Token::COMMA:
      break;
    case Token::LPAREN:
      break;
    case Token::RPAREN:
      break;
    case Token::INT:
      break;
    case Token::HEXINT:
      break;
    case Token::REG:
      break;
    case Token::WHITESPACE:
      break;
    case Token::COMMENT:
      break;
    }
    */
  }
  for(auto const& x : symbolTable){
    std::cerr  << x.first << " " << x.second << endl;
  }
  return 0;
}
