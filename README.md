# WLP4 Compiler

A complete implementation of a compiler for the WLP4 programming language, consisting of four stages: Scanner, Parser, Code Generator, and MIPS to x86 Converter. The compiler translates WLP4 source code into x86 assembly code.

## Project Structure

```
wlp4-compiler/
├── 01_scan/          # Scanner stage
│   ├── wlp4scan.cc   # Scanner implementation
│   ├── Makefile      # Build configuration
│   └── README.md     # Scanner documentation
├── 02_parse/         # Parser stage
│   ├── wlp4parse.cc  # Parser implementation
│   ├── WLP4.lr1      # Grammar specification
│   ├── Makefile      # Build configuration
│   └── README.md     # Parser documentation
├── 03_codegen/       # Code generator stage
│   ├── wlp4gen.cc    # Code generator implementation
│   ├── Makefile      # Build configuration
│   └── README.md     # Code generator documentation
├── 04_mips_to_x86/   # MIPS to x86 converter stage
│   ├── mips_to_x86.cc # MIPS to x86 converter implementation
│   ├── Makefile      # Build configuration
│   └── README.md     # Converter documentation
├── webserver.cpp     # C++ web server wrapper
├── Makefile          # Build configuration for webserver
└── README.md         # This file
```

## WLP4 Language Overview

WLP4 is a C-like programming language with the following features:

- **Types**: `int` and `int*` (pointers)
- **Functions**: User-defined procedures and a special `wain` main function
- **Control Flow**: `if/else`, `while` loops
- **Memory Management**: `new` and `delete` for dynamic allocation
- **I/O**: `println` for output
- **Expressions**: Arithmetic, logical, and comparison operations

### Example WLP4 Program

```c
int wain(int a, int b) {
    int x = 0;
    int y = 0;
    int rem = 0;
    
    x = a;
    y = b;
    
    while (y != 0) {
        rem = x % y;
        x = y;
        y = rem;
    }
    
    return x;
}
```

## Compilation Pipeline

The WLP4 compiler consists of four sequential worker stages:

1. **Scanner Worker** (`01_scan/`) - Lexical analysis
   - Input: WLP4 source code (`.wlp4`)
   - Output: Token stream (`.scanned`)

2. **Parser Worker** (`02_parse/`) - Syntax analysis
   - Input: Token stream (`.scanned`)
   - Output: Parse tree (`.parsed`)

3. **Code Generator Worker** (`03_codegen/`) - Code generation
   - Input: Parse tree (`.parsed`)
   - Output: MIPS assembly (`.asm`)

4. **MIPS to x86 Converter Worker** (`04_mips_to_x86/`) - Assembly conversion
   - Input: MIPS assembly (`.asm`)
   - Output: x86 assembly (`.x86`)

## Building the Compiler

### Prerequisites

- **C++ Compiler**: g++ with C++14 support
- **Make**: Build system
- **POSIX System**: Linux/Unix (for socket programming)

### Build Instructions

```bash
# Build webserver and all compiler stages
make

# Or build individually
make webserver
make compiler-stages

# Build individual stages
make -C 01_scan
make -C 02_parse
make -C 03_codegen
make -C 04_mips_to_x86
```

### Clean Build

```bash
# Clean everything
make clean

# Or clean individually
make -C 01_scan clean
make -C 02_parse clean
make -C 03_codegen clean
make -C 04_mips_to_x86 clean
```

## Usage

### Command Line

```bash
# Complete compilation pipeline
./01_scan/wlp4scan < input.wlp4 | ./02_parse/wlp4parse | ./03_codegen/wlp4gen | ./04_mips_to_x86/mips_to_x86 > output.x86

# Individual stages
./01_scan/wlp4scan < input.wlp4 > tokens.scanned
./02_parse/wlp4parse < tokens.scanned > parse_tree.parsed
./03_codegen/wlp4gen < parse_tree.parsed > output.asm
./04_mips_to_x86/mips_to_x86 < output.asm > output.x86
```

### API Server

