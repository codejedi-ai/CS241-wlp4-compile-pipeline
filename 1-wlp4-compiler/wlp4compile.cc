#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <streambuf>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#define CS241_SCANNER_H

using namespace std;

// Centralized error aggregation for detailed reporting using a CLI string
struct ErrorAccumulator {
  std::string cliOutput;
  bool hasError = false;
};
static ErrorAccumulator g_error;

// A streambuf that appends directly into g_error.cliOutput
class StringAppendBuf : public std::streambuf {
  std::string *target;

protected:
  int overflow(int ch) override {
    if (ch != EOF) {
      target->push_back(static_cast<char>(ch));
    }
    return ch;
  }
  std::streamsize xsputn(const char *s, std::streamsize n) override {
    target->append(s, static_cast<size_t>(n));
    return n;
  }

public:
  explicit StringAppendBuf(std::string *t) : target(t) {}
};
static StringAppendBuf g_errBuf(&g_error.cliOutput);
static std::ostream g_errOstream(&g_errBuf);

static inline std::ostream &errorLog() {
  g_error.hasError = true;
  return g_errOstream;
}
static inline void resetErrorLog() {
  g_error.cliOutput.clear();
  g_error.hasError = false;
}

// Scanner failure context captured at point of failure
struct ScannerErrorInfo {
  int column = -1;                // 1-based column index within the line
  char offendingChar = '\0';      // character that caused the failure
  int previousState = -1;         // DFA state before consuming offendingChar
  std::string munchedInput;       // input consumed so far (including offender)
};
static ScannerErrorInfo g_scannerErrorInfo;

// Array of token kind names
const string tokenNames[] = {
    "ID",         "NUM",    "LPAREN", "RPAREN",  "LBRACE",  "RBRACE",  "RETURN",
    "IF",         "ELSE",   "WHILE",  "PRINTLN", "PUTCHAR", "GETCHAR", "WAIN",
    "BECOMES",    "INT",    "EQ",     "NE",      "LT",      "GT",      "LE",
    "GE",         "PLUS",   "MINUS",  "STAR",    "SLASH",   "PCT",     "COMMA",
    "SEMI",       "NEW",    "DELETE", "LBRACK",  "RBRACK",  "AMP",     "NULL",
    "WHITESPACE", "COMMENT"};

/*
 * C++ Starter generateAssemblyCode for CS241 A3
 * All generateAssemblyCode requires C++14, so if you're getting compile errors
 * make sure to use -std=c++14.
 *
 * This file contains helpers for asm.cc and should not need to be modified by
 * you. However, its comments contain important information and you should
 * read the entire file before you start modifying asm.cc.
 */

class Token;

/* Scans a single line of input and produces a list of tokens.
 *
 * Scan returns tokens with the following kinds:
 * ID: identifiers and keywords.
 * LABEL: labels (identifiers ending in a colon).
 * WORD: the special ".word" keyword.
 * COMMA: a comma.
 * LPAREN: a left parenthesis.
 * RPAREN: a right parenthesis.
 * INT: a signed or unsigned 32-bit integer written in decimal.
 * HEXINT: an unsigned 32-bit integer written in hexadecimal.
 * REG: a register between $0 and $31.
 */

std::vector<Token> scan(const std::string &input);

// Track current input line number for scanner error reporting
static int g_currentInputLine = 0; // 1-based; set by performLexicalAnalysis

/* A scanned token produced by the scanner.
 * The "kind" tells us what kind of token it is
 * while the "lexeme" tells us exactly what text
 * the programmer typed. For example, the token
 * "abc" might have kind "ID" and lexeme "abc".
 *
 * While you can create tokens with any kind and
 * lexeme, the list of kinds produced by the
 * starter generateAssemblyCode can be found in the documentation
 * for scan above.
 */
class Token {
public:
  enum Kind {
    ID = 0,
    NUM,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    RETURN,
    IF,
    ELSE,
    WHILE,
    PRINTLN,
    PUTCHAR,
    GETCHAR,
    WAIN,
    BECOMES,
    INT,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PCT,
    COMMA,
    SEMI,
    NEW,
    DELETE,
    LBRACK,
    RBRACK,
    AMP,
    _NULL,
    WHITESPACE,
    COMMENT
  };

private:
  Kind kind;
  std::string lexeme;

public:
  Token(Kind kind, std::string lexeme);

  Kind getKind() const;
  const std::string &getLexeme() const;

  /* Converts a token to the corresponding number.
   * Only works on tokens of type INT, HEXINT, or REG.
   */
  int64_t toNumber() const;
  /* Further notes about the toNumber function:
   *
   * This function returns a 64-bit integer instead of 32-bit because
   * .word accepts both signed and unsigned 32-bit integers. The union
   * of the signed and unsigned ranges cannot be represented by a single
   * 32-bit type. Even when not working with .word, you should store the
   * result of this function in an int64_t variable, or you might get
   * unexpected behaviour due to C++'s automatic type conversions.
   *
   * If the number represented by the token does not fit in the signed
   * 64-bit range, there are two possibilities:
   * - If the token is a positive decimal integer or a hexadecimal
   *   integer, then 2^{63}-1 (the maximum int64_t value) is returned.
   * - If the token is a negative decimal integer, then -2^{63} (the
   *   minimum int64_t value) is returned.
   *
   * Hexadecimal integers are always interpreted as unsigned. For
   * example, the integer 0xffffffffffffffff (the 64-bit two's
   * complement representation of -1) will actually be interpreted as
   * 2^{64}-1, which is out of the signed 64-bit range. Thus, by the above
   * rule, the value 2^{63}-1 is returned. This behaviour may seem unusual,
   * but is actually important for detecting out of range hex integers.
   * For example, consider: beq $0, $0, 0xffffffffffffffff
   * If the hex integer here was interpreted as -1, that would be bad!
   */
};

/* Prints a string representation of a token.
 * Mainly useful for debugging.
 */
std::ostream &operator<<(std::ostream &out, const Token &tok);

/* An exception class thrown when an error is encountered while scanning.
 */
class ScanningFailure {
  std::string message;

public:
  ScanningFailure(std::string message);

  // Returns the message associated with the exception.
  const std::string &what() const;
};

/*
 * C++ Starter generateAssemblyCode for CS241 A3
 * All generateAssemblyCode requires C++14, so if you're getting compile errors
 * make sure to use -std=c++14.
 *
 * This file contains helpers for asm.cc and you don't need to modify it.
 * Read the scanner.h file for a description of the helper functions.
 *
 * This generateAssemblyCode may be helpful to understand when you write a
 * scanner in a later assignment. However, you do not need to understand how it
 * works to write the assembler.
 */

Token::Token(Token::Kind kind, std::string lexeme)
    : kind(kind), lexeme(std::move(lexeme)) {}

Token::Kind Token::getKind() const { return kind; }
const std::string &Token::getLexeme() const { return lexeme; }

std::ostream &operator<<(std::ostream &out, const Token &tok) {
  switch (tok.getKind()) {
  case Token::ID:
    out << "ID";
    break;
  case Token::NUM:
    out << "NUM";
    break;
  case Token::LPAREN:
    out << "LPAREN";
    break;
  case Token::RPAREN:
    out << "RPAREN";
    break;
  case Token::LBRACE:
    out << "LBRACE";
    break;
  case Token::RBRACE:
    out << "RBRACE";
    break;
  case Token::RETURN:
    out << "RETURN";
    break;
  case Token::IF:
    out << "IF";
    break;
  case Token::ELSE:
    out << "ELSE";
    break;
  case Token::WHILE:
    out << "WHILE";
    break;
  case Token::PRINTLN:
    out << "PRINTLN";
    break;
  case Token::PUTCHAR:
    out << "PUTCHAR";
    break;
  case Token::GETCHAR:
    out << "GETCHAR";
    break;
  case Token::WAIN:
    out << "WAIN";
    break;
  case Token::BECOMES:
    out << "BECOMES";
    break;
  case Token::INT:
    out << "INT";
    break;
  case Token::EQ:
    out << "EQ";
    break;
  case Token::NE:
    out << "NE";
    break;
  case Token::LT:
    out << "LT";
    break;
  case Token::GT:
    out << "GT";
    break;
  case Token::LE:
    out << "LE";
    break;
  case Token::GE:
    out << "GE";
    break;
  case Token::PLUS:
    out << "PLUS";
    break;
  case Token::MINUS:
    out << "MINUS";
    break;
  case Token::STAR:
    out << "STAR";
    break;
  case Token::SLASH:
    out << "SLASH";
    break;
  case Token::PCT:
    out << "PCT";
    break;
  case Token::COMMA:
    out << "COMMA";
    break;
  case Token::SEMI:
    out << "SEMI";
    break;
  case Token::NEW:
    out << "NEW";
    break;
  case Token::DELETE:
    out << "DELETE";
    break;
  case Token::LBRACK:
    out << "LBRACK";
    break;
  case Token::RBRACK:
    out << "RBRACK";
    break;
  case Token::AMP:
    out << "AMP";
    break;
  case Token::_NULL:
    out << "NULL";
    break;
  case Token::WHITESPACE:
    out << "WHITESPACE";
    break;
  case Token::COMMENT:
    out << "COMMENT";
    break;
  }
  out << " " << tok.getLexeme();

  return out;
}

int64_t Token::toNumber() const {
  std::istringstream iss;
  int64_t result;

  if (kind == NUM) {
    iss.str(lexeme);
  } else {
    // This should never happen if the user calls this function correctly
    return 0;
  }

  iss >> result;
  return result;
}

ScanningFailure::ScanningFailure(std::string message)
    : message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }

/* Representation of a DFA, used to handle the scanning process.
 */
class AsmDFA {
public:
  enum State {
    // States that are also kinds
    ID = 0,
    NUM,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    RETURN,
    IF,
    ELSE,
    WHILE,
    PRINTLN,
    PUTCHAR,
    GETCHAR,
    WAIN,
    BECOMES,
    INT,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PCT,
    COMMA,
    SEMI,
    NEW,
    DELETE,
    LBRACK,
    RBRACK,
    AMP,
    _NULL,
    WHITESPACE,
    COMMENT,
    // States that are not also kinds
    FAIL,
    START,
    DOT,
    ZERO,
    EXCLAMATIONMARK,

