#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <queue>
using namespace std;
// Skip the grammar part of the input.

// T should keep curState while a should keep the transaction rules

// Reads a .cfg file and prints the derivation
// without leading or trailing spaces.
bool allCaps(string s)
{
    bool ret = true;
    int i = 0;
    while (s[i])
    {
        ret = ret & (isupper(s[i]) > 0);
        i++;
    }
    return ret;
}

class Tree
{
public:
    string rule;
    vector<Tree *> children;
    vector<string> ch;
    ~Tree()
    {
        for (auto k : children)
        {
            delete k;
        }
    }
    void printParseTree()
    {
        cout << rule << endl;
        for (Tree *a : children)
        {
            a->printParseTree();
        }
    }
};
map<string, pair<vector<string>, map<string, string>>> tables;
void singleGetString(Tree *dcl, string &IDValue, string &tempType)
{
    Tree *TYPE = dcl->children[0];
    Tree *ID = dcl->children[1];
    IDValue = ID->rule.substr(3);
    if (TYPE->children.size() == 1)
    {
        // INT
        tempType = "int";
    }
    if (TYPE->children.size() == 2)
    {
        // INT *
        tempType = "int*";
    }
}
int singleDCL(Tree *dcl, map<string, string> &symbolTables)
{
    string IDValue, tempType;
    singleGetString(dcl, IDValue, tempType);

    if (symbolTables.count(IDValue))
    {
        std::cerr << "ERROR: Duplicate initialization" << endl;
        return 1;
    }
    symbolTables[IDValue] = tempType;
    return 0;
}

