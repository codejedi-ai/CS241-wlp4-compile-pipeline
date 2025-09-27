#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
struct DFAedge
{
    string startNode, tranactionAlphabet, endNode;
};

bool accept(string inString, string initialStartState, vector<DFAedge> transitions, vector<string> accepting){
std::stringstream line(inString);
        // For example, suppose each line contains two strings and an integer
        std::string s1;
        string curState = initialStartState;
        bool nonEmpty = false;
        while (line >> s1)
        {
            bool matched = false;
            for (DFAedge i : transitions)
            {
                if (i.tranactionAlphabet == s1 && i.startNode == curState)
                {
                    matched = true;
                    curState = i.endNode;
                    break;
                }
            }
            if (!matched)
            {
                nonEmpty = true;
                break;
            }
        }
        int mycount = count(accepting.begin(), accepting.end(), curState); // check is it an accepting state
        if (mycount == 0 || nonEmpty)
        {
            cout << "false" << endl;
        }
        else
        {
            cout << "true" << endl;
        }

}
int main()
{
    int n;
    vector<string> alphabet;
    vector<DFAedge> transitions;
    cin >> n;
    string ignore;
    getline(std::cin, ignore);
    for (int i = 0; i < n; i++)
    {
        string lineStr;
        getline(std::cin, lineStr);
        alphabet.push_back(lineStr);
    }
  /*
    cout << "alphabet:" << endl;

    for (string i : alphabet)
        std::cout << i << ',';
    cout << endl;
*/
    vector<string> states;
    cin >> n;
    getline(std::cin, ignore);
    for (int i = 0; i < n; i++)
    {
        string lineStr;
        getline(std::cin, lineStr);
        states.push_back(lineStr);
    }
  /*  cout << "States:" << endl;
    for (string i : states)
        std::cout << i << ',';
    cout << endl;
*/
    string initialStartState;
    getline(std::cin, initialStartState);
    //cout << initialStartState << endl;

    vector<string> accepting;
    cin >> n;
    getline(std::cin, ignore);
    for (int i = 0; i < n; i++)
    {
        string lineStr;
        getline(std::cin, lineStr);
        accepting.push_back(lineStr);
    }/*
    cout << "accepting" << endl;
    for (string i : accepting)
        std::cout << i << ',';
    cout << endl;
*/
    cin >> n;
    getline(std::cin, ignore);
    for (int i = 0; i < n; i++)
    {
        std::string lineStr;
        std::getline(std::cin, lineStr);
        std::stringstream line(lineStr);
        // For example, suppose each line contains two strings and an integer
        std::string s1, s2, s3;

        if (line >> s1 >> s2 >> s3)
        { // cout << "Transaction rule: ";
            // cout << s1 <<  ',' << s2 << ',' << s3 << endl;
            transitions.push_back({s1, s2, s3});
        }
    }
    // cout << "end reading dfa" << endl;
    string inString;
    while (std::getline(std::cin, inString))
    {
        
    }
}