    // Hack to let this be used easily in arrays. This should always be the
    // final element in the enum, and should always point to the previous
    // element.

    LARGEST_STATE = EXCLAMATIONMARK
  };

private:
  /* A set of all accepting states for the DFA.
   * Currently non-accepting states are not actually present anywhere
   * in memory, but a list can be found in the constructor.
   */
  std::set<State> acceptingStates;

  /*
   * The transition function for the DFA, stored as a map.
   */

  std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;

  /*
   * Converts a state to a kind to allow construction of Tokens from States.
   * Throws an exception if conversion is not possible.
   */
  Token::Kind stateToKind(State s) const {
    switch (s) {
    case ID:
      return Token::ID;
    case NUM:
      return Token::NUM;
    case LPAREN:
      return Token::LPAREN;
    case RPAREN:
      return Token::RPAREN;
    case LBRACE:
      return Token::LBRACE;
    case RBRACE:
      return Token::RBRACE;
    case RETURN:
      return Token::RETURN;
    case IF:
      return Token::IF;
    case ELSE:
      return Token::ELSE;
    case WHILE:
      return Token::WHILE;
    case PRINTLN:
      return Token::PRINTLN;
    case PUTCHAR:
      return Token::PUTCHAR;
    case GETCHAR:
      return Token::GETCHAR;
    case WAIN:
      return Token::WAIN;
    case BECOMES:
      return Token::BECOMES;
    case INT:
      return Token::INT;
    case ZERO:
      return Token::NUM;
    case EQ:
      return Token::EQ;
    case NE:
      return Token::NE;
    case LT:
      return Token::LT;
    case GT:
      return Token::GT;
    case LE:
      return Token::LE;
    case GE:
      return Token::GE;
    case PLUS:
      return Token::PLUS;
    case MINUS:
      return Token::MINUS;
    case STAR:
      return Token::STAR;
    case SLASH:
      return Token::SLASH;
    case PCT:
      return Token::PCT;
    case COMMA:
      return Token::COMMA;
    case SEMI:
      return Token::SEMI;
    case NEW:
      return Token::NEW;
    case DELETE:
      return Token::DELETE;
    case LBRACK:
      return Token::LBRACK;
    case RBRACK:
      return Token::RBRACK;
    case AMP:
      return Token::AMP;
    case _NULL:
      return Token::_NULL;
    case WHITESPACE:
      return Token::WHITESPACE;
    case COMMENT:
      return Token::COMMENT;
    default:
      throw ScanningFailure("ERROR: Cannot convert state to kind.");
    }
  }

public:
  /* Tokenizes an input string according to the Simplified Maximal Munch
   * scanning algorithm.
   */
  std::vector<Token> simplifiedMaximalMunch(const std::string &input) const {
    std::vector<Token> result;

    State state = start();
    std::string munchedInput;
    int columnIndex = 0; // 0-based while scanning

    // We can't use a range-based for loop effectively here
    // since the iterator doesn't always increment.
    for (std::string::const_iterator inputPosn = input.begin();
         inputPosn != input.end();) {

      State oldState = state;
      state = transition(state, *inputPosn);

      if (!failed(state)) {
        munchedInput += *inputPosn;
        oldState = state;

        ++inputPosn;
        columnIndex++;
      }

      if (inputPosn == input.end() || failed(state)) {
        if (accept(oldState)) {
          result.push_back(Token(stateToKind(oldState), munchedInput));

          munchedInput = "";
          state = start();
        } else {
          if (failed(state)) {
            munchedInput += *inputPosn;
            // Capture detailed scanner failure context
            g_scannerErrorInfo.column = columnIndex + 1; // 1-based
            g_scannerErrorInfo.offendingChar = *inputPosn;
            g_scannerErrorInfo.previousState = static_cast<int>(oldState);
            g_scannerErrorInfo.munchedInput = munchedInput;
          }
          throw ScanningFailure(
              "ERROR: Simplified maximal munch failed on input: " +
              munchedInput);
        }
      }
    }

    return result;
  }

  /* Initializes the accepting states for the DFA.
   */
  AsmDFA() {
    acceptingStates = {
        ID,     NUM,   LPAREN,  RPAREN,  LBRACE,  RBRACE,    RETURN,  IF,
        ELSE,   WHILE, PRINTLN, PUTCHAR, GETCHAR, WAIN,      BECOMES, INT,
        EQ,     NE,    LT,      GT,      LE,      GE,        PLUS,    MINUS,
        STAR,   SLASH, PCT,     COMMA,   SEMI,    NEW,       DELETE,  LBRACK,
        RBRACK, AMP,   _NULL,   ZERO,    COMMENT, WHITESPACE};
    // Non-accepting states are DOT, MINUS, ZEROX, DOLLARS, START

    // Initialize transitions for the DFA
    for (size_t i = 0; i < transitionFunction.size(); ++i) {
      for (size_t j = 0; j < transitionFunction[0].size(); ++j) {
        transitionFunction[i][j] = FAIL;
      }
    }

    registerTransition(START, isalpha, ID);
    registerTransition(START, ".", DOT);
    registerTransition(START, "0", ZERO);
    registerTransition(START, "123456789", NUM);
    registerTransition(START, "-", MINUS);
    registerTransition(START, "+", PLUS);
    registerTransition(START, ";", SEMI);
    registerTransition(START, isspace, WHITESPACE);
    registerTransition(START, "!", EXCLAMATIONMARK);
    registerTransition(EXCLAMATIONMARK, "=", NE);
    registerTransition(START, ",", COMMA);
    registerTransition(START, "(", LPAREN);
    registerTransition(START, ")", RPAREN);
    registerTransition(START, "{", LBRACE);
    registerTransition(START, "}", RBRACE);
    registerTransition(START, "[", LBRACK);
    registerTransition(START, "]", RBRACK);

    registerTransition(START, "=", BECOMES);
    registerTransition(BECOMES, "=", EQ);

    registerTransition(START, "<", LT);
    registerTransition(START, ">", GT);
    registerTransition(LT, "=", LE);
    registerTransition(GT, "=", GE);

    registerTransition(START, "/", SLASH);
    registerTransition(SLASH, "/", COMMENT);

    registerTransition(START, "*", STAR);
    registerTransition(START, "%", PCT);
    registerTransition(START, ",", COMMA);
    registerTransition(START, ":", SEMI);
    registerTransition(START, "&", AMP);

    registerTransition(ID, isalnum, ID);
    registerTransition(ID, isdigit, ID);
    registerTransition(NUM, isdigit, NUM);

    registerTransition(
        COMMENT, [](int c) -> int { return c != '\n'; }, COMMENT);
    registerTransition(WHITESPACE, isspace, WHITESPACE);
  }

  // Register a transition on all chars in chars
  void registerTransition(State oldState, const std::string &chars,
                          State newState) {
    for (char c : chars) {
      transitionFunction[oldState][c] = newState;
    }
  }

  // Register a transition on all chars matching test
  // For some reason the cctype functions all use ints, hence the function
  // argument type.
  void registerTransition(State oldState, int (*test)(int), State newState) {

    for (int c = 0; c < 128; ++c) {
      if (test(c)) {
        transitionFunction[oldState][c] = newState;
      }
    }
  }

  /* Returns the state corresponding to following a transition
   * from the given starting state on the given character,
   * or a special fail state if the transition does not exist.
   */
  State transition(State state, char nextChar) const {
    return transitionFunction[state][nextChar];
  }

  /* Checks whether the state returned by transition
   * corresponds to failure to transition.
   */
  bool failed(State state) const { return state == FAIL; }

  /* Checks whether the state returned by transition
   * is an accepting state.
   */
  bool accept(State state) const { return acceptingStates.count(state) > 0; }

  /* Returns the starting state of the DFA
   */
  State start() const { return START; }
};

std::vector<Token> scan(const std::string &input) {
  static AsmDFA theDFA;

  std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);

  // We need to:
  // * Throw exceptions for WORD tokens whose lexemes aren't ".word".
  // * Remove WHITESPACE and COMMENT tokens entirely.

  std::vector<Token> newTokens;

  for (auto &token : tokens) {
    if (token.getKind() == Token::ID) {
      if (token.getLexeme() == "wain") {
        newTokens.push_back(Token(Token::WAIN, "wain"));
      } else if (token.getLexeme() == "return") {
        newTokens.push_back(Token(Token::RETURN, "return"));
      } else if (token.getLexeme() == "if") {
        newTokens.push_back(Token(Token::IF, "if"));
      } else if (token.getLexeme() == "int") {
        newTokens.push_back(Token(Token::INT, "int"));
      } else if (token.getLexeme() == "else") {
        newTokens.push_back(Token(Token::ELSE, "else"));
      } else if (token.getLexeme() == "while") {
        newTokens.push_back(Token(Token::WHILE, "while"));
      } else if (token.getLexeme() == "println") {
        newTokens.push_back(Token(Token::PRINTLN, "println"));
      } else if (token.getLexeme() == "putchar") {
        newTokens.push_back(Token(Token::PUTCHAR, "putchar"));
      } else if (token.getLexeme() == "getchar") {
        newTokens.push_back(Token(Token::GETCHAR, "getchar"));
      } else if (token.getLexeme() == "new") {
        newTokens.push_back(Token(Token::NEW, "new"));
      } else if (token.getLexeme() == "delete") {
        newTokens.push_back(Token(Token::DELETE, "delete"));
      } else if (token.getLexeme() == "NULL") {
        newTokens.push_back(Token(Token::_NULL, "NULL"));
      } else {
        newTokens.push_back(token);
      }
    } else if (token.getKind() != Token::WHITESPACE &&
               token.getKind() != Token::Kind::COMMENT) {
      newTokens.push_back(token);
    }
  }

  return newTokens;
}

/*
 * C++ Starter generateAssemblyCode for CS241 A3
 * All generateAssemblyCode requires C++14, so if you're getting compile errors
 * make sure to use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */
