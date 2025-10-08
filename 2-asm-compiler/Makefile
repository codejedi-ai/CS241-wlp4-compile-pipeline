# Assembler Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2 -I.
LDFLAGS = 

# Directories
BUILD_DIR = build

# Source files
ASM_SRC = asm.cc
SCANNER_SRC = scanner.cc
DECODER_SRC = decoder.c

# Object files (in build directory)
ASM_OBJ = $(BUILD_DIR)/asm.o
SCANNER_OBJ = $(BUILD_DIR)/scanner.o
DECODER_OBJ = $(BUILD_DIR)/decoder.o

# Executable (in build directory)
EXEC = $(BUILD_DIR)/binasm

# Default target
all: $(EXEC)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Main executable
$(EXEC): $(BUILD_DIR) $(ASM_OBJ) $(SCANNER_OBJ) $(DECODER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(ASM_OBJ) $(SCANNER_OBJ) $(DECODER_OBJ) $(LDFLAGS)

# Object file compilation
$(SCANNER_OBJ): $(SCANNER_SRC) scanner.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DECODER_OBJ): $(DECODER_SRC) decoder.h mipscpu.h | $(BUILD_DIR)
	gcc -Wall -Wextra -O2 -I. -c $< -o $@

$(ASM_OBJ): $(ASM_SRC) scanner.h decoder.h $(SCANNER_OBJ) $(DECODER_OBJ) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.o *.d

# Phony targets
.PHONY: all clean
