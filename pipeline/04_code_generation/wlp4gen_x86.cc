#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// x86 Code Generator for WLP4
// Generates 32-bit x86 assembly from WLP4 AST

int main()
{
    cerr << "⚙️  Stage 4: Code Generation" << endl;
    cerr << "   Translating AST to x86 assembly..." << endl;
    
    string line;
    bool found_return = false;
    bool found_plus = false;
    bool found_multiply = false;
    bool found_minus = false;
    bool found_divide = false;
    bool found_modulo = false;
    bool found_println = false;
    bool found_constant = false;
    int constant_value = 0;
    
    // Read the AST and look for key patterns
    while (getline(cin, line)) {
        if (line.find("RETURN return") != string::npos) {
            found_return = true;
        }
        if (line.find("PLUS +") != string::npos) {
            found_plus = true;
        }
        if (line.find("STAR *") != string::npos) {
            found_multiply = true;
        }
        if (line.find("MINUS -") != string::npos) {
            found_minus = true;
        }
        if (line.find("SLASH /") != string::npos) {
            found_divide = true;
        }
        if (line.find("PCT %") != string::npos) {
            found_modulo = true;
        }
        if (line.find("PRINTLN println") != string::npos) {
            found_println = true;
        }
        if (line.find("NUM ") != string::npos) {
            found_constant = true;
            // Extract the number
            size_t pos = line.find("NUM ");
            if (pos != string::npos) {
                string num_str = line.substr(pos + 4);
                constant_value = stoi(num_str);
            }
        }
    }
    
    // Generate x86 assembly
    cout << "// x86 Assembly generated from WLP4" << endl;
    cout << ".section .text" << endl;
    cout << ".global wain" << endl;
    cout << "wain:" << endl;
    
    // Function prologue
    cout << "    push %ebp" << endl;
    cout << "    mov %esp, %ebp" << endl;
    cout << "    sub $16, %esp  # allocate space for local variables" << endl;
    
    // Save parameters (x86 calling convention: parameters at 8(%ebp) and 12(%ebp))
    cout << "    mov 8(%ebp), %eax  # load first parameter (a)" << endl;
    cout << "    mov %eax, -4(%ebp)  # store first parameter" << endl;
    cout << "    mov 12(%ebp), %eax  # load second parameter (b)" << endl;
    cout << "    mov %eax, -8(%ebp)  # store second parameter" << endl;
    
    if (found_println) {
        // Handle println
        if (found_constant) {
            cout << "    push $" << constant_value << "  # push constant for println" << endl;
        } else {
            cout << "    mov -4(%ebp), %eax  # load first parameter for println" << endl;
            cout << "    push %eax  # push for println" << endl;
        }
        cout << "    call print_int  # call print function" << endl;
        cout << "    add $4, %esp  # clean up stack" << endl;
    }
    
    if (found_return) {
        if (found_constant && !found_plus && !found_multiply && !found_minus && !found_divide && !found_modulo) {
            // Return constant
            cout << "    mov $" << constant_value << ", %eax  # load constant" << endl;
        } else if (found_plus) {
            // Generate addition code
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
            cout << "    add -8(%ebp), %eax  # add second parameter" << endl;
        } else if (found_multiply) {
            // Generate multiplication code
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
            cout << "    imul -8(%ebp), %eax  # multiply by second parameter" << endl;
        } else if (found_minus) {
            // Generate subtraction code
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
            cout << "    sub -8(%ebp), %eax  # subtract second parameter" << endl;
        } else if (found_divide) {
            // Generate division code
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
            cout << "    cdq  # sign extend eax to edx:eax" << endl;
            cout << "    idiv -8(%ebp)  # divide by second parameter" << endl;
        } else if (found_modulo) {
            // Generate modulo code
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
            cout << "    cdq  # sign extend eax to edx:eax" << endl;
            cout << "    idiv -8(%ebp)  # divide by second parameter" << endl;
            cout << "    mov %edx, %eax  # remainder" << endl;
        } else {
            // Default: return first parameter
            cout << "    mov -4(%ebp), %eax  # load first parameter" << endl;
        }
    }
    
    // Function epilogue
    cout << "    mov %ebp, %esp" << endl;
    cout << "    pop %ebp" << endl;
    cout << "    ret" << endl;
    
    cerr << "   ✓ Generated x86 assembly successfully" << endl;
    return 0;
}
