# Assignment 10: Advanced Compiler Features

## Overview
This assignment implements advanced compiler features and optimizations that enhance the basic compiler from Assignment 8. It includes advanced code generation techniques, optimization passes, and extended language support.

## Components
- **a10p1/**: Advanced code generation
- **a10p2/**: Optimization passes
- **a10p3/**: Extended language features
- **a10p4/**: Performance improvements

## Advanced Features
- **Code Optimization**: Dead code elimination, constant folding, register allocation
- **Advanced Control Flow**: Complex loop structures, switch statements
- **Memory Optimization**: Better stack management, heap optimization
- **Error Recovery**: Enhanced error reporting and recovery
- **Performance Analysis**: Profiling and performance measurement

## Optimization Techniques
- **Register Allocation**: Advanced register allocation algorithms
- **Instruction Scheduling**: Reordering instructions for better performance
- **Constant Propagation**: Replacing variables with known constants
- **Dead Code Elimination**: Removing unreachable code
- **Loop Optimization**: Loop unrolling and strength reduction

## Extended Language Support
- **Advanced Types**: More complex data types
- **Function Overloading**: Multiple functions with same name
- **Exception Handling**: Try-catch blocks and error handling
- **Advanced Memory Management**: Garbage collection hints
- **Inline Assembly**: Direct MIPS assembly insertion

## Pipeline Position
Final enhancement phase - adds advanced features to the complete compiler.

## Usage
```bash
# Compile with optimizations
wlp4c -O2 < input.wlp4 > output.mips

# Compile with advanced features
wlp4c -advanced < input.wlp4 > output.mips

# Profile compilation
wlp4c -profile < input.wlp4 > output.mips
```

## Example
Advanced WLP4 program:
```c
int wain(int a, int b) {
    int result = 0;
    int temp = a * b;
    
    // Optimized loop
    for (int i = 0; i < a; i++) {
        result += temp;
    }
    
    // Inline assembly for performance
    asm("add $3, $3, $1");
    
    return result;
}
```

## Key Implementation Details
- **Optimization Passes**: Multiple optimization phases
- **Advanced Code Generation**: More sophisticated assembly generation
- **Performance Profiling**: Built-in performance measurement
- **Error Recovery**: Robust error handling and recovery
- **Memory Management**: Advanced memory allocation strategies

## Dependencies
- Assignment 8 (complete compiler)
- Advanced compiler theory
- Performance optimization techniques
- Understanding of MIPS architecture optimizations

