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
map<string, map<string, int>> method_offset_tabel;

void push(string regValue)
{
    cout << "sw " << regValue << ", -4($30) ; push the value of " << regValue << " onto the stack" << endl;
    cout << "sub $30, $30, $4 ; -----------" << endl;
}
void pop(string regValue)
{
    cout << "add $30, $30, $4  ; " << endl;
    cout << "lw " << regValue << ",-4($30) ; -----------" << endl;
}
// code(a) ; results in lw $3, 0($29), which we will write as $3 <- a
//         ; results in lw $3, shift($29), which we will write as $3 <- a

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
        if (symbolTables.count(compairedString))
        {
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
        for (size_t k = 0; k < pr->children.size(); k++)
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
        int loopsize = statements->children.size();
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
int ifCounter = 0;
int whileCounter = 0;
int skipDeleteCounter = 0;
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
        for (size_t i = 0; i < pr->children.size(); i++)
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
        for (size_t i = 0; i < pr->children.size(); i++)
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

//================================================================================================================================
void code(Tree *expr, map<string, int> &offset_tabel, map<string, string> &symbolTables)
{
    if (expr->rule == "expr term")
    {
        code(expr->children[0], offset_tabel, symbolTables);
    }
    else if (expr->rule == "term factor")
    {
        code(expr->children[0], offset_tabel, symbolTables);
    }
    else if (expr->rule == "factor ID")
    {
        Tree *ID = expr->children[0];
        string a = ID->ch[0];
        // symbolTables[a] need number shift
        int shift = offset_tabel[a];
        cout << "lw $3, " << shift << "($29) ; which we will write as $3 <- " << a << endl;
    }
    else if (expr->rule == "factor LPAREN expr RPAREN")
    {
        cout << "; " << expr->rule << endl;
        Tree *expr_children = expr->children[1];
        code(expr_children, offset_tabel, symbolTables);
    }
    else if (expr->rule == "expr expr PLUS term")
    {
        cout << "; " << expr->rule << endl;
        Tree *expr_children = expr->children[0];
        Tree *term_children = expr->children[2];

        string firstExprType, secondExprType;
        firstExprType = exprType(expr_children, symbolTables);
        secondExprType = exprType(term_children, symbolTables);

        code(expr_children, offset_tabel, symbolTables);
        push("$3");
        code(term_children, offset_tabel, symbolTables);
        pop("$5");
        // remember $5  =  value of expr_children
        //          $3  =  value of term_children
        cout << "; =========== " << firstExprType << " " << secondExprType << endl;
        if (firstExprType == "int" && secondExprType == "int*")
        {
            cout << "mult $4, $5 ; ====== first is int" << endl;
            cout << "mflo $5" << endl;
        }
        if (firstExprType == "int*" && secondExprType == "int")
        {
            cout << "mult $4, $3 ; ====== second is int" << endl;
            cout << "mflo $3" << endl;
        }
        cout << "add $3, $5, $3" << endl;
    }
    else if (expr->rule == "expr expr MINUS term")
    {

        Tree *expr_children = expr->children[0];
        Tree *term_children = expr->children[2];
        code(expr_children, offset_tabel, symbolTables);
        push("$3");
        cout << "; start term children " << expr->rule << endl;
        code(term_children, offset_tabel, symbolTables);
        cout << "; end term children" << endl;
        pop("$5");
        // remember $5  =  value of expr_children
        //          $3  =  value of term_children

        string firstExprType, secondExprType;
        firstExprType = exprType(expr_children, symbolTables);
        secondExprType = exprType(term_children, symbolTables);
        cout << "; =========== " << firstExprType << " - " << secondExprType << endl;
        if (firstExprType == "int*" && secondExprType == "int")
        {

            cout << "mult $4, $3" << endl;
            cout << "mflo $3" << endl;
        }
        cout << "sub $3, $5, $3" << endl;
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "divu $3, $4" << endl;
            cout << "mflo $3" << endl;
        }
    }
    else if (expr->rule == "term term STAR factor")
    {
        Tree *expr_children = expr->children[0];
        Tree *term_children = expr->children[2];
        cerr << expr_children->rule << " " << term_children->rule << endl;
        code(expr_children, offset_tabel, symbolTables);
        push("$3");
        code(term_children, offset_tabel, symbolTables);
        pop("$5");
        // remember $5  =  value of expr_children
        //          $3  =  value of term_children
        cout << "mult $5, $3" << endl;
        cout << "mflo $3" << endl;
    }
    else if (expr->rule == "term term SLASH factor")
    {
        Tree *expr_children = expr->children[0];
        Tree *term_children = expr->children[2];
        code(expr_children, offset_tabel, symbolTables);
        push("$3");
        code(term_children, offset_tabel, symbolTables);
        pop("$5");
        // remember $5  =  value of expr_children
        //          $3  =  value of term_children
        cout << "div $5, $3" << endl;
        cout << "mflo $3" << endl;
    }
    else if (expr->rule == "term term PCT factor")
    {
        Tree *expr_children = expr->children[0];
        Tree *term_children = expr->children[2];
        code(expr_children, offset_tabel, symbolTables);
        push("$3");
        code(term_children, offset_tabel, symbolTables);
        pop("$5");
        // remember $5  =  value of expr_children
        //          $3  =  value of term_children
        cout << "div $5, $3" << endl;
        cout << "mfhi $3" << endl;
    }
    else if (expr->rule == "factor NUM")
    {
        Tree *NUM = expr->children[0];
        string val = NUM->ch[0];
        cout << "lis $3" << endl;
        cout << ".word " << val << endl;
    }
    else if (expr->rule == "statements statements statement")
    {
        Tree *statements_1 = expr->children[0];
        Tree *statements_2 = expr->children[1];
        code(statements_1, offset_tabel, symbolTables);
        code(statements_2, offset_tabel, symbolTables);
    }
    else if (expr->rule == "statement PRINTLN LPAREN expr RPAREN SEMI")
    {
        Tree *expr_1 = expr->children[2];
        push("$1");
        code(expr_1, offset_tabel, symbolTables);
        cout << "add $1, $3, $0" << endl;
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word print" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        pop("$1");
    }
    else if (expr->rule == "statement lvalue BECOMES expr SEMI")
    {
        cout << "; statement lvalue BECOMES expr SEMI" << endl;
        Tree *lvalue = expr->children[0];
        Tree *expr_1 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        if (lvalue->rule == "lvalue STAR factor")
        {
            Tree *factor = lvalue->children[1];
            push("$3");
            code(factor, offset_tabel, symbolTables);
            pop("$5");
            cout << "sw $5, 0($3)" << endl;
        }
        else if (lvalue->rule == "lvalue ID")
        {
            // store the value of expr in offset(char)($29)
            Tree *ID = lvalue->children[0];
            string a = ID->ch[0];
            // symbolTables[a] need number shift
            int shift = offset_tabel[a];
            cout << "sw $3, " << shift << "($29) ; which we will write as $3 -> $29 + " << shift << " Written to " << a << endl;
        }
    }
    else if (expr->rule == "lvalue LPAREN lvalue RPAREN")
    {
        Tree *lvalue_1 = expr->children[1];
        code(lvalue_1, offset_tabel, symbolTables);
    }
    else if (expr->rule == "test expr LT expr")
    {
        // for pointer compairson use unsigned shit

        // store the value of expr in offset(char)($29)
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];

        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");
        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $3, $5, $3" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $3, $5, $3" << endl;
        }
    }
    else if (expr->rule == "test expr GT expr")
    {
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");

        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $3, $3, $5" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $3, $3, $5" << endl;
        }
    }
    else if (expr->rule == "test expr GE expr")
    {
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");
        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $3, $5, $3" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $3, $5, $3" << endl;
        }
        cout << "sub $3, $11, $3 ; negation" << endl;
    }
    else if (expr->rule == "test expr LE expr")
    {
        // store the value of expr in offset(char)($29)
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");

        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        cout << "; " << firstExprType << " <= " << secondExprType << endl;
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $3, $3, $5" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $3, $3, $5" << endl;
        }
        cout << "sub $3, $11, $3 ; negation" << endl;
    }
    else if (expr->rule == "test expr NE expr")
    {
        // store the value of expr in offset(char)($29)
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");

        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $6, $3, $5 ; $6 = $3 < $5" << endl;
            cout << "sltu $7, $5, $3 ; $7 = $5 < $3" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $6, $3, $5 ; $6 = $3 < $5" << endl;
            cout << "slt $7, $5, $3 ; $7 = $5 < $3" << endl;
        }

        cout << "add $3, $6, $7 ; Sum $6 and $7" << endl;
    }
    else if (expr->rule == "test expr EQ expr")
    {
        Tree *expr_1 = expr->children[0];
        Tree *expr_2 = expr->children[2];
        code(expr_1, offset_tabel, symbolTables);
        push("$3");
        code(expr_2, offset_tabel, symbolTables);
        pop("$5");
        string firstExprType, secondExprType;
        firstExprType = exprType(expr_1, symbolTables);
        secondExprType = exprType(expr_2, symbolTables);
        if (firstExprType == "int*" && secondExprType == "int*")
        {

            cout << "sltu $6, $3, $5 ; $6 = $3 < $5" << endl;
            cout << "sltu $7, $5, $3 ; $7 = $5 < $3" << endl;
        }
        else if (firstExprType == "int" && secondExprType == "int")
        {

            cout << "slt $6, $3, $5 ; $6 = $3 < $5" << endl;
            cout << "slt $7, $5, $3 ; $7 = $5 < $3" << endl;
        }

        cout << "add $3, $6, $7 ; Sum $6 and $7" << endl;
        cout << "sub $3, $11, $3 ; negation" << endl;
    }

    else if (expr->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE")
    {
        string loopPointer = "loop" + to_string(whileCounter);
        string endWhile = "endWhile" + to_string(whileCounter);
        whileCounter++;
        cout << loopPointer << ":" << endl;
        Tree *test = expr->children[2];
        Tree *statements = expr->children[5];
        cout << "; BEGIN TEST" << endl;
        code(test, offset_tabel, symbolTables);
        cout << "; END TEST" << endl;
        cout << "beq $3, $0, " << endWhile << endl;
        code(statements, offset_tabel, symbolTables);
        cout << "beq $0, $0, " << loopPointer << endl;
        cout << endWhile << ":" << endl;
    }

    else if (expr->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE")
    {
        // whileCounter
        //  store the value of expr in offset(char)($29)
        string elsePointer = "else" + to_string(ifCounter);
        string endif = "endif" + to_string(ifCounter);
        ifCounter++;
        Tree *test = expr->children[2];
        Tree *statements_1 = expr->children[5];
        Tree *statements_2 = expr->children[9];
        code(test, offset_tabel, symbolTables);
        cout << "beq $3, $0, " << elsePointer << endl;
        code(statements_1, offset_tabel, symbolTables);
        cout << "beq $0, $0, " << endif << endl;
        cout << elsePointer << ":" << endl;
        code(statements_2, offset_tabel, symbolTables);
        cout << endif << ":" << endl;
    }
    else if (expr->rule == "factor NULL")
    {
        cout << "add $3, $0, $11" << endl;
    }
    else if (expr->rule == "factor AMP lvalue")
    {
        cout << "; " << expr->rule << endl;
        Tree *lvalue = expr->children[1];
        if (lvalue->rule == "lvalue STAR factor")
        {
            Tree *factor2 = lvalue->children[1];
            cout << "; " << lvalue->rule << " " << factor2->rule << endl;
            cout << "; ====== factor 2 ======= Begin" << endl;
            code(factor2, offset_tabel, symbolTables);
            cout << "; ====== factor 2 ======= End" << endl;
        }
        else if (lvalue->rule == "lvalue ID")
        {
            // store the value of expr in offset(char)($29)
            Tree *ID = lvalue->children[0];
            string a = ID->ch[0];
            cout << "lis $3" << endl;

            // symbolTables[a] need number shift
            int shift = offset_tabel[a];
            cout << ".word " << shift << endl;
            cout << "add $3, $3, $29" << endl;
        }
    }
    else if (expr->rule == "factor STAR factor")
    {
        Tree *factor = expr->children[1];
        code(factor, offset_tabel, symbolTables);

        cout << "lw $3, 0($3)" << endl;
    }

    else if (expr->rule == "factor NEW INT LBRACK expr RBRACK")
    {

        Tree *expr2 = expr->children[3];
        code(expr2, offset_tabel, symbolTables);
        cout << "add $1, $3, $0 ; new procedure expects value in $1" << endl;
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word new" << endl;
        cout << "jalr $5 " << endl;
        pop("$31");
        cout << "bne $3, $0, 1 ; if call succeeded skip next instruction" << endl;
        cout << "add $3, $11, $0 ; set $3 to NULL if allocation fails" << endl;
    }
    else if (expr->rule == "statement DELETE LBRACK RBRACK expr SEMI")
    {

        Tree *expr2 = expr->children[3];
        code(expr2, offset_tabel, symbolTables);
        string skipDelete = "skipDelete" + to_string(skipDeleteCounter);
        skipDeleteCounter++;
        cout << "beq $3, $11, " << skipDelete << "" << endl;
        cout << "add $1, $3, $0 ; delete expects the address in $1" << endl;
        push("$31");
        cout << "lis $5 " << endl;
        cout << ".word delete" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        cout << skipDelete << ":" << endl;
        cout << "" << endl;
    }
    else if (expr->rule == "factor ID LPAREN RPAREN")
    {

        Tree *ID = expr->children[0];
        string funcName = ID->ch[0];
        cout << "; Calling anouther function: " << funcName << endl;
        push("$29");
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word " << funcName << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        pop("$29");
    }
    else if (expr->rule == "factor ID LPAREN arglist RPAREN")
    {

        Tree *ID = expr->children[0];
        Tree *arglist = expr->children[2];
        string funcName = ID->ch[0];
        cout << "; Calling anouther function: " << funcName << " =============== " << endl;
        push("$29");
        push("$31");
        push("$28");
        cout << "add $28, $30, $0" << endl;
        
        int n = 0;
        while (true)
        {
            Tree *linkedExpr = arglist->children[0];
            cout << "; ARgument Number# " << n << endl;
            code(linkedExpr, offset_tabel, symbolTables);
            push("$3");
            n++;
            if (arglist->rule == "arglist expr")
            {
                break;
            }
            else if (arglist->rule == "arglist expr COMMA arglist")
            {
                arglist = arglist->children[2];
            }
        }

        cout << "lis $5" << endl;
        cout << ".word " << funcName << endl;
        cout << "jalr $5" << endl;
        
        cout << "add $30, $28, $0 ; this is the restore the stack" << endl;
        pop("$28");
        pop("$31");
        pop("$29");
    }
}

void paramlist_gencode(Tree * /* paramlist */, map<string, int> & /* offset_tabel */, int & /* offset */)
{
    /*
        singleGetString(first_dcl, IDValue, tempType);
        method_offset_tabel[methodName][IDValue] = offset;
        offset -= 4;
        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "sw $1, 0($29) ; push variable a" << endl;
        */
}

void dcls_gencode(Tree *dcls, map<string, int> &offset_tabel, int &offset)
{
    if (dcls->rule == "dcls")
    {
    }
    else if (dcls->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {
        Tree *dcls_child = dcls->children[0];
        Tree *dcl = dcls->children[1];
        Tree *NUM = dcls->children[3];
        string NUM_val = NUM->ch[0], IDValue, tempType;

        singleGetString(dcl, IDValue, tempType);
        cerr << "OFFSET FOR " << IDValue << " IS " << offset << endl;
        offset_tabel[IDValue] = offset;
        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "lis $3 ; set $3 to the value of " << NUM_val << endl;
        cout << ".word " << NUM_val << endl;
        cout << "sw $3, " << offset << "($29) ; push variable type: " << tempType << " name: " << IDValue << endl;
        offset -= 4;
        dcls_gencode(dcls_child, offset_tabel, offset);
    }
    else if (dcls->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {

        Tree *dcls_child = dcls->children[0];
        Tree *dcl = dcls->children[1];
        Tree *NUM = dcls->children[3];
        string NUM_val = NUM->ch[0], IDValue, tempType;

        singleGetString(dcl, IDValue, tempType);
        cerr << "Initializing " << IDValue << "Offset IS " << offset << endl;
        offset_tabel[IDValue] = offset;
        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "lis $3 ; set $3 to the value of " << NUM_val << endl;
        cout << ".word " << NUM_val << endl;
        cout << "sw $3, " << offset << "($29) ; push variable type: " << tempType << " name: " << IDValue << endl;
        offset -= 4;
        dcls_gencode(dcls_child, offset_tabel, offset);
    }
    else if (dcls->rule == "dcls dcls dcl BECOMES NULL SEMI")
    {
        Tree *dcls_child = dcls->children[0];
        Tree *dcl = dcls->children[1];
        Tree *NUM = dcls->children[3];
        string NUM_val = NUM->ch[0], IDValue, tempType;

        singleGetString(dcl, IDValue, tempType);
        cerr << "OFFSET FOR " << IDValue << " IS " << offset << endl;
        offset_tabel[IDValue] = offset;
        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "sw $11, " << offset << "($29) ; push variable type: " << tempType << " name: " << IDValue << endl;
        offset -= 4;
        dcls_gencode(dcls_child, offset_tabel, offset);
    }
}
int generateCode(Tree *pr)
{
    int sum = 0;
    stringstream ssin(pr->rule);

    if (pr->rule == "start BOF procedures EOF")
    {
        sum += generateCode(pr->children[1]);
    }
    if (pr->rule == "procedures procedure procedures")
    {
        sum += generateCode(pr->children[0]);
        sum += generateCode(pr->children[1]);
    }
    if (pr->rule == "procedures main")
    {
        sum += generateCode(pr->children[0]);
    }
    if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    {
        Tree *first_dcl = pr->children[3];
        Tree *second_dcl = pr->children[5];
        Tree *dcls = pr->children[8];
        Tree *statements = pr->children[9];
        Tree *return_expr = pr->children[11];
        string methodName = "wain", IDValue, tempType;
        int offset = 0;

        cout << "wain: ; begin prologue" << endl;

        cout << "sub $29, $30, $4 ; setup frame pointer" << endl;

        singleGetString(first_dcl, IDValue, tempType);
        method_offset_tabel[methodName][IDValue] = offset;
        offset -= 4;
        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "sw $1, 0($29) ; push variable a" << endl;

        singleGetString(second_dcl, IDValue, tempType);
        method_offset_tabel[methodName][IDValue] = offset;
        offset -= 4;

        cout << "sub $30 , $30, $4 ; update stack pointer" << endl;
        cout << "sw $2, -4($29) ; push variable b" << endl;
        cout << "; end prologue" << endl;

        // Calling init
        push("$2");
        string ID;
        singleGetString(first_dcl, IDValue, tempType);
        if (tempType == "int")
        {
            cout << "add $2, $0, $0" << endl;
        }
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word init" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        pop("$2");
        // end calling init

        dcls_gencode(dcls, method_offset_tabel[methodName], offset);
        code(statements, method_offset_tabel[methodName], tables[methodName].second);
        cout << "; begin return" << endl;
        code(return_expr, method_offset_tabel[methodName], tables[methodName].second);

        cout << "; begin epilogue" << endl;

        cout << "add $30 , $29, $4 ; update stack pointer" << endl;
        cout << "jr $31 " << endl;
        cout << "; end epilogue" << endl;
    }
    else if (pr->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE")
    { //                           0   1    2     3      4       5     6      7         8    9
        string methodName = pr->children[1]->rule.substr(3);
        cout << "; " << pr->rule << endl;
        cout << methodName << ": ; begin prologue ======== ANOUTHER FUNCTION: " << methodName << endl;
        Tree *params = pr->children[3];
        Tree *dcls = pr->children[6];
        Tree *statements = pr->children[7];
        Tree *return_expr = pr->children[9];

        int offset = 0;
        cout << "sub $29, $30, $4 ; setup frame pointer" << endl;

        if (params->rule == "params paramlist")
        {
            Tree *paramlist = params->children[0];
            while (true)
            {
                offset += 4;
                if (paramlist->rule == "paramlist dcl")
                {
                    break;
                }
                else if (paramlist->rule == "paramlist dcl COMMA paramlist")
                {
                    paramlist = paramlist->children[2];
                }
            }
            paramlist = params->children[0];
            while (true)
            {
                Tree *first_dcl = paramlist->children[0];
                string IDValue, tempType;
                singleGetString(first_dcl, IDValue, tempType);

                method_offset_tabel[methodName][IDValue] = (offset);
                offset = offset - 4;
                if (paramlist->rule == "paramlist dcl")
                {
                    break;
                }
                else if (paramlist->rule == "paramlist dcl COMMA paramlist")
                {
                    paramlist = paramlist->children[2];
                }
            }
        }

        cout << "; end prologue" << endl;
        dcls_gencode(dcls, method_offset_tabel[methodName], offset);
        code(statements, method_offset_tabel[methodName], tables[methodName].second);
        cout << "; begin return" << endl;
        code(return_expr, method_offset_tabel[methodName], tables[methodName].second);

        // I need to count the number of declairs REEEEEEE
        cout << "; begin epilogue" << endl;

        cout << "add $30 , $29, $4 ; update stack pointer" << endl;
        cout << "jr $31 " << endl;
        cout << "; end epilogue" << endl;
    }
    return sum;
}

int main()
{
    // First we need to make the parse tree from WLP4I file

    string curLine;
    stack<Tree *> WLP4I;
    stack<int> WLP4INumChildren;
    Tree rootWrapper{};
    Tree *root;
    bool first = true;
    while (getline(cin, curLine))
    {

        // cout << curLine << " " << children << " " << allCaps(firstNum) << endl;

        WLP4I.push(new Tree{curLine, {}, {}}); // add the node on top
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
    rootWrapper.children.push_back(root);
    // root->printParseTree();
    if (buildTable(root, tables))
    {
        cerr << "ERROR: build failed" << endl;
        return 1;
    }

    for (auto const &pair_0 : tables)
    {
        std::cerr << pair_0.first << ":";

        for (size_t i = 0; i < pair_0.second.first.size(); i++)
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
    cout << ".import print" << endl;
    cout << ".import init" << endl;
    cout << ".import new" << endl;
    cout << ".import delete" << endl;
    cout << "lis $4" << endl;
    cout << ".word 4" << endl;
    cout << "lis $11" << endl;
    cout << ".word 1" << endl;

    cout << "beq $0, $0, wain" << endl;
    generateCode(root);
}
