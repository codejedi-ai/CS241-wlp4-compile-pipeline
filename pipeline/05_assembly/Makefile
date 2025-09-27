# Makefile for Stage 5: Assembly
# Compiles the x86 assembler

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = x86_assembler
SOURCE = x86_assembler.cpp

# Default target
all: $(TARGET)

# Build the assembler
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Test the assembler with sample files
test: $(TARGET)
	@echo "Testing assembler with sample files..."
	@echo "Testing simple addition:"
	@../01_lexical_analysis/wlp4scan < ../../samples/01_simple_addition.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@./$(TARGET) temp.s temp.o
	@rm -f temp.s temp.o
	@echo ""
	@echo "Testing multiplication:"
	@../01_lexical_analysis/wlp4scan < ../../samples/02_multiplication.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@./$(TARGET) temp.s temp.o
	@rm -f temp.s temp.o
	@echo ""
	@echo "Testing hello world:"
	@../01_lexical_analysis/wlp4scan < ../../samples/03_hello_world.wlp4 | ../02_syntax_analysis/wlp4parse | ../04_code_generation/wlp4gen_x86 > temp.s
	@./$(TARGET) temp.s temp.o
	@rm -f temp.s temp.o

# Clean build artifacts
clean:
	rm -f $(TARGET) temp.s temp.o

# Install to parent directory
install: $(TARGET)
	cp $(TARGET) ../

.PHONY: all test clean install
