#!/bin/bash

# Test runner for assembler
# Usage: ./run_tests.sh

echo "=== Assembler Test Suite ==="
echo

# Build the project
echo "Building project..."
make clean
make
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi
echo "Build successful!"
echo

# Test 1: Binary format
echo "Test 1: Binary format"
echo "Input: tests/test1_binary.asm"
./build/binasm < tests/test1_binary.asm > tests/test1_output.bin 2> tests/test1_analysis.txt
echo "Output: tests/test1_output.bin"
echo "Analysis: tests/test1_analysis.txt"
echo

# Test 2: MERL format
echo "Test 2: MERL format"
echo "Input: tests/test2_merl.asm"
./build/binasm < tests/test2_merl.asm > tests/test2_output.merl 2> tests/test2_analysis.txt
echo "Output: tests/test2_output.merl"
echo "Analysis: tests/test2_analysis.txt"
echo

# Test 3: Complex MERL
echo "Test 3: Complex MERL with branches"
echo "Input: tests/test3_complex.asm"
./build/binasm < tests/test3_complex.asm > tests/test3_output.merl 2> tests/test3_analysis.txt
echo "Output: tests/test3_output.merl"
echo "Analysis: tests/test3_analysis.txt"
echo

# Test 4: Multiple imports/exports
echo "Test 4: Multiple imports/exports"
echo "Input: tests/test4_multiple.asm"
./build/binasm < tests/test4_multiple.asm > tests/test4_output.merl 2> tests/test4_analysis.txt
echo "Output: tests/test4_output.merl"
echo "Analysis: tests/test4_analysis.txt"
echo

echo "=== All tests completed ==="
echo "Check the tests/ directory for outputs and analysis files"