// Function name -> {parameters, ID -> type }
// ID -> type
int dclsLinkedList(Tree *dcls, map<string, string> &symbolTables)
{
    int sum = 0;
    while (dcls->children.size() > 0)
    {
        // pase dcl
        string tempType, IDValue;
        singleGetString(dcls->children[1], IDValue, tempType);
        if (tempType == "int" && dcls->ch[3] == "NULL")
        {
            cerr << "ERROR:: int = NULL ?!?!?!" << endl;
            return 1;
        }

        if (tempType == "int*" && dcls->ch[3] == "NUM")
        {
            cerr << "ERROR:: int* = NUM ?!?!?!" << endl;
            return 1;
        }
        sum += singleDCL(dcls->children[1], symbolTables);
        // now I need to check is the becomes num/null valid

        dcls = dcls->children[0];

        // get rules
    }
    return sum;
}
/*
The type of an lvalue deriving LPAREN lvalue RPAREN is the same as the type of the derived lvalue.
The type of a factor deriving AMP lvalue is int*. The type of the derived lvalue (i.e. the one preceded by AMP) must be int.
The type of a factor or lvalue deriving STAR factor is int. The type of the derived factor (i.e. the one preceded by STAR) must be int*.

*/
void makeParamTypeList(Tree *arglist, map<string, string> &symbolTables, vector<string> &typeList);
// retuns "invalid, int or int*"
string exprType(Tree *expr, map<string, string> &symbolTables)
{
    // expr->printParseTree();

    string ret = "invalid", a, b;
    if (expr->rule == "expr expr PLUS term")
    {
        a = exprType(expr->children[0], symbolTables);
        b = exprType(expr->children[2], symbolTables);
        if (a == "int" && b == "int")
        {
            ret = "int";
        }
        if (a == "int*" && b == "int")
        {
            ret = "int*";
        }
        if (a == "int" && b == "int*")
        {
            ret = "int*";
        }
    }
    else if (expr->rule == "factor ID" || expr->rule == "lvalue ID")
    {
        Tree *ID_Node = expr->children[0];
        string varName = ID_Node->rule.substr(3);
        ret = symbolTables[varName];
    }
    else if (expr->rule == "expr term")
    {
        a = exprType(expr->children[0], symbolTables);
        ret = a;
    }
    else if (expr->rule == "expr expr MINUS term")
    {
        a = exprType(expr->children[0], symbolTables);
        b = exprType(expr->children[2], symbolTables);
        if (a == "int" && b == "int")
        {
            ret = "int";
        }
        if (a == "int*" && b == "int*")
        {
            ret = "int";
        }
        if (a == "int*" && b == "int")
        {
            ret = "int*";
        }
    }
    else if (expr->rule == "term factor")
    {
        a = exprType(expr->children[0], symbolTables);
        ret = a;
    }
    else if (expr->rule == "term term STAR factor")
    {
        a = exprType(expr->children[0], symbolTables);
        b = exprType(expr->children[2], symbolTables);
        if (a == "int" && b == "int")
        {
            ret = "int";
        }
    }
    else if (expr->rule == "term term SLASH factor")
    {
        a = exprType(expr->children[0], symbolTables);
        b = exprType(expr->children[2], symbolTables);
        if (a == "int" && b == "int")
        {
            ret = "int";
        }
    }
    else if (expr->rule == "term term PCT factor")
    {
        a = exprType(expr->children[0], symbolTables);
        b = exprType(expr->children[2], symbolTables);
        if (a == "int" && b == "int")
        {
            ret = "int";
        }
    }
    else if (expr->rule == "factor NUM")
    {
        ret = "int";
    }
    else if (expr->rule == "factor NULL")
    {
        ret = "int*";
    }
    else if (expr->rule == "factor LPAREN expr RPAREN" || expr->rule == "lvalue LPAREN lvalue RPAREN")
    {
        ret = exprType(expr->children[1], symbolTables);
    }
    else if (expr->rule == "factor AMP lvalue")
    {
        a = exprType(expr->children[1], symbolTables);
        /*
            lvalue ID
            lvalue STAR factor
            lvalue LPAREN lvalue RPAREN
        */
        // a must be an int lvalue
        // getting the pointer
        // Need new method to find the value of lvalue
        if (a == "int")
        {
            ret = "int*";
        }
        // cerr << expr->rule << " " << ret << endl;
    }
    else if (expr->rule == "factor STAR factor" || expr->rule == "lvalue STAR factor")
    {
        // this is dereference factor muse be an int* orelse it is invalid
        a = exprType(expr->children[1], symbolTables); // what does this do?Î

        if (a == "int*")
        {
            ret = "int";
        }
    }
    else if (expr->rule == "factor NEW INT LBRACK expr RBRACK")
    {
        a = exprType(expr->children[3], symbolTables);
        if (a == "int")
        {
            ret = "int*";
        }
    }
    else if (expr->rule == "factor ID LPAREN RPAREN")
    {
        /*
        The type of a factor deriving ID LPAREN RPAREN is int.
        The procedure whose name is ID must have an empty signature.
        */
        string functionName = expr->children[0]->rule.substr(3);
        if (tables[functionName].first.size() == 0)
            ret = "int";

        // both just return the value that ID returns
    }
    else if (expr->rule == "factor ID LPAREN arglist RPAREN")
    {
        /*
        The type of a factor deriving ID LPAREN arglist RPAREN is int.
        The procedure whose name is ID must have a signature whose length is equal to the number of expr strings (separated by COMMA) that are derived from arglist. Further the types of these expr strings must exactly match, in order, the types in the procedure's signature.
       */
        // arglist to vector of strings
        vector<string> typeList;
        Tree *TD = expr->children[0];
        string functionName = TD->rule.substr(3);

        makeParamTypeList(expr->children[2], symbolTables, typeList);
        int len1 = tables[functionName].first.size(), len2 = typeList.size();
        bool allSameElements = (len1 == len2);
        // cerr << len1 << " != " << len2 << endl;
        // cerr << allSameElements << endl;
        for (int i = 0; i < len2 && allSameElements; i++)
        {
            allSameElements = allSameElements & (typeList[i] == tables[functionName].first[i]);
        }
        if (allSameElements)
            ret = "int";

        // both just return the value that ID returns
    }

    return ret;
}
void makeParamTypeList(Tree *arglist, map<string, string> &symbolTables, vector<string> &typeList)
{
    while (true)
    {
        string a = exprType(arglist->children[0], symbolTables);
        typeList.push_back(a);
        if (arglist->rule == "arglist expr")
        {
            break;
        }
        if (arglist->rule == "arglist expr COMMA arglist")
        {
            arglist = arglist->children[2];
        }
    }
}
bool allValidVar(Tree *pr, map<string, string> &symbolTables, string &illegalVar)
{
    // pr must be an exec node
    // run through the parse tree and check for the use of ininitialized shit
    // using uninitialized shit
    bool ret = true;
    stringstream ssin(pr->rule);
    string firstNum, strIn;
    ssin >> firstNum;
    int children = 0;
    vector<string> splitedRule;
    // Have child
    while (ssin >> strIn)
    {
        splitedRule.push_back(strIn);
        children++;
        // this is getting the children of the stack once it poped it goes to the main stack
    }
    if (firstNum == "dcls" || firstNum == "dcl" || firstNum == "params")
    {
        return ret;
    }
    else if (pr->rule == "factor ID")
    {
        Tree *ID = pr->children[0];
        string compairedString = ID->rule.substr(3);
        ret = (symbolTables.count(compairedString) > 0);
        if (symbolTables.count(compairedString) == 0)
        {
            illegalVar = compairedString;
            ret = false;
        }
        return ret;
    }
    else if (pr->rule == "factor ID LPAREN RPAREN" || pr->rule == "factor ID LPAREN arglist RPAREN")
    {
        Tree *ID = pr->children[0];
        string compairedString = ID->rule.substr(3);
        // look for ID in the FUNCTION call Look in tabels
        ret = false;
        if(symbolTables.count(compairedString)){
            return ret;
        }
        illegalVar = compairedString;
        for (auto const &pair_0 : tables)
        {
            if (compairedString == pair_0.first)
            {
                ret = true;
                illegalVar = "";
            }
        }
        if (pr->rule == "factor ID LPAREN arglist RPAREN")
        {
            bool buf = allValidVar(pr->children[2], symbolTables, illegalVar);
            ret = ret & buf;
        }
        return ret;
    }
    else
    {
        for (int k = 0; k < pr->children.size(); k++)
        {
            if (splitedRule[k] == "ID" && firstNum == "procedure")
                continue;
            // check function names

            bool buf = allValidVar(pr->children[k], symbolTables, illegalVar);
            ret = ret & buf;
        }
    }
    return ret;
}

