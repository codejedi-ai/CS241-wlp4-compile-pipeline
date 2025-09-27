# Stage 6: Linking

## Purpose
Combines object files and resolves external references to create an executable.

## Files
- `x86_linker.cpp` - Linker implementation
- `samples/` - Object files and linked executables

## What it does
1. Reads object files
2. Resolves external symbols
3. Performs relocation
4. Creates executable file format
5. Handles memory layout

## Input/Output
- **Input**: Object files (.o)
- **Output**: Executable binary

## Example
**Input**: Object files with unresolved symbols
**Output**: Executable with all symbols resolved

## Usage
```bash
g++ -std=c++14 -Wall -o x86_linker x86_linker.cpp
./x86_linker executable output.o
```
