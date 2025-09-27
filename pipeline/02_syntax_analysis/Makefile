# Makefile for Stage 2: Syntax Analysis (Parser)
# Compiles the WLP4 parser

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = wlp4parse
SOURCE = wlp4parse.cc

# Default target
all: $(TARGET)

# Build the parser
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Test the parser with sample files
test: $(TARGET)
	@echo "Testing parser with sample files..."
	@echo "Testing simple addition:"
	@../01_lexical_analysis/wlp4scan < ../../samples/01_simple_addition.wlp4 | ./$(TARGET)
	@echo ""
	@echo "Testing multiplication:"
	@../01_lexical_analysis/wlp4scan < ../../samples/02_multiplication.wlp4 | ./$(TARGET)
	@echo ""
	@echo "Testing hello world:"
	@../01_lexical_analysis/wlp4scan < ../../samples/03_hello_world.wlp4 | ./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install to parent directory
install: $(TARGET)
	cp $(TARGET) ../

.PHONY: all test clean install