string performLexicalAnalysis() {
  std::string line;
  string result;
  try {
    int lineNumber = 0;
    while (getline(cin, line)) {
      g_currentInputLine = ++lineNumber;
      // This example generateAssemblyCode just prints the scanned tokens on
      // each line.
      vector<Token> tokenLine = scan(line);

      // This generateAssemblyCode is just an example - you don't have to use a
      // range-based for loop in your actual assembler. You might find it easier
      // to use an index-based loop like this: for(int i=0; i<tokenLine.size();
      // i++) { ... }
      for (auto &token : tokenLine) {
        result += tokenNames[token.getKind()] + " " + token.getLexeme() + "\n";
      }
    }
  } catch (ScanningFailure &f) {
    // Aggregate detailed scanner error message
    errorLog() << "[Stage: Scanning] Failure" << '\n';
    errorLog() << "  Line: " << g_currentInputLine << '\n';
    errorLog() << "  Column: " << g_scannerErrorInfo.column << '\n';
    errorLog() << "  Offending char: '" << g_scannerErrorInfo.offendingChar
               << "' (ascii " << static_cast<int>(g_scannerErrorInfo.offendingChar)
               << ")" << '\n';
    errorLog() << "  Previous DFA state: " << g_scannerErrorInfo.previousState
               << '\n';
    errorLog() << "  Munched input: \"" << g_scannerErrorInfo.munchedInput
               << "\"" << '\n';
    errorLog() << "  Message: " << f.what() << '\n';
    return result;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.

  return result;
}

// Skip the grammar part of the input.

struct transitionRule {
  string non_terminal;
  string instruction;
  int number_of_popped_symbols;
  vector<string> tokens;
};
struct parseTree {
  string element;
  list<parseTree *> childrens;
  parseTree(string a) : element(a) {}
  void insertNode(parseTree *buf) { childrens.push_back(buf); }
  ~parseTree() {
    for (parseTree *a : childrens) {
      delete a;
    }
  }
  string printParseTree() {
    string result = "";
    result += element + "\n";
    for (parseTree *a : childrens) {
      result += a->printParseTree();
    }
    return result;
  }
};
vector<transitionRule> tokens;
struct LL1_DFA_NODE {
  int curState;
  string symbol, type_1;
  int int1;
  LL1_DFA_NODE(int a, string b, string c, int d)
      : curState(a), symbol(b), type_1(c), int1(d) {}
};
map<string, int> map_symbol_to_integer;
vector<transitionRule> productionRules;

string skipLine(istream &infile) {
  string s;
  getline(infile, s);
  return s;
}
/*
After reading infile the CFG
An integer s -- the number of states infile the LR(1) DFA. States are numbered 0
to s-1. State 0 is the start state. An integer t -- the number of shift and
reduce actions engenerateAssemblyCoded infile the LR(1) DFA. t lines, each
containing one of: state terminal reduce rule (Rules (productions) are numbered
from 0 infile the order they appear infile the CFG.) state1 symbol shift state2
(where symbol is either a terminal or a non-terminal)
*/
string trim(const string &str) {
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos)
    return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}
// T should keep curState while a should keep the transaction rules

// Reads a .cfg file and prints the derivation
// without leading or trailing spaces.
int Reduce(int top, string next, vector<LL1_DFA_NODE> &LL1_DFA, int t) {
  for (int i = 0; i < t; i++) {
    if (LL1_DFA[i].curState == top && LL1_DFA[i].symbol == next &&
        LL1_DFA[i].type_1 == "reduce") {
      return LL1_DFA[i].int1;
    }
  }
  return -1;
}
int delta(int top, string next, vector<LL1_DFA_NODE> &LL1_DFA, int t) {
  // cout << top << " " << next << endl;
  for (int i = 0; i < t; i++) {
    // cout << " " << LL1_DFA[i].curState << " " << LL1_DFA[i].symbol << " " <<
    // LL1_DFA[i].type_1 << " " <<LL1_DFA[i].int1 << endl;
    if (LL1_DFA[i].curState == top && LL1_DFA[i].symbol == next &&
        LL1_DFA[i].type_1 == "shift") {
      return LL1_DFA[i].int1;
    }
  }
  return -1;
}

// Enhanced error reporting from DFA algorithm perspective (aggregated)
void reportDFAError(int tokenNumber, const string &token, const string &lexeme,
                    int currentState, const vector<LL1_DFA_NODE> &LL1_DFA,
                    const vector<transitionRule> &productionRules, int t) {

  errorLog() << "[Stage: Parsing] Failure" << '\n';
  errorLog() << "  At token #: " << tokenNumber << '\n';
  errorLog() << "  Saw token: '" << token << "'";
  if (!lexeme.empty() && lexeme != token) {
    errorLog() << " (lexeme '" << lexeme << "')";
  }
  errorLog() << '\n';

  errorLog() << "DFA Algorithm Analysis:" << '\n';
  errorLog() << "  Current DFA State: " << currentState << '\n';
  errorLog() << "  Input Token: '" << token << "'" << '\n';

  // Find all valid transitions from current state
  errorLog() << "  Valid transitions from state " << currentState << ":" << '\n';
  bool foundValidTransitions = false;
  for (int i = 0; i < t; i++) {
    if (LL1_DFA[i].curState == currentState) {
      errorLog() << "    - On '" << LL1_DFA[i].symbol << "' -> ";
      if (LL1_DFA[i].type_1 == "shift") {
        errorLog() << "SHIFT to state " << LL1_DFA[i].int1;
      } else if (LL1_DFA[i].type_1 == "reduce") {
        errorLog() << "REDUCE using rule " << LL1_DFA[i].int1;
      }
      errorLog() << '\n';
      foundValidTransitions = true;
    }
  }

  if (!foundValidTransitions) {
    errorLog() << "    - No valid transitions found (dead state)" << '\n';
  }

  // Explain why the token is invalid
  errorLog() << "  Algorithm Decision:" << '\n';
  errorLog() << "    The DFA searched through all " << t << " transition rules" << '\n';
  errorLog() << "    looking for a valid transition from state " << currentState << '\n';
  errorLog() << "    on token '" << token << "', but found NONE." << '\n';

  // Show what the algorithm expected
  errorLog() << "  What the DFA expected:" << '\n';
  vector<string> expectedTokens;
  for (int i = 0; i < t; i++) {
    if (LL1_DFA[i].curState == currentState) {
      expectedTokens.push_back(LL1_DFA[i].symbol);
    }
  }

  if (!expectedTokens.empty()) {
    errorLog() << "    Valid tokens: ";
    for (size_t i = 0; i < expectedTokens.size(); i++) {
      errorLog() << "'" << expectedTokens[i] << "'";
      if (i < expectedTokens.size() - 1)
        errorLog() << ", ";
    }
    errorLog() << '\n';
  } else {
    errorLog() << "    No valid tokens (parser reached invalid state)" << '\n';
  }

  // Show grammar rules that could potentially help
  errorLog() << "  Grammar Rule Analysis:" << '\n';
  errorLog() << "    The parser looked through " << productionRules.size() << " grammar rules" << '\n';
  errorLog() << "    to find a production that could handle token '" << token << "'" << '\n';

  bool foundRelevantRules = false;
  for (size_t i = 0; i < productionRules.size(); i++) {
    const auto &rule = productionRules[i];
    for (const auto &ruleToken : rule.tokens) {
      if (ruleToken == token) {
        errorLog() << "    Rule " << i << ": " << rule.non_terminal << " -> ";
        for (size_t j = 0; j < rule.tokens.size(); j++) {
          errorLog() << rule.tokens[j];
          if (j < rule.tokens.size() - 1)
            errorLog() << " ";
        }
        errorLog() << '\n';
        foundRelevantRules = true;
        break;
      }
    }
  }

  if (!foundRelevantRules) {
    errorLog() << "    No grammar rules contain token '" << token << "'" << '\n';
  }

  // Provide algorithm-specific explanation
  errorLog() << "  Why parsing failed:" << '\n';
  errorLog() << "    1. The DFA algorithm maintains a state stack" << '\n';
  errorLog() << "    2. From current state " << currentState << ", it looked for transitions" << '\n';
  errorLog() << "    3. No transition rule matched: (state=" << currentState << ", token='" << token << "')" << '\n';
  errorLog() << "    4. The algorithm cannot proceed without a valid transition" << '\n';
  errorLog() << "    5. This violates the grammar's syntax rules" << '\n';

  // Suggest fixes based on DFA perspective
  errorLog() << "  Algorithm suggests:" << '\n';
  if (!expectedTokens.empty()) {
    errorLog() << "    - Replace '" << token << "' with one of the expected tokens" << '\n';
    errorLog() << "    - Check if you're missing a required token before this position" << '\n';
  }
  errorLog() << "    - Verify the grammar allows '" << token << "' in this context" << '\n';
  errorLog() << "    - Check for missing or misplaced tokens in the input" << '\n';
}

