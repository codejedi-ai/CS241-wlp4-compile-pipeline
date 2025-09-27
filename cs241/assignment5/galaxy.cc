#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
// Skip the grammar part of the input.
void skipLine(istream &in)
{
  string s;
  getline(in, s);
}

void skipGrammar(istream &in)
{
  int i, numTerm, numNonTerm, numRules;

  // read the number of terminals and move to the next line
  in >> numTerm;
  skipLine(in);

  // skip the lines containing the terminals
  for (i = 0; i < numTerm; i++)
  {
    skipLine(in);
  }

  // read the number of non-terminals and move to the next line
  in >> numNonTerm;
  skipLine(in);

  // skip the lines containing the non-terminals
  for (i = 0; i < numNonTerm; i++)
  {
    skipLine(in);
  }

  // skip the line containing the start symbol
  skipLine(in);

  // read the number of rules and move to the next line
  in >> numRules;
  skipLine(in);

  // skip the lines containing the production rules
  for (i = 0; i < numRules; i++)
  {
    skipLine(in);
  }
}

string trim(const string &str)
{
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos)
    return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}
// T should keep curState while a should keep the transaction rules
int evalTree(string t, queue<string> &a)
{
  string transaction_rule = a.front();
  a.pop();
  if (t == "S")
  {
    // starting at S suggests only evaluate at eval thus need to check transaction rule
    return evalTree("expr", a);
  }
  else if (t == "expr")
  {
    if (transaction_rule == "expr expr - term")
    {
      int firstVal = evalTree("expr", a); 
      int secondVal = evalTree("term", a);
      return firstVal - secondVal;
    }
    if (transaction_rule == "expr term")
    {
      return evalTree("term",a);
    }
  }
  else if (t == "term")
  {
    if (transaction_rule == "term id")
    {
      return 42;
    }
    if (transaction_rule == "term ( expr )")
    {
      return evalTree("expr",a);
    }
  }
  return -1;
}
// Prints the derivation with whitespace trimmed.
int printDerivation(istream &in, ostream &out)
{
  string line;
  queue<string> buildStack;
  while (getline(in, line))
  {
    string f = trim(line);
    buildStack.push(f);
  }
  return evalTree("S", buildStack);
}
  // Reads a .cfg file and prints the derivation
  // without leading or trailing spaces.
  int main()
  {
    skipGrammar(std::cin);
    cout << printDerivation(std::cin, std::cout) << endl;
  }