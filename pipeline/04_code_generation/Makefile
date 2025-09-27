# Makefile for Stage 4: Code Generation
# Compiles the WLP4 to x86 code generator

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = wlp4gen_x86
SOURCE = wlp4gen_x86.cc

# Default target
all: $(TARGET)

# Build the code generator
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Test the code generator with sample files
test: $(TARGET)
	@echo "Testing code generator with sample files..."
	@echo "Testing simple addition:"
	@../01_lexical_analysis/wlp4scan < ../../samples/01_simple_addition.wlp4 | ../02_syntax_analysis/wlp4parse | ./$(TARGET)
	@echo ""
	@echo "Testing multiplication:"
	@../01_lexical_analysis/wlp4scan < ../../samples/02_multiplication.wlp4 | ../02_syntax_analysis/wlp4parse | ./$(TARGET)
	@echo ""
	@echo "Testing hello world:"
	@../01_lexical_analysis/wlp4scan < ../../samples/03_hello_world.wlp4 | ../02_syntax_analysis/wlp4parse | ./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install to parent directory
install: $(TARGET)
	cp $(TARGET) ../

.PHONY: all test clean install
