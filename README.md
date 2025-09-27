# WLP4 to x86 Compiler Pipeline

A complete, modular compiler pipeline that translates WLP4 source code to x86 assembly and executable binaries.

## 🏗️ Pipeline Architecture

The compiler is organized into 7 numbered stages, each with its own Makefile:

```
pipeline/
├── 01_lexical_analysis/    # Scanner (wlp4scan.cc)
├── 02_syntax_analysis/     # Parser (wlp4parse_simple.cc)
├── 03_semantic_analysis/   # Integrated (no separate compilation)
├── 04_code_generation/     # Code Generator (wlp4gen_x86.cc)
├── 05_assembly/           # Assembler (x86_assembler.cpp)
├── 06_linking/            # Linker (x86_linker.cpp)
└── 07_execution/          # System execution
```

## 📁 Project Structure

```
sep272025-cs241-pipeline/
├── pipeline/               # Main pipeline stages
├── samples/               # WLP4 sample programs
├── grammar/               # Grammar files
├── Makefile              # Master build system
└── run_pipeline.sh       # Orchestration script
```

## 🚀 Quick Start

### Build All Stages
```bash
make build
```

### Test All Stages
```bash
make test
```

### Run Complete Pipeline
```bash
./run_pipeline.sh samples/01_simple_addition.wlp4 simple_add
```

### Clean Everything
```bash
make clean
```

## 📋 Available Commands

| Command | Description |
|---------|-------------|
| `make build` | Build all pipeline stages |
| `make test` | Test all pipeline stages |
| `make clean` | Clean all build artifacts |
| `make demo` | Run complete pipeline demo |
| `make status` | Show pipeline status |

## 🔧 Individual Stage Commands

Each stage can be built and tested independently:

```bash
# Build specific stage
cd pipeline/01_lexical_analysis && make build

# Test specific stage
cd pipeline/01_lexical_analysis && make test

# Clean specific stage
cd pipeline/01_lexical_analysis && make clean
```

## 📝 Sample Programs

The `samples/` directory contains WLP4 test programs:

- `01_simple_addition.wlp4` - Basic addition operation
- `02_multiplication.wlp4` - Basic multiplication operation  
- `03_hello_world.wlp4` - Hello world with println
- `04_constant_return.wlp4` - Constant return value
- `05_subtraction.wlp4` - Basic subtraction operation
- `06_division.wlp4` - Basic division operation
- `07_modulo.wlp4` - Basic modulo operation

## 🎯 Pipeline Flow

1. **Lexical Analysis**: WLP4 source → Tokens
2. **Syntax Analysis**: Tokens → Abstract Syntax Tree (AST)
3. **Semantic Analysis**: AST validation (integrated)
4. **Code Generation**: AST → x86 Assembly
5. **Assembly**: x86 Assembly → Object code
6. **Linking**: Object code → Executable
7. **Execution**: System execution

## ✅ Features

- ✅ Modular design with separate Makefiles
- ✅ Clean separation of concerns
- ✅ Comprehensive testing at each stage
- ✅ Orchestration script for complete pipeline
- ✅ Sample programs for testing
- ✅ x86 target architecture
- ✅ ELF executable generation

## 🛠️ Requirements

- C++14 compiler (g++)
- Linux environment
- Make build system

## 📊 Current Status

All pipeline stages are functional and tested:
- ✅ Scanner: Tokenizes WLP4 source code
- ✅ Parser: Generates AST from tokens
- ✅ Code Generator: Produces x86 assembly
- ✅ Assembler: Converts assembly to object code
- ✅ Linker: Creates ELF executables
- ✅ Orchestration: Complete pipeline automation