bool checkStatementTypeValidity(Tree *statements, map<string, string> &symbolTables)
{
    bool ret = true;


    if (statements->rule == "statement lvalue BECOMES expr SEMI")
    {
        Tree *leftexpr = statements->children[0];
        Tree *rightexpr = statements->children[2];
        string leftexpr_typ = exprType(leftexpr, symbolTables);
        string rightexpr_typ = exprType(rightexpr, symbolTables);
        ret = ret & (rightexpr_typ == leftexpr_typ && leftexpr_typ != "invalid");
        return ret;
    }
    else if (statements->rule == "statement PRINTLN LPAREN expr RPAREN SEMI")
    {

        string exprTpye_thus = exprType(statements->children[2], symbolTables);

        ret = (exprTpye_thus == "int");
        return ret;
    }
    else if (statements->rule == "statement DELETE LBRACK RBRACK expr SEMI")
    {
        string exprTpye_thus = exprType(statements->children[3], symbolTables);
        ret = (exprTpye_thus == "int*");
        return ret;
    }
    else if (statements->rule == "statements")
    {
        return ret;
    }
    else if (statements->rule == "statements statements statement")
    {
        ret = checkStatementTypeValidity(statements->children[0], symbolTables) &
              checkStatementTypeValidity(statements->children[1], symbolTables);
        return ret;
    }
    else if (statements->rule == "statement lvalue BECOMES expr SEMI")
    {
        // cout << "============" << endl;
        //  for(int i = 0; i < statements->children.size(); i++){
        Tree *lvalue = statements->children[0];
        Tree *expr_2 = statements->children[2];

        // cerr << "seg fault ? " << statements->children.size() << endl;
        string lvaluestr = exprType(lvalue, symbolTables);
        string theexpression = exprType(expr_2, symbolTables);

        // statements->printParseTree();

        //}
        ret = (lvaluestr == theexpression);
        return ret;
    }
    else
    {
        int loopsize =  statements->children.size();
        for (int k = 0; k < loopsize; k++)
        {
            if (statements->ch[k] == "statements")
            {
                
                ret = ret & checkStatementTypeValidity(statements->children[k], symbolTables);
            }
            if (statements->ch[k] == "test")
            {
                // test operation tests
                Tree *test = statements->children[k];
                Tree *leftexpr = test->children[0];
                Tree *rightexpr = test->children[2];
                string leftexpr_typ = exprType(leftexpr, symbolTables);
                string rightexpr_typ = exprType(rightexpr, symbolTables);
                ret = ret & (rightexpr_typ == leftexpr_typ && leftexpr_typ != "invalid");
            }
        }
    }
    return ret;
}

