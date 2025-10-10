#!/bin/bash
# Build script demonstrating the linking workflow

echo "=== Enhanced Assembler Pipeline Demo ==="
echo

echo "1. Assembling main program..."
../assembler/linkasm < main.asm > main.merl
echo "   Generated: main.merl"

echo
echo "2. Assembling library..."
../assembler/linkasm < library.asm > library.merl
echo "   Generated: library.merl"

echo
echo "3. Linking modules..."
../linker/linker main.merl library.merl > linked.merl
echo "   Generated: linked.merl"

echo
echo "4. Processing MERL to binary..."
../loader/merl 0 < linked.merl > final.bin
echo "   Generated: final.bin"

echo
echo "=== Build Complete ==="
echo "Generated files:"
ls -la *.merl *.bin

echo
echo "=== MERL File Structure ==="
echo "Main MERL file:"
hexdump -C main.merl | head -3

echo
echo "Library MERL file:"
hexdump -C library.merl | head -3

echo
echo "Linked MERL file:"
hexdump -C linked.merl | head -3

echo
echo "Final binary:"
hexdump -C final.bin | head -3
