#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <queue>
using namespace std;
// Skip the grammar part of the input.

struct transitionRule
{
    string non_terminal;
    string instruction;
    int number_of_popped_symbols;
    vector<string> tokens;
};
vector<transitionRule> tokens;
struct LL1_DFA_NODE
{
    int curState;
    string symbol, type_1;
    int int1;
    LL1_DFA_NODE(int a, string b, string c, int d) : curState(a), symbol(b), type_1(c), int1(d) {}
};
map<string, int> map_symbol_to_integer;
vector<transitionRule> productionRules;

string skipLine(istream &cin)
{
    string s;
    getline(cin, s);
    return s;
}
/*
After reading cin the CFG
An integer s -- the number of states cin the LR(1) DFA. States are numbered 0 to s-1. State 0 is the start state.
An integer t -- the number of shift and reduce actions encoded cin the LR(1) DFA.
t lines, each containing one of:
state terminal reduce rule (Rules (productions) are numbered from 0 cin the order they appear cin the CFG.)
state1 symbol shift state2 (where symbol is either a terminal or a non-terminal)
*/
string trim(const string &str)
{
    size_t begin = str.find_first_not_of(" \t\n");
    if (begin == string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\n");

    return str.substr(begin, end - begin + 1);
}
// T should keep curState while a should keep the transaction rules

// Reads a .cfg file and prints the derivation
// without leading or trailing spaces.
int Reduce(int top, string next, vector<LL1_DFA_NODE> &LL1_DFA, int t)
{
    for (int i = 0; i < t; i++)
    {
        if (LL1_DFA[i].curState == top && LL1_DFA[i].symbol == next && LL1_DFA[i].type_1 == "reduce")
        {
            return LL1_DFA[i].int1;
        }
    }
    return -1;
}
int delta(int top, string next, vector<LL1_DFA_NODE> &LL1_DFA, int t)
{
    for (int i = 0; i < t; i++)
    {
        if (LL1_DFA[i].curState == top && LL1_DFA[i].symbol == next && LL1_DFA[i].type_1 == "shift")
        {
            return LL1_DFA[i].int1;
        }
    }
    return -1;
}
int main()
{

    int i, numTerm, numNonTerm, numRules, s, t;

    // read the number of terminals and move to the next line
    cin >> numTerm;
    skipLine(cin);

    // skip the lines containing the terminals
    int f = 0;
    for (i = 0; i < numTerm; i++)
    {
        string buf = skipLine(cin);
        map_symbol_to_integer[buf] = f++;
    }

    // read the number of non-terminals and move to the next line
    cin >> numNonTerm;
    skipLine(cin);

    // skip the lines containing the non-terminals
    for (i = 0; i < numNonTerm; i++)
    {
        string buf = skipLine(cin);
        map_symbol_to_integer[buf] = f++;
    }

    // skip the line containing the start symbol
    string start = skipLine(cin);

    // read the number of rules and move to the next line
    cin >> numRules;
    skipLine(cin);

    // skip the lines containing the production rules
    for (i = 0; i < numRules; i++)
    {
        transitionRule curRule;
        string lineStr = skipLine(cin);
        curRule.instruction = lineStr;
        std::stringstream line(lineStr);
        line >> curRule.non_terminal;
        curRule.number_of_popped_symbols = 0;
        while (line >> lineStr)
        {
            curRule.number_of_popped_symbols++;
            curRule.tokens.push_back(lineStr);
        }
        productionRules.push_back(curRule);
    }
    cin >> s;
    skipLine(cin);
    cin >> t;
    skipLine(cin);

    /*
     Need to make an array of vectors of size s then add the rules for each states
     When reading cin the cin string it would do it string by string and keep track of the states cin a queue
     There would be a queue for the states and anouther queue for the symbols
     or there can just be one queue and pop and accept nilly willyny
     I want to make a 2d array, dynamic 2d array cin c++ is hard
    */
    vector<LL1_DFA_NODE> LL1_DFA;
    for (int i = 0; i < t; i++)
    {
        int curState, int1;
        string symbol, type_1;
        cin >> curState >> symbol >> type_1 >> int1;
        LL1_DFA.push_back({curState, symbol, type_1, int1});
    }
    stack<int> stateStack; // the current state is ALWAYS the first element in the integer stack
    stateStack.push(0);
    stack<string> symbolStack; // this is the stack of all the read symbols
    string lookforward;
    int readIn = 0;

    while (true)
    {
        if (!(cin >> lookforward))
        { // Remember, operator! === fail()
            if (cin.eof())
                break;
            cin.clear();
            cin.ignore();
        }
        // cout << "Currently at " << lookforward << endl;
        //  Ask is it a reduce state
        while (Reduce(stateStack.top(), lookforward, LL1_DFA, t) != -1)
        {
            // cout << "State: " << stateStack.top() << " is reduce state" << endl;
            int rule = Reduce(stateStack.top(), lookforward, LL1_DFA, t);
            string B = productionRules[rule].non_terminal;
            for (int foo = 0; foo < productionRules[rule].number_of_popped_symbols; foo++)
            {
                symbolStack.pop();
                stateStack.pop();
            }
            cout << productionRules[rule].instruction << endl;
            symbolStack.push(B);
            stateStack.push(delta(stateStack.top(), B, LL1_DFA, t));
        }
        symbolStack.push(lookforward);
        int nextState = delta(stateStack.top(), lookforward, LL1_DFA, t);
        if (nextState == -1)
        {
            // error message
            cerr << "ERROR at " << readIn + 1 << endl;
            return 1;
        }
        stateStack.push(nextState);
        readIn++;
    }
    for (i = 0; i < numRules; i++)
    {
        if (productionRules[i].non_terminal == start)
        {
            cout << productionRules[i].instruction;
        }
    }
}
