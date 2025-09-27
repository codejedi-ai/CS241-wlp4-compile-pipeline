# Makefile for Stage 1: Lexical Analysis (Scanner)
# Compiles the WLP4 scanner

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = wlp4scan
SOURCE = wlp4scan.cc

# Default target
all: $(TARGET)

# Build the scanner
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Test the scanner with sample files
test: $(TARGET)
	@echo "Testing scanner with sample files..."
	@echo "Testing simple addition:"
	@./$(TARGET) < ../../samples/01_simple_addition.wlp4
	@echo ""
	@echo "Testing multiplication:"
	@./$(TARGET) < ../../samples/02_multiplication.wlp4
	@echo ""
	@echo "Testing hello world:"
	@./$(TARGET) < ../../samples/03_hello_world.wlp4

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install to parent directory
install: $(TARGET)
	cp $(TARGET) ../

.PHONY: all test clean install
