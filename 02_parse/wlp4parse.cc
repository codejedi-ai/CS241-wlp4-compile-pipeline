#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <fstream>
#include <list>
using namespace std;
// Skip the grammar part of the input.

struct transitionRule
{
    string non_terminal;
    string instruction;
    int number_of_popped_symbols;
    vector<string> tokens;
};
struct parseTree
{
    string element;
    list<parseTree *> childrens;
    parseTree(string a) : element(a) {}
    void insertNode(parseTree *buf)
    {
        childrens.push_back(buf);
    }
    ~parseTree()
    {
        for (parseTree *a : childrens)
        {
            delete a;
        }
    }
    void printParseTree()
    {
        cout << element << endl;
        for (parseTree *a : childrens)
        {
            a->printParseTree();
        }
    }
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

string skipLine(istream &infile)
{
    string s;
    getline(infile, s);
    return s;
}
/*
After reading infile the CFG
An integer s -- the number of states infile the LR(1) DFA. States are numbered 0 to s-1. State 0 is the start state.
An integer t -- the number of shift and reduce actions encoded infile the LR(1) DFA.
t lines, each containing one of:
state terminal reduce rule (Rules (productions) are numbered from 0 infile the order they appear infile the CFG.)
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
    // cout << top << " " << next << endl;
    for (int i = 0; i < t; i++)
    {
        // cout << " " << LL1_DFA[i].curState << " " << LL1_DFA[i].symbol << " " << LL1_DFA[i].type_1 << " " <<LL1_DFA[i].int1 << endl;
        if (LL1_DFA[i].curState == top && LL1_DFA[i].symbol == next && LL1_DFA[i].type_1 == "shift")
        {
            return LL1_DFA[i].int1;
        }
    }
    return -1;
}

// Enhanced error reporting from DFA algorithm perspective
void reportDFAError(int tokenNumber, const string& token, const string& lexeme, 
                   int currentState, const vector<LL1_DFA_NODE>& LL1_DFA, 
                   const vector<transitionRule>& productionRules, int t) {
    
    cerr << "ERROR at token " << tokenNumber << ": ";
    cerr << "Unexpected token '" << token << "'";
    if (!lexeme.empty() && lexeme != token) {
        cerr << " ('" << lexeme << "')";
    }
    cerr << endl;
    
    cerr << "DFA Algorithm Analysis:" << endl;
    cerr << "  Current DFA State: " << currentState << endl;
    cerr << "  Input Token: '" << token << "'" << endl;
    
    // Find all valid transitions from current state
    cerr << "  Valid transitions from state " << currentState << ":" << endl;
    bool foundValidTransitions = false;
    for (int i = 0; i < t; i++) {
        if (LL1_DFA[i].curState == currentState) {
            cerr << "    - On '" << LL1_DFA[i].symbol << "' → ";
            if (LL1_DFA[i].type_1 == "shift") {
                cerr << "SHIFT to state " << LL1_DFA[i].int1;
            } else if (LL1_DFA[i].type_1 == "reduce") {
                cerr << "REDUCE using rule " << LL1_DFA[i].int1;
            }
            cerr << endl;
            foundValidTransitions = true;
        }
    }
    
    if (!foundValidTransitions) {
        cerr << "    - No valid transitions found (dead state)" << endl;
    }
    
    // Explain why the token is invalid
    cerr << "  Algorithm Decision:" << endl;
    cerr << "    The DFA searched through all " << t << " transition rules" << endl;
    cerr << "    looking for a valid transition from state " << currentState << endl;
    cerr << "    on token '" << token << "', but found NONE." << endl;
    
    // Show what the algorithm expected
    cerr << "  What the DFA expected:" << endl;
    vector<string> expectedTokens;
    for (int i = 0; i < t; i++) {
        if (LL1_DFA[i].curState == currentState) {
            expectedTokens.push_back(LL1_DFA[i].symbol);
        }
    }
    
    if (!expectedTokens.empty()) {
        cerr << "    Valid tokens: ";
        for (size_t i = 0; i < expectedTokens.size(); i++) {
            cerr << "'" << expectedTokens[i] << "'";
            if (i < expectedTokens.size() - 1) cerr << ", ";
        }
        cerr << endl;
    } else {
        cerr << "    No valid tokens (parser reached invalid state)" << endl;
    }
    
    // Show grammar rules that could potentially help
    cerr << "  Grammar Rule Analysis:" << endl;
    cerr << "    The parser looked through " << productionRules.size() << " grammar rules" << endl;
    cerr << "    to find a production that could handle token '" << token << "'" << endl;
    
    bool foundRelevantRules = false;
    for (size_t i = 0; i < productionRules.size(); i++) {
        const auto& rule = productionRules[i];
        for (const auto& ruleToken : rule.tokens) {
            if (ruleToken == token) {
                cerr << "    Rule " << i << ": " << rule.non_terminal << " → ";
                for (size_t j = 0; j < rule.tokens.size(); j++) {
                    cerr << rule.tokens[j];
                    if (j < rule.tokens.size() - 1) cerr << " ";
                }
                cerr << endl;
                foundRelevantRules = true;
                break;
            }
        }
    }
    
    if (!foundRelevantRules) {
        cerr << "    No grammar rules contain token '" << token << "'" << endl;
    }
    
    // Provide algorithm-specific explanation
    cerr << "  Why parsing failed:" << endl;
    cerr << "    1. The DFA algorithm maintains a state stack" << endl;
    cerr << "    2. From current state " << currentState << ", it looked for transitions" << endl;
    cerr << "    3. No transition rule matched: (state=" << currentState << ", token='" << token << "')" << endl;
    cerr << "    4. The algorithm cannot proceed without a valid transition" << endl;
    cerr << "    5. This violates the grammar's syntax rules" << endl;
    
    // Suggest fixes based on DFA perspective
    cerr << "  Algorithm suggests:" << endl;
    if (!expectedTokens.empty()) {
        cerr << "    - Replace '" << token << "' with one of the expected tokens" << endl;
        cerr << "    - Check if you're missing a required token before this position" << endl;
    }
    cerr << "    - Verify the grammar allows '" << token << "' in this context" << endl;
    cerr << "    - Check for missing or misplaced tokens in the input" << endl;
}

int main()
{

    int i, numTerm, numNonTerm, numRules, s, t;
    ifstream infile("grammar/WLP4.lr1");
    // read the number of terminals and move to the next line
    infile >> numTerm;
    skipLine(infile);
    // cout << numTerm << endl;
    //  skip the lines containing the terminals
    int f = 0;
    for (i = 0; i < numTerm; i++)
    {
        string buf = skipLine(infile);
        map_symbol_to_integer[buf] = f++;
    }

    // read the number of non-terminals and move to the next line
    infile >> numNonTerm;
    skipLine(infile);
    // cout << numNonTerm << endl;
    //  skip the lines containing the non-terminals
    for (i = 0; i < numNonTerm; i++)
    {
        string buf = skipLine(infile);
        map_symbol_to_integer[buf] = f++;
    }

    // skip the line containing the start symbol
    string start = skipLine(infile);
    // cout << start << endl;
    //  read the number of rules and move to the next line
    infile >> numRules;
    skipLine(infile);
    // cout << numRules << endl;
    //  skip the lines containing the production rules
    for (i = 0; i < numRules; i++)
    {
        transitionRule curRule;
        string lineStr = skipLine(infile);
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
    infile >> s;
    skipLine(infile);

    infile >> t;
    skipLine(infile);
    // cout << s << " " << t << endl;
    /*
     Need to make an array of vectors of size s then add the rules for each states
     When reading infile the infile string it would do it string by string and keep track of the states infile a queue
     There would be a queue for the states and anouther queue for the symbols
     or there can just be one queue and pop and accept nilly willyny
     I want to make a 2d array, dynamic 2d array infile c++ is hard
    */
    vector<LL1_DFA_NODE> LL1_DFA;
    for (int i = 0; i < t; i++)
    {
        int curState, int1;
        string symbol, type_1;
        infile >> curState >> symbol >> type_1 >> int1;
        LL1_DFA.push_back({curState, symbol, type_1, int1});
    }