string performSyntaxAnalysis(string scannedContent) {

  int i, numTerm, numNonTerm, numRules, s, t;
  ifstream infile("grammar/WLP4.lr1");
  // read the number of terminals and move to the next line
  infile >> numTerm;
  skipLine(infile);
  // cout << numTerm << endl;
  //  skip the lines containing the terminals
  int f = 0;
  for (i = 0; i < numTerm; i++) {
    string buf = skipLine(infile);
    map_symbol_to_integer[buf] = f++;
  }

  // read the number of non-terminals and move to the next line
  infile >> numNonTerm;
  skipLine(infile);
  // cout << numNonTerm << endl;
  //  skip the lines containing the non-terminals
  for (i = 0; i < numNonTerm; i++) {
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
  for (i = 0; i < numRules; i++) {
    transitionRule curRule;
    string lineStr = skipLine(infile);
    curRule.instruction = lineStr;
    std::stringstream line(lineStr);
    line >> curRule.non_terminal;
    curRule.number_of_popped_symbols = 0;
    while (line >> lineStr) {
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
   When reading infile the infile string it would do it string by string and
   keep track of the states infile a queue There would be a queue for the states
   and anouther queue for the symbols or there can just be one queue and pop and
   accept nilly willyny I want to make a 2d array, dynamic 2d array infile c++
   is hard
  */
  vector<LL1_DFA_NODE> LL1_DFA;
  for (int i = 0; i < t; i++) {
    int curState, int1;
    string symbol, type_1;
    infile >> curState >> symbol >> type_1 >> int1;
    LL1_DFA.push_back({curState, symbol, type_1, int1});
  }

  stack<int> stateStack; // the current state is ALWAYS the first element in the
                         // integer stack
  stateStack.push(0);
  stack<parseTree *> symbolStack; // this is the stack of all the read symbols
  symbolStack.push(new parseTree{"BOF BOF"});
  int nextState = delta(stateStack.top(), "BOF", LL1_DFA, t);
  stateStack.push(nextState);
  int readIn = 0;
  // cout << "Accepting inputs right now" << endl;
  // make string stream instead of cin
  string parseResult = "";
  stringstream ss(scannedContent);
  while (true) {
    string token, lexeme;
    if (!(ss >> token) || !(ss >> lexeme)) { // Remember, operator! === fail()
      if (ss.eof()) {
        token = "EOF";
        lexeme = token;
      }
      ss.clear();
      ss.ignore();
    }

    // Handle PUTCHAR and GETCHAR by converting them for parsing
    string originalToken = token;
    if (token == "PUTCHAR") {
      // Convert PUTCHAR to PRINTLN for parsing since they have the same pattern
      token = "PRINTLN";
    } else if (token == "GETCHAR") {
      // Convert GETCHAR to ID for parsing since it follows the ID LPAREN RPAREN
      // pattern
      token = "ID";
    }

    // cout << "Currently at " << symbolStack.top() << " Next is " << token <<
    // endl; cout << "State at " << stateStack.top() << endl;
    //   Ask is it a reduce state
    while (Reduce(stateStack.top(), token, LL1_DFA, t) != -1) {
      // cout << "State: " << stateStack.top() << " is reduce state" << endl;
      int rule = Reduce(stateStack.top(), token, LL1_DFA, t);
      string B = productionRules[rule].non_terminal;
      string ruleInstruction = productionRules[rule].instruction;

      // Replace ID with GETCHAR/PUTCHAR in the rule instruction if needed
      if (originalToken == "GETCHAR" &&
          ruleInstruction.find("factor ID LPAREN RPAREN") != string::npos) {
        ruleInstruction = "factor GETCHAR LPAREN RPAREN";
      } else if (originalToken == "PUTCHAR" &&
                 ruleInstruction.find(
                     "statement PRINTLN LPAREN expr RPAREN SEMI") !=
                     string::npos) {
        ruleInstruction = "statement PUTCHAR LPAREN expr RPAREN SEMI";
      }

      parseTree *addOn = new parseTree{ruleInstruction};
      for (int foo = 0; foo < productionRules[rule].number_of_popped_symbols;
           foo++) {
        addOn->childrens.push_front(symbolStack.top());
        symbolStack.pop();
        stateStack.pop();
      }

      // Update the rule instruction after building the tree
      if (ruleInstruction == "factor ID LPAREN RPAREN") {
        // Check if the first child is GETCHAR
        if (!addOn->childrens.empty()) {
          parseTree *firstChild = addOn->childrens.front();
          if (firstChild->element.find("GETCHAR") == 0) {
            addOn->element = "factor GETCHAR LPAREN RPAREN";
          }
        }
      } else if (ruleInstruction ==
                 "statement PRINTLN LPAREN expr RPAREN SEMI") {
        // Check if the first child is PUTCHAR
        if (!addOn->childrens.empty()) {
          parseTree *firstChild = addOn->childrens.front();
          if (firstChild->element.find("PUTCHAR") == 0) {
            addOn->element = "statement PUTCHAR LPAREN expr RPAREN SEMI";
          }
        }
      }

      symbolStack.push(addOn);
      stateStack.push(delta(stateStack.top(), B, LL1_DFA, t));
    }

    symbolStack.push(new parseTree{originalToken + " " + lexeme});
    nextState = delta(stateStack.top(), token, LL1_DFA, t);
    if (nextState == -1) {
      // Enhanced DFA error message (aggregated)
      reportDFAError(readIn + 1, token, lexeme, stateStack.top(), LL1_DFA,
                     productionRules, t);
      while (!symbolStack.empty()) {
        delete symbolStack.top();
        symbolStack.pop();
      }
      return "Parse failed";
    }
    stateStack.push(nextState);
    readIn++;
    if (token == "EOF") { // Remember, operator! === fail()
      break;
    }
  }
  parseTree *root = new parseTree{start};
  for (i = 0; i < numRules; i++) {
    if (productionRules[i].non_terminal == start) {
      root->element = productionRules[i].instruction;
    }
  }
  while (!symbolStack.empty()) {
    root->childrens.push_front(symbolStack.top());
    symbolStack.pop();
  }
  parseResult = "Parse Failed";
  parseResult = root->printParseTree();
  delete root;
  return parseResult;
}

// Configurable word size for different architectures
#ifndef WORD_SIZE
#define WORD_SIZE                                                              \
  4 // Default to 4 bytes (32-bit), can be overridden with -DWORD_SIZE=16 for
    // 128-bit
#endif
// Skip the grammar part of the input.

// T should keep curState while a should keep the transaction rules

// Reads a .cfg file and prints the derivation
// without leading or trailing spaces.
bool allCaps(string s) {
  bool ret = true;
  int i = 0;
  while (s[i]) {
    ret = ret & (isupper(s[i]) > 0);
    i++;
  }
  return ret;
}

class Tree {
public:
  string rule;
  vector<Tree *> children;
  vector<string> ch;
  ~Tree() {
    for (auto k : children) {
      delete k;
    }
  }
  void printParseTree() {
    cout << rule << endl;
    for (Tree *a : children) {
      a->printParseTree();
    }
  }
};

map<string, pair<vector<string>, map<string, string>>> tables;
map<string, map<string, int>> method_offset_tabel;

string generatePushInstruction(string regValue) {
  string ret = ";  ================ Pushing " + regValue + " to stack ================ \n";
  ret += "sw " + regValue + ", -" + to_string(WORD_SIZE) + "($30) ; Store " +
         regValue + " at top of stack\n";
  ret += "sub $30, $30, $" + to_string(WORD_SIZE) +
         " ; Decrement stack pointer by " + to_string(WORD_SIZE) + " bytes\n";
  return ret;
}
string generatePopInstruction(string regValue) {
  string ret = ";  ================ Popping " + regValue + " from stack ================ \n";
  ret += "add $30, $30, $" + to_string(WORD_SIZE) +
         " ; Increment stack pointer by " + to_string(WORD_SIZE) + " bytes\n";
  ret += "lw " + regValue + ",-" + to_string(WORD_SIZE) + "($30) ; Load " +
         regValue + " from top of stack\n";
  return ret;
}
// generateAssemblyCode(a) ; results in lw $3, 0($29), which we will write as $3
// <- a
//         ; results in lw $3, shift($29), which we will write as $3 <- a

void extractDeclarationInfo(Tree *dcl, string &IDValue, string &tempType) {
  Tree *TYPE = dcl->children[0];
  Tree *ID = dcl->children[1];
  IDValue = ID->rule.substr(3);
  if (TYPE->children.size() == 1) {
    // INT
    tempType = "int";
  }
  if (TYPE->children.size() == 2) {
    // INT *
    tempType = "int*";
  }
}
int singleDCL(Tree *dcl, map<string, string> &symbolTables) {
  string IDValue, tempType;
  extractDeclarationInfo(dcl, IDValue, tempType);

  if (symbolTables.count(IDValue)) {
    errorLog() << "[Stage: Codegen] ERROR: Duplicate initialization" << '\n';
    return 1;
  }
  symbolTables[IDValue] = tempType;
  return 0;
}

// Function name -> {parameters, ID -> type }
// ID -> type
int dclsLinkedList(Tree *dcls, map<string, string> &symbolTables) {
  int sum = 0;
  while (dcls->children.size() > 0) {
    // pase dcl
    string tempType, IDValue;
    extractDeclarationInfo(dcls->children[1], IDValue, tempType);
    if (tempType == "int" && dcls->ch[3] == "NULL") {
      errorLog() << "[Stage: Codegen] ERROR: int = NULL invalid" << '\n';
      return 1;
    }

    if (tempType == "int*" && dcls->ch[3] == "NUM") {
      errorLog() << "[Stage: Codegen] ERROR: int* = NUM invalid" << '\n';
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
The type of an lvalue deriving LPAREN lvalue RPAREN is the same as the type of
the derived lvalue. The type of a factor deriving AMP lvalue is int*. The type
of the derived lvalue (i.e. the one preceded by AMP) must be int. The type of a
factor or lvalue deriving STAR factor is int. The type of the derived factor
(i.e. the one preceded by STAR) must be int*.

*/
void makeParamTypeList(Tree *arglist, map<string, string> &symbolTables,
                       vector<string> &typeList);
// retuns "invalid, int or int*"
string exprType(Tree *expr, map<string, string> &symbolTables) {
  // expr->printParseTree();

  string ret = "invalid", a, b;
  if (expr->rule == "expr expr PLUS term") {
    a = exprType(expr->children[0], symbolTables);
    b = exprType(expr->children[2], symbolTables);
    if (a == "int" && b == "int") {
      ret = "int";
    }
    if (a == "int*" && b == "int") {
      ret = "int*";
    }
    if (a == "int" && b == "int*") {
      ret = "int*";
    }
  } else if (expr->rule == "factor ID" || expr->rule == "lvalue ID") {
    Tree *ID_Node = expr->children[0];
    string varName = ID_Node->rule.substr(3);
    ret = symbolTables[varName];
  } else if (expr->rule == "expr term") {
    a = exprType(expr->children[0], symbolTables);
    ret = a;
  } else if (expr->rule == "expr expr MINUS term") {
    a = exprType(expr->children[0], symbolTables);
    b = exprType(expr->children[2], symbolTables);
    if (a == "int" && b == "int") {
      ret = "int";
    }
    if (a == "int*" && b == "int*") {
      ret = "int";
    }
    if (a == "int*" && b == "int") {
      ret = "int*";
    }
  } else if (expr->rule == "term factor") {
    a = exprType(expr->children[0], symbolTables);
    ret = a;
  } else if (expr->rule == "term term STAR factor") {
    a = exprType(expr->children[0], symbolTables);
    b = exprType(expr->children[2], symbolTables);
    if (a == "int" && b == "int") {
      ret = "int";
    }
  } else if (expr->rule == "term term SLASH factor") {
    a = exprType(expr->children[0], symbolTables);
    b = exprType(expr->children[2], symbolTables);
    if (a == "int" && b == "int") {
      ret = "int";
    }
  } else if (expr->rule == "term term PCT factor") {
    a = exprType(expr->children[0], symbolTables);
    b = exprType(expr->children[2], symbolTables);
    if (a == "int" && b == "int") {
      ret = "int";
    }
  } else if (expr->rule == "factor NUM") {
    ret = "int";
  } else if (expr->rule == "factor NULL") {
    ret = "int*";
  } else if (expr->rule == "factor LPAREN expr RPAREN" ||
             expr->rule == "lvalue LPAREN lvalue RPAREN") {
    ret = exprType(expr->children[1], symbolTables);
  } else if (expr->rule == "factor AMP lvalue") {
    a = exprType(expr->children[1], symbolTables);
    /*
        lvalue ID
        lvalue STAR factor
        lvalue LPAREN lvalue RPAREN
    */
    // a must be an int lvalue
    // getting the pointer
    // Need new method to find the value of lvalue
    if (a == "int") {
      ret = "int*";
    }
    // cerr << expr->rule << " " << ret << endl;
  } else if (expr->rule == "factor STAR factor" ||
             expr->rule == "lvalue STAR factor") {
    // this is dereference factor muse be an int* orelse it is invalid
    a = exprType(expr->children[1], symbolTables); // what does this do?Î

    if (a == "int*") {
      ret = "int";
    }
  } else if (expr->rule == "factor NEW INT LBRACK expr RBRACK") {
    a = exprType(expr->children[3], symbolTables);
    if (a == "int") {
      ret = "int*";
    }
  } else if (expr->rule == "factor ID LPAREN RPAREN") {
    /*
    The type of a factor deriving ID LPAREN RPAREN is int.
    The procedure whose name is ID must have an empty signature.
    */
    string functionName = expr->children[0]->rule.substr(3);
    if (tables[functionName].first.size() == 0)
      ret = "int";

    // both just return the value that ID returns
  } else if (expr->rule == "factor ID LPAREN arglist RPAREN") {
    /*
    The type of a factor deriving ID LPAREN arglist RPAREN is int.
    The procedure whose name is ID must have a signature whose length is equal
    to the number of expr strings (separated by COMMA) that are derived from
    arglist. Further the types of these expr strings must exactly match, in
    order, the types in the procedure's signature.
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
    for (int i = 0; i < len2 && allSameElements; i++) {
      allSameElements =
          allSameElements & (typeList[i] == tables[functionName].first[i]);
    }
    if (allSameElements)
      ret = "int";

    // both just return the value that ID returns
  }

  return ret;
}
void makeParamTypeList(Tree *arglist, map<string, string> &symbolTables,
                       vector<string> &typeList) {
  while (true) {
    string a = exprType(arglist->children[0], symbolTables);
    typeList.push_back(a);
    if (arglist->rule == "arglist expr") {
      break;
    }
    if (arglist->rule == "arglist expr COMMA arglist") {
      arglist = arglist->children[2];
    }
  }
}
bool allValidVar(Tree *pr, map<string, string> &symbolTables,
                 string &illegalVar) {
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
  while (ssin >> strIn) {
    splitedRule.push_back(strIn);
    children++;
    // this is getting the children of the stack once it poped it goes to the
    // main stack
  }
  if (firstNum == "dcls" || firstNum == "dcl" || firstNum == "params") {
    return ret;
  } else if (pr->rule == "factor ID") {
    Tree *ID = pr->children[0];
    string compairedString = ID->rule.substr(3);
    ret = (symbolTables.count(compairedString) > 0);
    if (symbolTables.count(compairedString) == 0) {
      illegalVar = compairedString;
      ret = false;
    }
    return ret;
  } else if (pr->rule == "factor ID LPAREN RPAREN" ||
             pr->rule == "factor ID LPAREN arglist RPAREN") {
    Tree *ID = pr->children[0];
    string compairedString = ID->rule.substr(3);
    // look for ID in the FUNCTION call Look in tabels
    ret = false;
    if (symbolTables.count(compairedString)) {
      return ret;
    }
    illegalVar = compairedString;
    for (auto const &pair_0 : tables) {
      if (compairedString == pair_0.first) {
        ret = true;
        illegalVar = "";
      }
    }
    if (pr->rule == "factor ID LPAREN arglist RPAREN") {
      bool buf = allValidVar(pr->children[2], symbolTables, illegalVar);
      ret = ret & buf;
    }
    return ret;
  } else {
    for (size_t k = 0; k < pr->children.size(); k++) {
      if (splitedRule[k] == "ID" && firstNum == "procedure")
        continue;
      // check function names

      bool buf = allValidVar(pr->children[k], symbolTables, illegalVar);
      ret = ret & buf;
    }
  }
  return ret;
}

bool checkStatementTypeValidity(Tree *statements,
                                map<string, string> &symbolTables) {
  bool ret = true;

  if (statements->rule == "statement lvalue BECOMES expr SEMI") {
    Tree *leftexpr = statements->children[0];
    Tree *rightexpr = statements->children[2];
    string leftexpr_typ = exprType(leftexpr, symbolTables);
    string rightexpr_typ = exprType(rightexpr, symbolTables);
    ret = ret & (rightexpr_typ == leftexpr_typ && leftexpr_typ != "invalid");
    return ret;
  } else if (statements->rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {

    string exprTpye_thus = exprType(statements->children[2], symbolTables);

    ret = (exprTpye_thus == "int");
    return ret;
  } else if (statements->rule == "statement DELETE LBRACK RBRACK expr SEMI") {
    string exprTpye_thus = exprType(statements->children[3], symbolTables);
    ret = (exprTpye_thus == "int*");
    return ret;
  } else if (statements->rule == "statements") {
    return ret;
  } else if (statements->rule == "statements statements statement") {
    ret = checkStatementTypeValidity(statements->children[0], symbolTables) &
          checkStatementTypeValidity(statements->children[1], symbolTables);
    return ret;
  } else if (statements->rule == "statement lvalue BECOMES expr SEMI") {
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
  } else {
    int loopsize = statements->children.size();
    for (int k = 0; k < loopsize; k++) {
      if (statements->ch[k] == "statements") {

        ret = ret &
              checkStatementTypeValidity(statements->children[k], symbolTables);
      }
      if (statements->ch[k] == "test") {
        // test operation tests
        Tree *test = statements->children[k];
        Tree *leftexpr = test->children[0];
        Tree *rightexpr = test->children[2];
        string leftexpr_typ = exprType(leftexpr, symbolTables);
        string rightexpr_typ = exprType(rightexpr, symbolTables);
        ret =
            ret & (rightexpr_typ == leftexpr_typ && leftexpr_typ != "invalid");
      }
    }
  }
  return ret;
}

int buildTable(Tree *pr,
               map<string, pair<vector<string>, map<string, string>>> &tables) {
  int sum = 0;
  stringstream ssin(pr->rule);

  if (pr->rule == "start BOF procedures EOF") {
    sum += buildTable(pr->children[1], tables);
  }
  if (pr->rule == "procedures procedure procedures") {
    sum += buildTable(pr->children[0], tables);
    sum += buildTable(pr->children[1], tables);
  }
  if (pr->rule == "procedures main") {
    sum += buildTable(pr->children[0], tables);
  }
  if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls "
                  "statements RETURN expr SEMI RBRACE") {
    // Base case
    string methodName = "wain";
    string tempType, ID;
    int ind_lis[]{3, 5};
    for (int i_buf = 0; i_buf < 2; i_buf++) {
      extractDeclarationInfo(pr->children[ind_lis[i_buf]], ID, tempType);
      sum += singleDCL(pr->children[ind_lis[i_buf]], tables["wain"].second);
      tables["wain"].first.push_back(tempType);
    }
    extractDeclarationInfo(pr->children[5], ID, tempType);
    if (tempType != "int") {
      errorLog() << "[Stage: Codegen] ERROR: second variable in main must be an INTEGER varname: "
                << tempType << endl;
      return 1;
    }

    sum += dclsLinkedList(pr->children[8], tables["wain"].second);
    string illegal = "";
    bool cmp = allValidVar(pr, tables["wain"].second, illegal);
    if (!cmp) {
      errorLog() << "[Stage: Codegen] ERROR: uninitialized variable found in main" << '\n'
                << " "
                << " (" << illegal << ")" << endl;
      return 1;
    }
  }
  if (pr->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls "
                  "statements RETURN expr SEMI RBRACE") {
    string methodName = pr->children[1]->rule.substr(3);
    Tree *paramlist = NULL;
    // params -> nothing
    if (pr->children[3]->rule == "params") {
    } else {
      // Params -> paramlist
      paramlist = pr->children[3]->children[0];
    }
    if (tables.count(methodName) > 0) {
      errorLog() << "[Stage: Codegen] ERROR: duplicate method found in " << methodName << '\n';
      return 1;
    }
    while (paramlist != NULL) {
      // pase dcl
      string IDValue;
      string tempType;
      extractDeclarationInfo(paramlist->children[0], IDValue, tempType);
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
    if (!cmp) {
      errorLog() << "[Stage: Codegen] ERROR: use of undeclared variable found in " << methodName
           << " (" << illegal << ")" << endl;
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
int checkProgramTypeValidity(
    Tree *pr, map<string, pair<vector<string>, map<string, string>>> &tables) {
  int sum = 0;
  stringstream ssin(pr->rule);

  if (pr->rule == "start BOF procedures EOF") {
    sum += checkProgramTypeValidity(pr->children[1], tables);
  }
  if (pr->rule == "procedures procedure procedures") {
    sum += checkProgramTypeValidity(pr->children[0], tables);
    sum += checkProgramTypeValidity(pr->children[1], tables);
  }
  if (pr->rule == "procedures main") {
    sum += checkProgramTypeValidity(pr->children[0], tables);
  }
  if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls "
                  "statements RETURN expr SEMI RBRACE") {
    string methodName = "wain";
    for (size_t i = 0; i < pr->children.size(); i++) {
      if (pr->ch[i] == "statements") {
        bool valid = checkStatementTypeValidity(pr->children[i],
                                                tables[methodName].second);
        if (!valid) {
          errorLog() << "[Stage: Codegen] ERROR: Invalid statement in " << methodName << '\n';
          return 1;
        }
      }
      if (pr->ch[i] == "expr") {
        string exprTpye_thus =
            exprType(pr->children[i], tables[methodName].second);
        if (exprTpye_thus == "invalid") {
          errorLog() << "[Stage: Codegen] ERROR: Invalid return statement " << methodName << '\n';
          return 1;
        }
        if (exprTpye_thus == "int*") {
          errorLog() << "[Stage: Codegen] ERROR: Invalid return value int* " << methodName
                    << endl;
          return 1;
        }
      }
    }
  }
  if (pr->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls "
                  "statements RETURN expr SEMI RBRACE") {
    string methodName = pr->children[1]->rule.substr(3);
    for (size_t i = 0; i < pr->children.size(); i++) {
      if (pr->ch[i] == "statements") {
        bool valid = checkStatementTypeValidity(pr->children[i],
                                                tables[methodName].second);
        if (!valid) {
          errorLog() << "[Stage: Codegen] ERROR: Invalid statement in " << methodName << '\n';
          return 1;
        }
      }
      if (pr->ch[i] == "expr") {
        string exprTpye_thus =
            exprType(pr->children[i], tables[methodName].second);
        if (exprTpye_thus == "invalid") {
          errorLog() << "[Stage: Codegen] ERROR: Invalid return statement " << methodName << '\n';
          return 1;
        }
        if (exprTpye_thus == "int*") {
          errorLog() << "[Stage: Codegen] ERROR: Invalid return value int* " << methodName
                    << endl;
          return 1;
        }
      }
    }
  }
  return sum;
}

//================================================================================================================================
string generateAssemblyCode(Tree *expr, map<string, int> &offset_tabel,
                            map<string, string> &symbolTables) {
  string ret = "";
  if (expr->rule == "expr term") {
    ret += generateAssemblyCode(expr->children[0], offset_tabel, symbolTables);
  } else if (expr->rule == "term factor") {
    ret += generateAssemblyCode(expr->children[0], offset_tabel, symbolTables);
  } else if (expr->rule == "factor ID") {
    Tree *ID = expr->children[0];
    string a = ID->ch[0];
    // Load variable from stack frame at calculated offset
    int shift = offset_tabel[a];
    ret += "lw $3, " + to_string(shift) + "($29) ; Load variable '" + a +
           "' from stack frame into $3\n";
  } else if (expr->rule == "factor LPAREN expr RPAREN") {
    ret += "; " + expr->rule + "\n";
    Tree *expr_children = expr->children[1];
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
  } else if (expr->rule == "expr expr PLUS term") {
    ret += "; Addition operation: " + expr->rule + "\n";
    Tree *expr_children = expr->children[0];
    Tree *term_children = expr->children[2];

    string firstExprType, secondExprType;
    firstExprType = exprType(expr_children, symbolTables);
    secondExprType = exprType(term_children, symbolTables);

    // Evaluate left operand and store result in $3
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3"); // Save left operand on stack

    // Evaluate right operand and store result in $3
    ret += generateAssemblyCode(term_children, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5"); // Restore left operand to $5

    // Now: $5 = left operand, $3 = right operand
    ret += "; Adding " + firstExprType + " + " + secondExprType + "\n";

    // Handle pointer arithmetic: multiply int by word size when adding to
    // pointer
    if (firstExprType == "int" && secondExprType == "int*") {
      ret += "mult $" + to_string(WORD_SIZE) +
             ", $5 ; Scale int by word size for pointer arithmetic\n";
      ret += "mflo $5 ; Move multiplication result back to $5\n";
    }
    if (firstExprType == "int*" && secondExprType == "int") {
      ret += "mult $" + to_string(WORD_SIZE) +
             ", $3 ; Scale int by word size for pointer arithmetic\n";
      ret += "mflo $3 ; Move multiplication result back to $3\n";
    }
    ret += "add $3, $5, $3 ; Perform addition: $3 = $5 + $3\n";
  } else if (expr->rule == "expr expr MINUS term") {

    Tree *expr_children = expr->children[0];
    Tree *term_children = expr->children[2];
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += "; start term children " + expr->rule + "\n";
    ret += generateAssemblyCode(term_children, offset_tabel, symbolTables);
    ret += "; end term children\n";
    ret += generatePopInstruction("$5");
    // remember $5  =  value of expr_children
    //          $3  =  value of term_children

    string firstExprType, secondExprType;
    firstExprType = exprType(expr_children, symbolTables);
    secondExprType = exprType(term_children, symbolTables);
    cout << "; =========== " << firstExprType << " - " << secondExprType
         << endl;
    if (firstExprType == "int*" && secondExprType == "int") {

      ret += "mult $" + to_string(WORD_SIZE) + ", $3\n";
      ret += "mflo $3\n";
    }
    ret += "sub $3, $5, $3\n";
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "divu $3, $" + to_string(WORD_SIZE) + "\n";
      ret += "mflo $3\n";
    }
  } else if (expr->rule == "term term STAR factor") {
    Tree *expr_children = expr->children[0];
    Tree *term_children = expr->children[2];
    ret += "; " + expr_children->rule + " " + term_children->rule + "\n";
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(term_children, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    // remember $5  =  value of expr_children
    //          $3  =  value of term_children
    ret += "mult $5, $3\n";
    ret += "mflo $3\n";
  } else if (expr->rule == "term term SLASH factor") {
    Tree *expr_children = expr->children[0];
    Tree *term_children = expr->children[2];
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(term_children, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    // remember $5  =  value of expr_children
    //          $3  =  value of term_children
    ret += "div $5, $3\n";
    ret += "mflo $3\n";
  } else if (expr->rule == "term term PCT factor") {
    Tree *expr_children = expr->children[0];
    Tree *term_children = expr->children[2];
    ret += generateAssemblyCode(expr_children, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(term_children, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    // remember $5  =  value of expr_children
    //          $3  =  value of term_children
    ret += "div $5, $3\n";
    ret += "mfhi $3\n";
  } else if (expr->rule == "factor NUM") {
    Tree *NUM = expr->children[0];
    string val = NUM->ch[0];
    ret += "lis $3 ; Load immediate value into $3\n";
    ret += ".word " + val + " ; Store constant " + val + " in $3\n";
  } else if (expr->rule == "statements statements statement") {
    Tree *statements_1 = expr->children[0];
    Tree *statements_2 = expr->children[1];
    ret += generateAssemblyCode(statements_1, offset_tabel, symbolTables);
    ret += generateAssemblyCode(statements_2, offset_tabel, symbolTables);
  } else if (expr->rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
    Tree *expr_1 = expr->children[2];
    ret += generatePushInstruction("$1"); // Save $1 register
    ret += generateAssemblyCode(expr_1, offset_tabel,
                                symbolTables); // Evaluate expression
    ret += "add $1, $3, $0 ; Copy expression result to $1 for print function\n";
    ret += generatePushInstruction("$31"); // Save return address
    ret += "lis $5 ; Load address of print function\n";
    ret += ".word print ; Print function label\n";
    ret += "jalr $5 ; Call print function\n";
    ret += generatePopInstruction("$31"); // Restore return address
    ret += generatePopInstruction("$1");  // Restore $1 register
  } else if (expr->rule == "statement PUTCHAR LPAREN expr RPAREN SEMI") {
    Tree *expr_1 = expr->children[2];
    ret += generatePushInstruction("$1"); // Save $1 register
    ret += generateAssemblyCode(expr_1, offset_tabel,
                                symbolTables); // Evaluate expression
    ret +=
        "add $1, $3, $0 ; Copy expression result to $1 for putchar function\n";
    ret += generatePushInstruction("$31"); // Save return address
    ret += "lis $5 ; Load address of putchar function\n";
    ret += ".word putchar ; Putchar function label\n";
    ret += "jalr $5 ; Call putchar function\n";
    ret += generatePopInstruction("$31"); // Restore return address
    ret += generatePopInstruction("$1");  // Restore $1 register
  } else if (expr->rule == "statement lvalue BECOMES expr SEMI") {
    ret += "; Assignment statement: " + expr->rule + "\n";
    Tree *lvalue = expr->children[0];
    Tree *expr_1 = expr->children[2];

    // Evaluate the right-hand side expression
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);

    if (lvalue->rule == "lvalue STAR factor") {
      // Dereference assignment: *ptr = value
      Tree *factor = lvalue->children[1];
      ret += generatePushInstruction("$3"); // Save the value to be assigned
      ret += generateAssemblyCode(factor, offset_tabel,
                                  symbolTables); // Get pointer address
      ret += generatePopInstruction("$5");       // Restore the value
      ret += "sw $5, 0($3) ; Store value at pointer address (dereference "
             "assignment)\n";
    } else if (lvalue->rule == "lvalue ID") {
      // Variable assignment: var = value
      Tree *ID = lvalue->children[0];
      string a = ID->ch[0];
      int shift = offset_tabel[a];
      ret += "sw $3, " + to_string(shift) +
             "($29) ; Store value in variable '" + a + "' at stack offset " +
             to_string(shift) + "\n";
    }
  } else if (expr->rule == "lvalue LPAREN lvalue RPAREN") {
    Tree *lvalue_1 = expr->children[1];
    ret += generateAssemblyCode(lvalue_1, offset_tabel, symbolTables);
  } else if (expr->rule == "test expr LT expr") {
    // for pointer compairson use unsigned shit

    // store the value of expr in offset(char)($29)
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];

    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $3, $5, $3\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $3, $5, $3\n";
    }
  } else if (expr->rule == "test expr GT expr") {
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");

    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $3, $3, $5\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $3, $3, $5\n";
    }
  } else if (expr->rule == "test expr GE expr") {
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $3, $5, $3\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $3, $5, $3\n";
    }
    ret += "sub $3, $11, $3 ; negation\n";
  } else if (expr->rule == "test expr LE expr") {
    // store the value of expr in offset(char)($29)
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");

    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    ret += "; " + firstExprType + " <= " + secondExprType + "\n";
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $3, $3, $5\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $3, $3, $5\n";
    }
    cout << "sub $3, $11, $3 ; negation" << endl;
  } else if (expr->rule == "test expr NE expr") {
    // store the value of expr in offset(char)($29)
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");

    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $6, $3, $5 ; $6 = $3 < $5\n";
      ret += "sltu $7, $5, $3 ; $7 = $5 < $3\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $6, $3, $5 ; $6 = $3 < $5\n";
      ret += "slt $7, $5, $3 ; $7 = $5 < $3\n";
    }

    ret += "add $3, $6, $7 ; Sum $6 and $7\n";
  } else if (expr->rule == "test expr EQ expr") {
    Tree *expr_1 = expr->children[0];
    Tree *expr_2 = expr->children[2];
    ret += generateAssemblyCode(expr_1, offset_tabel, symbolTables);
    ret += generatePushInstruction("$3");
    ret += generateAssemblyCode(expr_2, offset_tabel, symbolTables);
    ret += generatePopInstruction("$5");
    string firstExprType, secondExprType;
    firstExprType = exprType(expr_1, symbolTables);
    secondExprType = exprType(expr_2, symbolTables);
    if (firstExprType == "int*" && secondExprType == "int*") {

      ret += "sltu $6, $3, $5 ; $6 = $3 < $5\n";
      ret += "sltu $7, $5, $3 ; $7 = $5 < $3\n";
    } else if (firstExprType == "int" && secondExprType == "int") {

      ret += "slt $6, $3, $5 ; $6 = $3 < $5\n";
      ret += "slt $7, $5, $3 ; $7 = $5 < $3\n";
    }

    ret += "add $3, $6, $7 ; Sum $6 and $7\n";
    cout << "sub $3, $11, $3 ; negation" << endl;
  }

  else if (expr->rule ==
           "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
    string loopPointer = "loop" + to_string(whileCounter);
    string endWhile = "endWhile" + to_string(whileCounter);
    whileCounter++;
    ret += loopPointer + ":\n";
    Tree *test = expr->children[2];
    Tree *statements = expr->children[5];
    ret += "; BEGIN TEST\n";
    ret += generateAssemblyCode(test, offset_tabel, symbolTables);
    ret += "; END TEST\n";
    ret += "beq $3, $0, " + endWhile + "\n";
    ret += generateAssemblyCode(statements, offset_tabel, symbolTables);
    ret += "beq $0, $0, " + loopPointer + "\n";
    ret += endWhile + ":\n";
  }

  else if (expr->rule == "statement IF LPAREN test RPAREN LBRACE statements "
                         "RBRACE ELSE LBRACE statements RBRACE") {
    // whileCounter
    //  store the value of expr in offset(char)($29)
    string elsePointer = "else" + to_string(ifCounter);
    string endif = "endif" + to_string(ifCounter);
    ifCounter++;
    Tree *test = expr->children[2];
    Tree *statements_1 = expr->children[5];
    Tree *statements_2 = expr->children[9];
    ret += generateAssemblyCode(test, offset_tabel, symbolTables);
    ret += "beq $3, $0, " + elsePointer + "\n";
    ret += generateAssemblyCode(statements_1, offset_tabel, symbolTables);
    ret += "beq $0, $0, " + endif + "\n";
    ret += elsePointer + ":\n";
    ret += generateAssemblyCode(statements_2, offset_tabel, symbolTables);
    ret += endif + ":\n";
  } else if (expr->rule == "factor NULL") {
    ret += "add $3, $0, $11\n";
  } else if (expr->rule == "factor AMP lvalue") {
    ret += "; " + expr->rule + "\n";
    Tree *lvalue = expr->children[1];
    if (lvalue->rule == "lvalue STAR factor") {
      Tree *factor2 = lvalue->children[1];
      ret += "; " + lvalue->rule + " " + factor2->rule + "\n";
      ret += "; ====== factor 2 ======= Begin\n";
      ret += generateAssemblyCode(factor2, offset_tabel, symbolTables);
      ret += "; ====== factor 2 ======= End\n";
    } else if (lvalue->rule == "lvalue ID") {
      // store the value of expr in offset(char)($29)
      Tree *ID = lvalue->children[0];
      string a = ID->ch[0];
      ret += "lis $3\n";

      // symbolTables[a] need number shift
      int shift = offset_tabel[a];
      ret += ".word " + to_string(shift) + "\n";
      ret += "add $3, $3, $29\n";
    }
  } else if (expr->rule == "factor STAR factor") {
    Tree *factor = expr->children[1];
    ret += generateAssemblyCode(factor, offset_tabel, symbolTables);

    ret += "lw $3, 0($3)\n";
  }

  else if (expr->rule == "factor NEW INT LBRACK expr RBRACK") {

    Tree *expr2 = expr->children[3];
    ret += generateAssemblyCode(expr2, offset_tabel, symbolTables);
    ret += "add $1, $3, $0 ; new procedure expects value in $1\n";
    ret += generatePushInstruction("$31");
    ret += "lis $5\n";
    ret += ".word new\n";
    ret += "jalr $5 \n";
    ret += generatePopInstruction("$31");
    ret += "bne $3, $0, 1 ; if call succeeded skip next instruction\n";
    ret += "add $3, $11, $0 ; set $3 to NULL if allocation fails\n";
  } else if (expr->rule == "statement DELETE LBRACK RBRACK expr SEMI") {

    Tree *expr2 = expr->children[3];
    ret += generateAssemblyCode(expr2, offset_tabel, symbolTables);
    string skipDelete = "skipDelete" + to_string(skipDeleteCounter);
    skipDeleteCounter++;
    ret += "beq $3, $11, " + skipDelete + "\n";
    ret += "add $1, $3, $0 ; delete expects the address in $1\n";
    ret += generatePushInstruction("$31");
    ret += "lis $5 \n";
    ret += ".word delete\n";
    ret += "jalr $5\n";
    ret += generatePopInstruction("$31");
    ret += skipDelete + ":\n";
    ret += "\n";
  } else if (expr->rule == "factor GETCHAR LPAREN RPAREN") {
    ret += "; Calling getchar function\n";
    ret += generatePushInstruction("$29");
    ret += generatePushInstruction("$31");
    ret += "lis $5\n";
    ret += ".word getchar\n";
    ret += "jalr $5\n";
    ret += generatePopInstruction("$31");
    ret += generatePopInstruction("$29");
  } else if (expr->rule == "factor ID LPAREN RPAREN") {

    Tree *ID = expr->children[0];
    string funcName = ID->ch[0];
    ret += "; Calling anouther function: " + funcName + "\n";
    ret += generatePushInstruction("$29");
    ret += generatePushInstruction("$31");
    ret += "lis $5\n";
    ret += ".word " + funcName + "\n";
    ret += "jalr $5\n";
    ret += generatePopInstruction("$31");
    ret += generatePopInstruction("$29");
  } else if (expr->rule == "factor ID LPAREN arglist RPAREN") {

    Tree *ID = expr->children[0];
    Tree *arglist = expr->children[2];
    string funcName = ID->ch[0];
    ret += "; Calling anouther function: " + funcName + " =============== \n";
    ret += generatePushInstruction("$29");
    ret += generatePushInstruction("$31");
    ret += generatePushInstruction("$28");
    ret += "add $28, $30, $0\n";

    int n = 0;
    while (true) {
      Tree *linkedExpr = arglist->children[0];
      ret += "; ARgument Number# " + to_string(n) + "\n";
      ret += generateAssemblyCode(linkedExpr, offset_tabel, symbolTables);
      ret += generatePushInstruction("$3");
      n++;
      if (arglist->rule == "arglist expr") {
        break;
      } else if (arglist->rule == "arglist expr COMMA arglist") {
        arglist = arglist->children[2];
      }
    }

    ret += "sub $30, $28, $" + to_string(n * WORD_SIZE) + "\n";
    ret += "lis $5\n";
    ret += ".word " + funcName + "\n";
    ret += "jalr $5\n";

    ret += "add $30, $28, $0 ; this is the restore the stack\n";
    ret += generatePopInstruction("$28");
    ret += generatePopInstruction("$31");
    ret += generatePopInstruction("$29");
  }
  return ret;
}

void paramlist_gengenerateAssemblyCode(Tree * /* paramlist */,
                                       map<string, int> & /* offset_tabel */,
                                       int & /* offset */) {
  /*
      extractDeclarationInfo(first_dcl, IDValue, tempType);
      method_offset_tabel[methodName][IDValue] = offset;
      offset -= WORD_SIZE;
      cout << "sub $30 , $30, $" << WORD_SIZE << " ; update stack pointer" <<
     endl; cout << "sw $1, 0($29) ; push variable a" << endl;
      */
}

string generateDeclarationCode(Tree *dcls, map<string, int> &offset_tabel,
                               int &offset) {
  string ret = "";
  if (dcls->rule == "dcls") {
    return ret;
  } else if (dcls->rule == "dcls dcls dcl BECOMES NUM SEMI") {
    Tree *dcls_child = dcls->children[0];
    Tree *dcl = dcls->children[1];
    Tree *NUM = dcls->children[3];
    string NUM_val = NUM->ch[0], IDValue, tempType;

    extractDeclarationInfo(dcl, IDValue, tempType);
    // Debug: offset trace suppressed from direct stderr
    offset_tabel[IDValue] = offset;
    ret += "sub $30, $30, $" + to_string(WORD_SIZE) +
           " ; Allocate space for variable '" + IDValue + "' on stack\n";
    ret += "lis $3 ; Load immediate value for variable '" + IDValue + "'\n";
    ret += ".word " + NUM_val + " ; Store constant " + NUM_val + " in $3\n";
    ret += "sw $3, " + to_string(offset) + "($29) ; Store variable '" +
           IDValue + "' (type: " + tempType + ") at stack offset " +
           to_string(offset) + "\n";
    offset -= WORD_SIZE;
    ret += generateDeclarationCode(dcls_child, offset_tabel, offset);
  } else if (dcls->rule == "dcls dcls dcl BECOMES NULL SEMI") {
    Tree *dcls_child = dcls->children[0];
    Tree *dcl = dcls->children[1];
    Tree *NUM = dcls->children[3];
    string NUM_val = NUM->ch[0], IDValue, tempType;

    extractDeclarationInfo(dcl, IDValue, tempType);
    // Debug: offset trace suppressed from direct stderr
    offset_tabel[IDValue] = offset;
    ret += "sub $30, $30, $" + to_string(WORD_SIZE) +
           " ; Allocate space for variable '" + IDValue + "' on stack\n";
    ret += "sw $11, " + to_string(offset) + "($29) ; Store NULL in variable '" +
           IDValue + "' (type: " + tempType + ") at stack offset " +
           to_string(offset) + "\n";
    offset -= WORD_SIZE;
    ret += generateDeclarationCode(dcls_child, offset_tabel, offset);
  }
  return ret;
}
string generateFunctionCode(Tree *pr) {
  string ret = "";
  stringstream ssin(pr->rule);

  if (pr->rule == "start BOF procedures EOF") {
    ret += generateFunctionCode(pr->children[1]);
  }
  if (pr->rule == "procedures procedure procedures") {
    ret += generateFunctionCode(pr->children[0]);
    ret += generateFunctionCode(pr->children[1]);
  }
  if (pr->rule == "procedures main") {
    ret += generateFunctionCode(pr->children[0]);
  }
  if (pr->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls "
                  "statements RETURN expr SEMI RBRACE") {
    Tree *first_dcl = pr->children[3];
    Tree *second_dcl = pr->children[5];
    Tree *dcls = pr->children[8];
    Tree *statements = pr->children[9];
    Tree *return_expr = pr->children[11];
    string methodName = "wain", IDValue, tempType;
    int offset = 0;

    ret += "wain: ; Main function entry point\n";
    ret += "; === FUNCTION PROLOGUE ===\n";

    ret += "sub $29, $30, $" + to_string(WORD_SIZE) +
           " ; Set up frame pointer ($29) to point to first local variable\n";

    // Handle first parameter (a)
    extractDeclarationInfo(first_dcl, IDValue, tempType);
    method_offset_tabel[methodName][IDValue] = offset;
    offset -= WORD_SIZE;
    ret += "sub $30, $30, $" + to_string(WORD_SIZE) +
           " ; Allocate space for first parameter on stack\n";
    ret += "sw $1, 0($29) ; Store first parameter (a) from $1 to stack frame\n";

    // Handle second parameter (b)
    extractDeclarationInfo(second_dcl, IDValue, tempType);
    method_offset_tabel[methodName][IDValue] = offset;
    offset -= WORD_SIZE;

    ret += "sub $30, $30, $" + to_string(WORD_SIZE) +
           " ; Allocate space for second parameter on stack\n";
    ret += "sw $2, -" + to_string(WORD_SIZE) +
           "($29) ; Store second parameter (b) from $2 to stack frame\n";
    ret += "; === END PROLOGUE ===\n";

    // Calling init function for memory management
    ret += generatePushInstruction("$2"); // Save second parameter
    string ID;
    extractDeclarationInfo(first_dcl, IDValue, tempType);
    if (tempType == "int") {
      ret += "add $2, $0, $0 ; Set $2 to 0 for int parameter (no array "
             "initialization needed)\n";
    }
    ret += generatePushInstruction("$31"); // Save return address
    ret += "lis $5 ; Load address of init function\n";
    ret += ".word init ; Init function label\n";
    ret += "jalr $5 ; Call init function for memory management\n";
    ret += generatePopInstruction("$31"); // Restore return address
    ret += generatePopInstruction("$2");  // Restore second parameter
    // End calling init

    ret +=
        generateDeclarationCode(dcls, method_offset_tabel[methodName], offset);
    ret += generateAssemblyCode(statements, method_offset_tabel[methodName],
                                tables[methodName].second);
    ret += "; === FUNCTION EPILOGUE ===\n";
    ret += "; Evaluate return expression\n";
    ret += generateAssemblyCode(return_expr, method_offset_tabel[methodName],
                                tables[methodName].second);

    ret += "; Restore stack frame and return\n";
    ret += "add $30, $29, $" + to_string(WORD_SIZE) +
           " ; Restore stack pointer to original position\n";
    ret += "jr $31 ; Return to caller\n";
    ret += "; === END EPILOGUE ===\n";
  } else if (pr->rule ==
             "procedure INT ID LPAREN params RPAREN LBRACE dcls statements "
             "RETURN expr SEMI RBRACE") { //                           0   1 2
                                          //                           3      4
                                          //                           5     6
                                          //                           7 8    9
    string methodName = pr->children[1]->rule.substr(3);
    ret += "; " + pr->rule + "\n";
    ret += methodName +
           ": ; begin prologue ======== ANOUTHER FUNCTION: " + methodName +
           "\n";
    Tree *params = pr->children[3];
    Tree *dcls = pr->children[6];
    Tree *statements = pr->children[7];
    Tree *return_expr = pr->children[9];

    int offset = 0;
    ret +=
        "sub $29, $30, $" + to_string(WORD_SIZE) + " ; setup frame pointer\n";

    if (params->rule == "params paramlist") {
      Tree *paramlist = params->children[0];
      while (true) {
        offset += WORD_SIZE;
        if (paramlist->rule == "paramlist dcl") {
          break;
        } else if (paramlist->rule == "paramlist dcl COMMA paramlist") {
          paramlist = paramlist->children[2];
        }
      }
      paramlist = params->children[0];
      while (true) {
        Tree *first_dcl = paramlist->children[0];
        string IDValue, tempType;
        extractDeclarationInfo(first_dcl, IDValue, tempType);

        method_offset_tabel[methodName][IDValue] = (offset);
        offset = offset - WORD_SIZE;
        if (paramlist->rule == "paramlist dcl") {
          break;
        } else if (paramlist->rule == "paramlist dcl COMMA paramlist") {
          paramlist = paramlist->children[2];
        }
      }
    }

    ret += "; end prologue\n";
    ret +=
        generateDeclarationCode(dcls, method_offset_tabel[methodName], offset);
    ret += generateAssemblyCode(statements, method_offset_tabel[methodName],
                                tables[methodName].second);
    ret += "; begin return\n";
    ret += generateAssemblyCode(return_expr, method_offset_tabel[methodName],
                                tables[methodName].second);

    // I need to count the number of declairs REEEEEEE
    ret += "; begin epilogue\n";

    ret +=
        "add $30, $29, $" + to_string(WORD_SIZE) + " ; update stack pointer\n";
    ret += "jr $31 \n";
    ret += "; end epilogue\n";
  }
  return ret;
}

string generateCompleteAssembly(string scannedContent) {
  // First we need to make the parse tree from WLP4I file

  string curLine;
  stack<Tree *> WLP4I;
  stack<int> WLP4INumChildren;
  Tree rootWrapper{};
  Tree *root;
  bool first = true;
  stringstream ss(scannedContent);
  while (getline(ss, curLine)) {

    // cout << curLine << " " << children << " " << allCaps(firstNum) << endl;

    WLP4I.push(new Tree{curLine, {}, {}}); // add the node on top
    if (first) {
      root = WLP4I.top();
      first = false;
    }
    stringstream ssin(curLine);
    string firstNum, strIn;
    ssin >> firstNum;
    int children = 0;
    // Have child
    while (ssin >> strIn) {
      WLP4I.top()->ch.push_back(strIn);
      children++;
      // this is getting the children of the stack once it poped it goes to the
      // main stack
    }
    if (allCaps(firstNum)) {
      children = 0;
    }
    WLP4INumChildren.push(children);
    while (!WLP4INumChildren.empty() && !WLP4INumChildren.top())
    // the top is zero in the new assignemnt the zero top needs to be identified
    // immediatelly
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
  if (buildTable(root, tables)) {
    errorLog() << "[Stage: Codegen] Failure" << '\n';
    errorLog() << "  While building symbol tables from parse tree" << '\n';
    errorLog() << "  Message: build failed" << '\n';
    return "";
  }
  checkProgramTypeValidity(root, tables);

  string ret = "";
  ret += ".import print ; Import print function for output\n";
  ret += ".import init ; Import init function for memory management\n";
  ret += ".import new ; Import new function for dynamic allocation\n";
  ret += ".import delete ; Import delete function for memory deallocation\n";
  ret += ".import putchar ; Import putchar function for character output\n";
  ret += ".import getchar ; Import getchar function for character input\n";
  ret += "lis $" + to_string(WORD_SIZE) + " ; Load word size constant\n";
  ret += ".word " + to_string(WORD_SIZE) + " ; Store word size (" +  to_string(WORD_SIZE) + " bytes) in $" + to_string(WORD_SIZE) + "\n";
  ret += "lis $11 ; Load constant 1\n";
  ret += ".word 1 ; Store 1 in $11 (used for comparisons and NULL)\n";
  ret += "beq $0, $0, wain ; Jump to main function (unconditional branch)\n";

  ret += generateFunctionCode(root);
  return ret;
}

string wlp4_to_asm(string wlp4_input) {
  resetErrorLog();
  
  // Process the input string instead of reading from stdin
  string scannedContent = "";
  stringstream input_stream(wlp4_input);
  string line;
  int lineNumber = 0;
  
  try {
    while (getline(input_stream, line)) {
      g_currentInputLine = ++lineNumber;
      vector<Token> tokenLine = scan(line);
      
      for (auto &token : tokenLine) {
        scannedContent += tokenNames[token.getKind()] + " " + token.getLexeme() + "\n";
      }
    }
  } catch (ScanningFailure &f) {
    errorLog() << "[Stage: Scanning] Failure" << '\n';
    errorLog() << "  Line: " << g_currentInputLine << '\n';
    errorLog() << "  Column: " << g_scannerErrorInfo.column << '\n';
    errorLog() << "  Offending char: '" << g_scannerErrorInfo.offendingChar
               << "' (ascii " << static_cast<int>(g_scannerErrorInfo.offendingChar)
               << ")" << '\n';
    errorLog() << "  Previous DFA state: " << g_scannerErrorInfo.previousState
               << '\n';
    errorLog() << "  Munched input: \"" << g_scannerErrorInfo.munchedInput
               << "\"" << '\n';
    errorLog() << "  Message: " << f.what() << '\n';
    return "";
  }
  
  if (g_error.hasError) {
    cerr << g_error.cliOutput;
    return "";
  }

  string parseResult = performSyntaxAnalysis(scannedContent);
  
  if (g_error.hasError || parseResult == "Parse failed") {
    cerr << g_error.cliOutput;
    return "";
  }

  string assemblyCode = generateCompleteAssembly(parseResult);
  if (g_error.hasError || assemblyCode.empty()) {
    cerr << g_error.cliOutput;
    return "";
  }

  return assemblyCode;
}


int main() {
  // Read WLP4 input from stdin
  string wlp4_input = "";
  string line;
  while (getline(cin, line)) {
    wlp4_input += line + "\n";
  }
  
  // Compile WLP4 to Assembly
  string asm_output = wlp4_to_asm(wlp4_input);
  if (asm_output.empty()) {
    cerr << "ERROR: WLP4 compilation failed" << endl;
    return 1;
  }
  
  // Output the assembly
  cout << asm_output;
  
  return 0;
}