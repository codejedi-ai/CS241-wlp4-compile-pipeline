# Stage 5: Assembly

## Purpose
Converts x86 assembly code into object code (binary machine code).

## Files
- `x86_assembler.cpp` - Assembler implementation
- `samples/` - Assembly files and object code

## What it does
1. Reads x86 assembly source code
2. Encodes instructions to machine code
3. Resolves symbols and labels
4. Generates object file format
5. Handles relocation information

## Input/Output
- **Input**: x86 assembly source code
- **Output**: Object file (binary machine code)

## Example
**Input** (x86 assembly):
```assembly
push %ebp
mov %esp, %ebp
sub $16, %esp
mov 8(%ebp), %eax
add -8(%ebp), %eax
mov %ebp, %esp
pop %ebp
ret
```

**Output** (object code):
```
55 89 e5 83 ec 10 8b 45 08 03 45 f8 89 ec 5d c3
```

## Usage
```bash
g++ -std=c++14 -Wall -o x86_assembler x86_assembler.cpp
./x86_assembler input.s output.o
```
