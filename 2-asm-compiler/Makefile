# Assembler Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2 -I.
LDFLAGS = 

# Source files
ASM_SRC = asm.cc
SCANNER_SRC = scanner.cc

# Object files
ASM_OBJ = asm.o
SCANNER_OBJ = scanner.o

# Executable
EXEC = binasm

# Default target
all: $(EXEC)

# Main executable
$(EXEC): $(ASM_OBJ) $(SCANNER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Object file compilation
$(SCANNER_OBJ): $(SCANNER_SRC) scanner.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ASM_OBJ): $(ASM_SRC) scanner.h $(SCANNER_OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -f $(ASM_OBJ) $(SCANNER_OBJ) $(EXEC)
	rm -f *.o *.d

# Phony targets
.PHONY: all clean
