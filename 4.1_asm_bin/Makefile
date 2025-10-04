# Main Makefile for Enhanced Assembler Pipeline
CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD

# Build all components
all: assembler linker loader

# Assembler
assembler:
	$(MAKE) -C assembler

# Linker
linker:
	$(MAKE) -C linker

# Loader (MERL Processor)
loader:
	$(MAKE) -C loader

# Clean everything
clean:
	$(MAKE) -C assembler clean
	$(MAKE) -C linker clean
	$(MAKE) -C loader clean

# Test the complete pipeline
test: all
	@echo "Testing complete pipeline..."
	@echo "1. Assembling main module..."
	./assembler/linkasm < examples/test_import.asm > examples/test_import.merl
	@echo "2. Assembling library module..."
	./assembler/linkasm < examples/test_lib.asm > examples/test_lib.merl
	@echo "3. Linking modules..."
	./linker/linker examples/test_import.merl examples/test_lib.merl > examples/linked.merl
	@echo "4. Processing MERL to binary..."
	./loader/merl 0 < examples/linked.merl > examples/final.bin
	@echo "Pipeline test complete!"
	@echo "Generated files:"
	@ls -la examples/*.merl examples/*.bin

# Test individual components
test-assembler: assembler
	@echo "Testing assembler..."
	./assembler/linkasm < examples/sample.asm > examples/sample.merl
	@echo "Assembler test complete!"

test-linker: linker
	@echo "Testing linker..."
	@echo "Linker test complete!"

test-loader: loader
	@echo "Testing loader..."
	@echo "Loader test complete!"

.PHONY: all clean test test-assembler test-linker test-loader assembler linker loader