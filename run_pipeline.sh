#!/bin/bash

# WLP4 to x86 Compiler Pipeline Orchestration Script
# Runs the complete pipeline from WLP4 source to executable

set -e

echo "🚀 WLP4 to x86 Compiler Pipeline"
echo "================================="
echo ""

# Check if sample file is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <sample_file.wlp4> [output_name]"
    echo ""
    echo "Available samples:"
    ls -la samples/
    echo ""
    echo "Example: $0 samples/01_simple_addition.wlp4 simple_add"
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_NAME="${2:-$(basename "$INPUT_FILE" .wlp4)}"

# Check if input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "❌ Error: Input file '$INPUT_FILE' not found!"
    exit 1
fi

echo "📝 Input: $INPUT_FILE"
echo "📝 Output: $OUTPUT_NAME"
echo ""

# Build all stages if not already built
echo "🔨 Building pipeline stages..."
make build > /dev/null 2>&1 || {
    echo "Building pipeline stages..."
    make build
}
echo ""

# Run the complete pipeline
echo "🔄 Running complete pipeline..."
echo ""

# Stage 1: Lexical Analysis
cd pipeline/01_lexical_analysis
./wlp4scan < "../../$INPUT_FILE" > temp.tokens

# Stage 2: Syntax Analysis  
cd ../02_syntax_analysis
../01_lexical_analysis/wlp4scan < "../../$INPUT_FILE" | ./wlp4parse > temp.ast
if [ $? -ne 0 ]; then
    echo "   ❌ Parser failed"
    exit 1
fi

# Stage 3: Semantic Analysis
cd ../03_semantic_analysis
../01_lexical_analysis/wlp4scan < "../../$INPUT_FILE" | ../02_syntax_analysis/wlp4parse | ./wlp4type > temp.semantic
if [ $? -ne 0 ]; then
    echo "   ❌ Semantic analysis failed - compilation aborted"
    exit 1
fi

# Stage 4: Code Generation
cd ../04_code_generation
../01_lexical_analysis/wlp4scan < "../../$INPUT_FILE" | ../02_syntax_analysis/wlp4parse | ../03_semantic_analysis/wlp4type | ./wlp4gen_x86 > temp.s
if [ $? -ne 0 ]; then
    echo "   ❌ Code generation failed"
    exit 1
fi

# Stage 5: Assembly
cd ../05_assembly
../01_lexical_analysis/wlp4scan < "../../$INPUT_FILE" | ../02_syntax_analysis/wlp4parse | ../03_semantic_analysis/wlp4type | ../04_code_generation/wlp4gen_x86 > temp.s
./x86_assembler temp.s temp.o

# Stage 6: Linking
cd ../06_linking
../05_assembly/x86_assembler ../05_assembly/temp.s ../05_assembly/temp.o
./x86_linker "$OUTPUT_NAME" ../05_assembly/temp.o

# Stage 7: Execution Ready
echo ""
echo "🚀 Stage 7: Execution Ready"
echo "   ✓ Pipeline completed successfully!"

# Clean up temporary files
cd ../..
rm -f pipeline/*/temp.*

echo ""
echo "✅ Pipeline completed successfully!"
echo "📁 Generated files:"
echo "  - pipeline/01_lexical_analysis/temp.tokens (tokens)"
echo "  - pipeline/02_syntax_analysis/temp.ast (AST)"
echo "  - pipeline/04_code_generation/temp.s (assembly)"
echo "  - pipeline/05_assembly/temp.o (object)"
echo "  - pipeline/06_linking/$OUTPUT_NAME (executable)"
echo ""
echo "🚀 To run: ./pipeline/06_linking/$OUTPUT_NAME"
