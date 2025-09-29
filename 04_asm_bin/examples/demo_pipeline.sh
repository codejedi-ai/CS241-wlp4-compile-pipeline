#!/bin/bash
# Complete Pipeline Demo - Shows ASM → MERL → Linked → Binary

echo "=========================================="
echo "    ENHANCED ASSEMBLER PIPELINE DEMO"
echo "=========================================="
echo

echo "📁 FILES WE'RE WORKING WITH:"
echo "1. math_lib.asm      - Library with exported functions"
echo "2. main_program.asm  - Program that imports functions"
echo

echo "🔍 STEP 1: SHOWING SOURCE ASSEMBLY FILES"
echo "=========================================="
echo
echo "📄 math_lib.asm (Library with exports):"
echo "----------------------------------------"
cat math_lib.asm
echo
echo "📄 main_program.asm (Program with imports):"
echo "-------------------------------------------"
cat main_program.asm
echo

echo "🔨 STEP 2: ASSEMBLING EACH MODULE TO MERL"
echo "=========================================="
echo
echo "Assembling math_lib.asm → math_lib.merl"
../assembler/linkasm < math_lib.asm > math_lib.merl
echo "✅ Generated: math_lib.merl ($(wc -c < math_lib.merl) bytes)"
echo

echo "Assembling main_program.asm → main_program.merl"
../assembler/linkasm < main_program.asm > main_program.merl
echo "✅ Generated: main_program.merl ($(wc -c < main_program.merl) bytes)"
echo

echo "🔍 STEP 3: SHOWING MERL FILES"
echo "=============================="
echo
echo "📄 math_lib.merl (Library MERL file):"
echo "--------------------------------------"
hexdump -C math_lib.merl
echo
echo "📄 main_program.merl (Main program MERL file):"
echo "---------------------------------------------"
hexdump -C main_program.merl
echo

echo "🔗 STEP 4: LINKING MODULES TOGETHER"
echo "===================================="
echo
echo "Linking math_lib.merl + main_program.merl → linked_program.merl"
../linker/linker math_lib.merl main_program.merl > linked_program.merl
echo "✅ Generated: linked_program.merl ($(wc -c < linked_program.merl) bytes)"
echo

echo "🔍 STEP 5: SHOWING LINKED MERL FILE"
echo "===================================="
echo
echo "📄 linked_program.merl (Combined MERL file):"
echo "-------------------------------------------"
hexdump -C linked_program.merl
echo

echo "⚙️  STEP 6: PROCESSING MERL TO BINARY"
echo "====================================="
echo
echo "Processing linked_program.merl → final_program.bin"
../loader/merl 0 < linked_program.merl > final_program.bin
echo "✅ Generated: final_program.bin ($(wc -c < final_program.bin) bytes)"
echo

echo "🔍 STEP 7: SHOWING FINAL BINARY"
echo "================================"
echo
echo "📄 final_program.bin (Final executable binary):"
echo "----------------------------------------------"
hexdump -C final_program.bin
echo

echo "📊 SUMMARY"
echo "=========="
echo "Generated files:"
ls -la *.merl *.bin
echo
echo "File sizes:"
echo "- math_lib.merl:      $(wc -c < math_lib.merl) bytes"
echo "- main_program.merl:  $(wc -c < main_program.merl) bytes"  
echo "- linked_program.merl: $(wc -c < linked_program.merl) bytes"
echo "- final_program.bin:  $(wc -c < final_program.bin) bytes"
echo
echo "🎉 PIPELINE COMPLETE!"
echo "====================="
echo "You now have:"
echo "1. Source assembly files (.asm)"
echo "2. Individual MERL files (.merl)" 
echo "3. Linked MERL file (.merl)"
echo "4. Final binary file (.bin)"
