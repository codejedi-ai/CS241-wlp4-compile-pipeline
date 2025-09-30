# Makefile for WLP4 Compiler Pipeline
# Organized with separate folders for each compilation stage

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -O2 -pthread

# Target executable
TARGET = webserver

# Source files
SOURCES = webserver.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Stage directories
STAGES_DIR = @completed
SCANNED_DIR = $(STAGES_DIR)/01_scanned
PARSED_DIR = $(STAGES_DIR)/02_parsed
MIPS_DIR = $(STAGES_DIR)/03_mips_asm

# Default target - webserver depends on compiler stages
all: $(TARGET)

# Build the webserver executable (depends on compiler stages)
$(TARGET): $(OBJECTS) compiler-stages
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build all compiler stages (workers)
compiler-stages:
	@echo "Building compiler worker stages..."
	@echo "Building Scanner worker..."
	@cd 01_scan && make
	@echo "Building Parser worker..."
	@cd 02_parse && make
	@echo "Building Code Generator worker..."
	@cd 03_codegen && make
	@echo "Building MIPS Emulator worker..."
	@cd 04_mips_emulator && make
	@echo "All compiler worker stages built successfully!"

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET)
	cd 01_scan && make clean
	cd 02_parse && make clean
	cd 03_codegen && make clean
	cd 04_mips_emulator && make clean
	rm -rf $(STAGES_DIR)/*

# Create stage directories
setup-dirs:
	mkdir -p $(SCANNED_DIR) $(PARSED_DIR) $(MIPS_DIR)

# Compile a WLP4 file through the complete pipeline
compile: setup-dirs
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make compile FILE=filename.wlp4"; \
		exit 1; \
	fi
	@echo "Compiling $(FILE) through complete pipeline..."
	@echo "Stage 1: Scanning..."
	@cat $(FILE) | ./01_scan/wlp4scan > $(SCANNED_DIR)/$(shell basename $(FILE) .wlp4).scan
	@echo "Stage 2: Parsing..."
	@cd 02_parse && cat ../$(SCANNED_DIR)/$(shell basename $(FILE) .wlp4).scan | ./wlp4parse > ../$(PARSED_DIR)/$(shell basename $(FILE) .wlp4).parse
	@echo "Stage 3: Code Generation (MIPS)..."
	@cat $(PARSED_DIR)/$(shell basename $(FILE) .wlp4).parse | ./03_codegen/wlp4gen > $(MIPS_DIR)/$(shell basename $(FILE) .wlp4).asm
	@echo "Compilation complete! Output files:"
	@echo "  Scanned: $(SCANNED_DIR)/$(shell basename $(FILE) .wlp4).scan"
	@echo "  Parsed:  $(PARSED_DIR)/$(shell basename $(FILE) .wlp4).parse"
	@echo "  MIPS:    $(MIPS_DIR)/$(shell basename $(FILE) .wlp4).asm"

# Run the API server (ensures workers are built)
run: $(TARGET)
	@echo "Starting WLP4 Compiler API Server with workers..."
	./$(TARGET)

# Run on specific port
run-port: $(TARGET)
	@read -p "Enter port number: " port; \
	./$(TARGET) $$port

# Test the API server
test: $(TARGET)
	@echo "Testing API server endpoints..."
	@echo "Health check:"
	@curl -s http://localhost:5000/health || echo "Server not running"
	@echo ""
	@echo "Available endpoints: /health, /scan, /parse, /codegen, /compile"

# Install target (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall target (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show project structure
structure:
	@echo "WLP4 Compiler Pipeline Structure:"
	@echo "├── 01_scan/          # Scanner stage"
	@echo "├── 02_parse/         # Parser stage"
	@echo "│   └── grammar/      # Grammar files"
	@echo "├── 03_codegen/       # Code generator stage"
	@echo "├── 04_mips_emulator/ # MIPS processor emulator"
	@echo "├── stages/           # Compilation stage outputs"
	@echo "│   ├── 01_scanned/   # Token streams"
	@echo "│   ├── 02_parsed/    # Parse trees"
	@echo "│   └── 03_mips_asm/  # MIPS assembly"
	@echo "├── grammar/          # Main grammar files"
	@echo "└── webserver         # API server"

# Phony targets
.PHONY: all clean run run-port test install uninstall compiler-stages setup-dirs compile structure

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the API server and all compiler worker stages (default)"
	@echo "  webserver  - Build API server (automatically builds workers)"
	@echo "  compiler-stages - Build all compiler worker stages"
	@echo "  clean      - Remove generated files"
	@echo "  setup-dirs - Create stage directories"
	@echo "  compile    - Compile a WLP4 file through complete pipeline"
	@echo "             Usage: make compile FILE=filename.wlp4"
	@echo "  run        - Run the API server with workers on port 5000"
	@echo "  run-port   - Run the API server with workers on a custom port"
	@echo "  test       - Test API server endpoints"
	@echo "  install    - Install to /usr/local/bin/"
	@echo "  uninstall  - Remove from /usr/local/bin/"
	@echo "  structure  - Show project structure"
	@echo "  help       - Show this help message"