    stack<int> stateStack; // the current state is ALWAYS the first element in the integer stack
    stateStack.push(0);
    stack<parseTree *> symbolStack; // this is the stack of all the read symbols
    symbolStack.push(new parseTree{"BOF BOF"});
    int nextState = delta(stateStack.top(), "BOF", LL1_DFA, t);
    stateStack.push(nextState);
    int readIn = 0;
    // cout << "Accepting inputs right now" << endl;
    while (true)
    {
        string token, lexeme;
        if (!(cin >> token) || !(cin >> lexeme))
        { // Remember, operator! === fail()
            if (cin.eof())
            {
                token = "EOF";
                lexeme = token;
            }
            cin.clear();
            cin.ignore();
        }

        // cout << "Currently at " << symbolStack.top() << " Next is " << token << endl;
        // cout << "State at " << stateStack.top() << endl;
        //   Ask is it a reduce state
        while (Reduce(stateStack.top(), token, LL1_DFA, t) != -1)
        {
            // cout << "State: " << stateStack.top() << " is reduce state" << endl;
            int rule = Reduce(stateStack.top(), token, LL1_DFA, t);
            string B = productionRules[rule].non_terminal;
            parseTree *addOn = new parseTree{productionRules[rule].instruction};
            for (int foo = 0; foo < productionRules[rule].number_of_popped_symbols; foo++)
            {
                addOn->childrens.push_front(symbolStack.top());
                symbolStack.pop();
                stateStack.pop();
            }

            symbolStack.push(addOn);
            stateStack.push(delta(stateStack.top(), B, LL1_DFA, t));
        }

        symbolStack.push(new parseTree{token + " " + lexeme});
        nextState = delta(stateStack.top(), token, LL1_DFA, t);
        if (nextState == -1)
        {
            // Enhanced DFA error message
            reportDFAError(readIn + 1, token, lexeme, stateStack.top(), LL1_DFA, productionRules, t);
            while (!symbolStack.empty())
            {
                delete symbolStack.top();
                symbolStack.pop();
            }
            return 1;
        }
        stateStack.push(nextState);
        readIn++;
        if (token == "EOF")
        { // Remember, operator! === fail()
            break;
        }
    }
    parseTree *root = new parseTree{start};
    for (i = 0; i < numRules; i++)
    {
        if (productionRules[i].non_terminal == start)
        {
            root->element = productionRules[i].instruction;
        }
    }
    while (!symbolStack.empty())
    {
        root->childrens.push_front(symbolStack.top());
        symbolStack.pop();
    }
    root->printParseTree();
    delete root;
}