int buildTable(Tree *pr, map<string, pair<vector<string>, map<string, string>>> &tables)
{
    int sum = 0;
    stringstream ssin(pr->rule);

    if (pr->rule == "start BOF procedures EOF")
    {
        sum += buildTable(pr->children[1], tables);
    }
    if (pr->rule == "procedures procedure procedures")
    {
        sum += buildTable(pr->children[0], tables);
        sum += buildTable(pr->children[1], tables);
    }
    if (pr->rule == "procedures main")
    {
        sum += buildTable(pr->children[0], tables);
    }
    if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    {
        // Base case
        string methodName = "wain";
        string tempType, ID;
        int ind_lis[]{3, 5};
        for (int i_buf = 0; i_buf < 2; i_buf++)
        {
            singleGetString(pr->children[ind_lis[i_buf]], ID, tempType);
            sum += singleDCL(pr->children[ind_lis[i_buf]], tables["wain"].second);
            tables["wain"].first.push_back(tempType);
        }
        singleGetString(pr->children[5], ID, tempType);
        if (tempType != "int")
        {
            std::cerr << "ERROR: second variable in main must be an INTEGER varname: " << tempType << endl;
            return 1;
        }

        sum += dclsLinkedList(pr->children[8], tables["wain"].second);
        string illegal = "";
        bool cmp = allValidVar(pr, tables["wain"].second, illegal);
        if (!cmp)
        {
            std::cerr << "ERROR: uninitialized variable found in main"
                      << " "
                      << " (" << illegal << ")" << endl;
            return 1;
        }
    }
    if (pr->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    {
        string methodName = pr->children[1]->rule.substr(3);
        Tree *paramlist = NULL;
        // params -> nothing
        if (pr->children[3]->rule == "params")
        {
        }
        else
        {
            // Params -> paramlist
            paramlist = pr->children[3]->children[0];
        }
        if (tables.count(methodName) > 0)
        {
            cerr << "ERROR: duplicate method found in " << methodName << endl;
            return 1;
        }
        while (paramlist != NULL)
        {
            // pase dcl
            string IDValue;
            string tempType;
            singleGetString(paramlist->children[0], IDValue, tempType);
            sum += singleDCL(paramlist->children[0], tables[methodName].second);
            tables[methodName].first.push_back(tempType);
            if (paramlist->children.size() == 1)
                break;
            paramlist = paramlist->children[2];
        }
        sum += dclsLinkedList(pr->children[6], tables[methodName].second);
        // check EXPR checks all instances of expr under statements and return
        string illegal = "";
        bool cmp = allValidVar(pr, tables[methodName].second, illegal);
        if (!cmp)
        {
            cerr << "ERROR: use of undeclared variable found in " << methodName << " (" << illegal << ")" << endl;
            return 1;
        }
    }
    return sum;
}
/*
statements
statements statements statement
statement lvalue BECOMES expr SEMI
*/

int checkProgramTypeValidity(Tree *pr, map<string, pair<vector<string>, map<string, string>>> &tables)
{
    int sum = 0;
    stringstream ssin(pr->rule);

    if (pr->rule == "start BOF procedures EOF")
    {
        sum += checkProgramTypeValidity(pr->children[1], tables);
    }
    if (pr->rule == "procedures procedure procedures")
    {
        sum += checkProgramTypeValidity(pr->children[0], tables);
        sum += checkProgramTypeValidity(pr->children[1], tables);
    }
    if (pr->rule == "procedures main")
    {
        sum += checkProgramTypeValidity(pr->children[0], tables);
    }
    if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    {
        string methodName = "wain";
        for (int i = 0; i < pr->children.size(); i++)
        {
            if (pr->ch[i] == "statements")
            {
                bool valid = checkStatementTypeValidity(pr->children[i], tables[methodName].second);
                if (!valid)
                {
                    std::cerr << "ERROR: Invalid statement in " << methodName << endl;
                    return 1;
                }
            }
            if (pr->ch[i] == "expr")
            {
                string exprTpye_thus = exprType(pr->children[i], tables[methodName].second);
                if (exprTpye_thus == "invalid")
                {
                    std::cerr << "ERROR: Invalid return statement " << methodName << endl;
                    return 1;
                }
                if (exprTpye_thus == "int*")
                {
                    std::cerr << "ERROR: Invalid return value int* " << methodName << endl;
                    return 1;
                }
            }
        }
    }
    if (pr->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    {
        string methodName = pr->children[1]->rule.substr(3);
        for (int i = 0; i < pr->children.size(); i++)
        {
            if (pr->ch[i] == "statements")
            {
                bool valid = checkStatementTypeValidity(pr->children[i], tables[methodName].second);
                if (!valid)
                {
                    std::cerr << "ERROR: Invalid statement in " << methodName << endl;
                    return 1;
                }
            }
            if (pr->ch[i] == "expr")
            {
                string exprTpye_thus = exprType(pr->children[i], tables[methodName].second);
                if (exprTpye_thus == "invalid")
                {
                    std::cerr << "ERROR: Invalid return statement " << methodName << endl;
                    return 1;
                }
                if (exprTpye_thus == "int*")
                {
                    std::cerr << "ERROR: Invalid return value int* " << methodName << endl;
                    return 1;
                }
            }
        }
    }
    return sum;
}

int main()
{
    // First we need to make the parse tree from WLP4I file

    string curLine;
    stack<Tree *> WLP4I;
    stack<int> WLP4INumChildren;
    Tree *root;
    bool first = true;
    while (getline(cin, curLine))
    {

        // cout << curLine << " " << children << " " << allCaps(firstNum) << endl;

        WLP4I.push(new Tree{curLine}); // add the node on top
        if (first)
        {
            root = WLP4I.top();
            first = false;
        }
        stringstream ssin(curLine);
        string firstNum, strIn;
        ssin >> firstNum;
        int children = 0;
        // Have child
        while (ssin >> strIn)
        {
            WLP4I.top()->ch.push_back(strIn);
            children++;
            // this is getting the children of the stack once it poped it goes to the main stack
        }
        if (allCaps(firstNum))
        {
            children = 0;
        }
        WLP4INumChildren.push(children);
        while (!WLP4INumChildren.empty() && !WLP4INumChildren.top())
        // the top is zero in the new assignemnt the zero top needs to be identified immediatelly
        {
            // cout << treeStack.top().num << " " << treeStack.top().children << endl;
            WLP4INumChildren.pop();
            Tree *removed_node = WLP4I.top();
            WLP4I.pop();
            if (WLP4INumChildren.empty() || WLP4I.empty())
                break;
            WLP4I.top()->children.push_back(removed_node);

            // this is removing one from the parent node of needy children

            int buf = WLP4INumChildren.top();
            WLP4INumChildren.pop();
            WLP4INumChildren.push(buf - 1);
        }
    }

    if (buildTable(root, tables))
    {
        delete root;
        return 1;
    }

    for (auto const &pair_0 : tables)
    {
        std::cerr << pair_0.first << ":";

        for (int i = 0; i < pair_0.second.first.size(); i++)
        {
            std::cerr << " " << tables[pair_0.first].first[i];
        }
        cerr << endl;
        for (auto const &pair_3 : pair_0.second.second)
        {
            std::cerr << pair_3.first << " " << pair_3.second << endl;
        }
    }
    checkProgramTypeValidity(root, tables);
    delete root;
}
