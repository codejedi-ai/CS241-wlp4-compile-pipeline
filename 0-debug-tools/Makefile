# Unified Makefile for WLP4 Compile Pipeline
# CS241 Assignment - Complete WLP4 to Binary Compilation

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2 -I.
LDFLAGS = 

# Source files
MAIN_SRC = main.cc

# Object files
MAIN_OBJ = main.o

# Executable
EXEC = wlp4compiler

# Subdirectories
WLP4_DIR = wlp4-compiler
ASM_DIR = asm-compiler

# Default target
all: $(EXEC)

# Main executable
$(EXEC): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJ) $(LDFLAGS)

# Object file compilation
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@



# Clean targets
clean:
	rm -f $(MAIN_OBJ) $(EXEC)
	rm -f *.o *.d
	rm -f output.merl output.bin test_output.bin

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(EXEC)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(EXEC)

# Test target - compile a sample WLP4 file
test: $(EXEC)
	@echo "Testing with sample input..."
	@echo "int wain(int a, int b) { return a + b; }" | ./$(EXEC)
	@echo "Test completed."

# Compile WLP4 to binary target
binary: $(EXEC) build-subdirs
	@echo "Compiling WLP4 to binary..."
	@echo "int wain(int a, int b) { return a + b; }" | ./$(EXEC) > output.bin
	@echo "Binary output written to output.bin"
	@ls -la output.bin

# Compile specific WLP4 file to binary
# Usage: make compile FILE=input.wlp4 OUTPUT=output.bin
compile: $(EXEC) build-subdirs
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make compile FILE=input.wlp4 OUTPUT=output.bin"; \
		exit 1; \
	fi
	@echo "Compiling $(FILE) to binary..."
	@cat $(FILE) | ./$(EXEC) > $(or $(OUTPUT),output.bin)
	@echo "Binary output written to $(or $(OUTPUT),output.bin)"
	@ls -la $(or $(OUTPUT),output.bin)

# Build subdirectories
build-subdirs:
	$(MAKE) -C $(WLP4_DIR)
	$(MAKE) -C $(ASM_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build complete WLP4 compiler (default)"
	@echo "  $(EXEC)     - Build WLP4 compiler executable"
	@echo "  clean      - Remove all object files and executables"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized version"
	@echo "  test       - Test the compiler with sample input"
	@echo "  binary     - Compile sample WLP4 to binary (output.bin)"
	@echo "  compile    - Compile specific WLP4 file to binary"
	@echo "              Usage: make compile FILE=input.wlp4 OUTPUT=output.bin"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all clean debug release test binary compile help build-subdirs

# Dependencies
$(MAIN_OBJ): $(MAIN_SRC)
