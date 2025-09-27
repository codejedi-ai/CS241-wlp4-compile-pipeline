# Master Makefile for WLP4 to x86 Compiler Pipeline
# Orchestrates the complete compilation pipeline

# Default target
all: build

# Build all pipeline stages
build:
	@echo "🔨 Building WLP4 to x86 Compiler Pipeline..."
	@echo "============================================="
	@echo ""
	@echo "📝 Building Stage 1: Lexical Analysis (Scanner)..."
	@cd pipeline/01_lexical_analysis && make all
	@echo "   ✓ Scanner built successfully"
	@echo ""
	@echo "🌳 Building Stage 2: Syntax Analysis (Parser)..."
	@cd pipeline/02_syntax_analysis && make all
	@echo "   ✓ Parser built successfully"
	@echo ""
	@echo "🔍 Building Stage 3: Semantic Analysis..."
	@cd pipeline/03_semantic_analysis && make all
	@echo "   ✓ Semantic analyzer built successfully"
	@echo ""
	@echo "⚙️  Building Stage 4: Code Generation..."
	@cd pipeline/04_code_generation && make all
	@echo "   ✓ Code generator built successfully"
	@echo ""
	@echo "🔧 Building Stage 5: Assembly..."
	@cd pipeline/05_assembly && make all
	@echo "   ✓ Assembler built successfully"
	@echo ""
	@echo "🔗 Building Stage 6: Linking..."
	@cd pipeline/06_linking && make all
	@echo "   ✓ Linker built successfully"
	@echo ""
	@echo "🚀 Building Stage 7: Execution..."
	@cd pipeline/07_execution && make all
	@echo "   ✓ Execution stage ready"
	@echo ""
	@echo "✅ All pipeline stages built successfully!"

# Test all pipeline stages
test:
	@echo "Testing WLP4 to x86 Compiler Pipeline..."
	@echo "======================================="
	@cd pipeline/01_lexical_analysis && make test
	@cd pipeline/02_syntax_analysis && make test
	@cd pipeline/03_semantic_analysis && make test
	@cd pipeline/04_code_generation && make test
	@cd pipeline/05_assembly && make test
	@cd pipeline/06_linking && make test
	@cd pipeline/07_execution && make test
	@echo ""
	@echo "✅ All pipeline tests completed successfully!"

# Clean all pipeline stages
clean:
	@echo "Cleaning WLP4 to x86 Compiler Pipeline..."
	@echo "========================================="
	@cd pipeline/01_lexical_analysis && make clean
	@cd pipeline/02_syntax_analysis && make clean
	@cd pipeline/03_semantic_analysis && make clean
	@cd pipeline/04_code_generation && make clean
	@cd pipeline/05_assembly && make clean
	@cd pipeline/06_linking && make clean
	@cd pipeline/07_execution && make clean
	@echo ""
	@echo "✅ All pipeline stages cleaned!"

# Install all pipeline stages
install:
	@echo "Installing WLP4 to x86 Compiler Pipeline..."
	@echo "=========================================="
	@cd pipeline/01_lexical_analysis && make install
	@cd pipeline/02_syntax_analysis && make install
	@cd pipeline/03_semantic_analysis && make install
	@cd pipeline/04_code_generation && make install
	@cd pipeline/05_assembly && make install
	@cd pipeline/06_linking && make install
	@cd pipeline/07_execution && make install
	@echo ""
	@echo "✅ All pipeline stages installed!"

# Run complete pipeline demo
demo: build
	@echo "Running Complete Pipeline Demo..."
	@echo "================================="
	@echo "Compiling sample programs through all stages..."
	@echo ""
	@echo "1. Simple Addition:"
	@cd pipeline/01_lexical_analysis && ./wlp4scan < ../../samples/01_simple_addition.wlp4 | cd ../02_syntax_analysis && ./wlp4parse | cd ../04_code_generation && ./wlp4gen_x86 | cd ../05_assembly && ./x86_assembler /dev/stdin temp.o && cd ../06_linking && ./x86_linker simple_add ../05_assembly/temp.o
	@echo ""
	@echo "2. Multiplication:"
	@cd pipeline/01_lexical_analysis && ./wlp4scan < ../../samples/02_multiplication.wlp4 | cd ../02_syntax_analysis && ./wlp4parse | cd ../04_code_generation && ./wlp4gen_x86 | cd ../05_assembly && ./x86_assembler /dev/stdin temp.o && cd ../06_linking && ./x86_linker multiplication ../05_assembly/temp.o
	@echo ""
	@echo "3. Hello World:"
	@cd pipeline/01_lexical_analysis && ./wlp4scan < ../../samples/03_hello_world.wlp4 | cd ../02_syntax_analysis && ./wlp4parse | cd ../04_code_generation && ./wlp4gen_x86 | cd ../05_assembly && ./x86_assembler /dev/stdin temp.o && cd ../06_linking && ./x86_linker hello_world ../05_assembly/temp.o
	@echo ""
	@echo "✅ Demo completed successfully!"

# Show pipeline status
status:
	@echo "WLP4 to x86 Compiler Pipeline Status"
	@echo "===================================="
	@echo "Pipeline Stages:"
	@echo "  01_lexical_analysis  - Scanner (wlp4scan.cc)"
	@echo "  02_syntax_analysis   - Parser (wlp4parse.cc)"
	@echo "  03_semantic_analysis - Integrated"
	@echo "  04_code_generation   - Code Generator (wlp4gen_x86.cc)"
	@echo "  05_assembly          - Assembler (x86_assembler.cpp)"
	@echo "  06_linking           - Linker (x86_linker.cpp)"
	@echo "  07_execution         - System execution"
	@echo ""
	@echo "Sample Programs:"
	@ls -la samples/
	@echo ""
	@echo "Available targets:"
	@echo "  make build  - Build all stages"
	@echo "  make test   - Test all stages"
	@echo "  make clean  - Clean all stages"
	@echo "  make demo   - Run complete demo"
	@echo "  make status - Show this status"

.PHONY: all build test clean install demo status
