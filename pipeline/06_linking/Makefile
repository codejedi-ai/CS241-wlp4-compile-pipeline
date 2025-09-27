# Makefile for Stage 6: Linking
# Compiles the x86 linker

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = x86_linker
SOURCE = x86_linker.cpp

# Default target
all: $(TARGET)

# Build the linker
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Test the linker with sample files
test: $(TARGET)
	@echo "Testing linker with sample files..."
	@echo "Testing simple addition:"
	@../01_lexical_analysis/wlp4scan < ../../samples/01_simple_addition.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@../05_assembly/x86_assembler temp.s temp.o
	@./$(TARGET) simple_add temp.o
	@rm -f temp.s temp.o simple_add
	@echo ""
	@echo "Testing multiplication:"
	@../01_lexical_analysis/wlp4scan < ../../samples/02_multiplication.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@../05_assembly/x86_assembler temp.s temp.o
	@./$(TARGET) multiplication temp.o
	@rm -f temp.s temp.o multiplication
	@echo ""
	@echo "Testing hello world:"
	@../01_lexical_analysis/wlp4scan < ../../samples/03_hello_world.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@../05_assembly/x86_assembler temp.s temp.o
	@./$(TARGET) hello_world temp.o
	@rm -f temp.s temp.o hello_world

# Clean build artifacts
clean:
	rm -f $(TARGET) temp.s temp.o simple_add multiplication hello_world

# Install to parent directory
install: $(TARGET)
	cp $(TARGET) ../

.PHONY: all test clean install