The compiler includes a C++ API server that provides HTTP endpoints for each worker stage:

```bash
# Build the API server and workers
make

# Start the API server
./webserver

# Or start on a custom port
./webserver 8080
```

The API server will be available at `http://localhost:5000` (or custom port) with the following endpoints:

- **GET /health** - Health check
- **POST /scan** - Scanner worker
- **POST /parse** - Parser worker
- **POST /codegen** - Code generator worker
- **POST /mips_to_x86** - MIPS to x86 converter worker
- **POST /compile** - Complete compilation pipeline

#### API Usage Examples

**Health Check:**
```bash
curl http://localhost:5000/health
```

**Scanner Worker:**
```bash
curl -X POST http://localhost:5000/scan \
  -F "code=int wain() { return 0; }"
```

**Parser Worker:**
```bash
curl -X POST http://localhost:5000/parse \
  -F "tokens=INT int\nWAIN wain\n..."
```

**Code Generator Worker:**
```bash
curl -X POST http://localhost:5000/codegen \
  -F "parse_tree=start BOF procedures EOF\n..."
```

**MIPS to x86 Converter Worker:**
```bash
curl -X POST http://localhost:5000/mips_to_x86 \
  -F "mips_asm=add \$1, \$2, \$3\nlw \$4, 4(\$5)"
```

**Complete Compilation Pipeline:**
```bash
curl -X POST http://localhost:5000/compile \
  -F "code=int wain() { return 0; }"
```

## Generated Assembly

The compiler generates MIPS assembly code that is then converted to x86 assembly:

### MIPS Assembly Features
- Uses standard MIPS instruction set
- Implements proper stack management
- Handles function calls and returns
- Supports dynamic memory allocation
- Includes runtime library imports

### x86 Assembly Features
- Uses x86 instruction set
- Maps MIPS registers to x86 registers
- Converts MIPS addressing modes to x86 equivalents
- Maintains semantic equivalence

### Example Generated Assembly

**MIPS Assembly (intermediate):**
```assembly
.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
beq $0, $0, wain
wain: ; begin prologue
sub $29, $30, $4 ; setup frame pointer
; ... function body ...
; begin epilogue
add $30 , $29, $4 ; update stack pointer
jr $31 
; end epilogue
```

**x86 Assembly (final):**
```assembly
.import print
.import init
.import new
.import delete
mov ecx, 4
mov edx, 1
cmp eax, eax
je wain
wain: ; begin prologue
mov esp, ebp
sub esp, ecx ; setup frame pointer
; ... function body ...
; begin epilogue
add esp, ecx ; update stack pointer
jmp eax
; end epilogue
```

## Testing

Each stage includes test files:

- **Scanner**: `gcd.wlp4` → `gcd.wlp4_scanned`
- **Parser**: `gcd.wlp4_scanned` → `gcd.wlp4_parsed`
- **Code Generator**: `gcd.wlp4_parsed` → `gcd.asm`
- **MIPS to x86 Converter**: `gcd.asm` → `gcd.x86`

## Error Handling

The compiler provides detailed error messages for:

- **Lexical Errors**: Invalid characters or malformed tokens
- **Syntax Errors**: Invalid grammar constructions
- **Semantic Errors**: Type mismatches, undeclared variables
- **Runtime Errors**: Division by zero, null pointer access

## Development

### Code Structure

- **Scanner**: DFA-based tokenizer with maximal munch algorithm
- **Parser**: LR(1) bottom-up parser with parse tree construction
- **Code Generator**: Recursive tree traversal with type checking
- **MIPS to x86 Converter**: Instruction-by-instruction translation with register mapping

### Adding Features

To extend the compiler:

1. **Scanner**: Add new token types to the DFA
2. **Parser**: Update grammar in `WLP4.lr1`
3. **Code Generator**: Add code generation rules for new constructs
4. **MIPS to x86 Converter**: Add new instruction mappings and register allocation strategies

## License

This project is part of a compiler construction course and is intended for educational purposes.

## Contributing

This is an educational project. For questions or issues, please refer to the course materials or instructor.
