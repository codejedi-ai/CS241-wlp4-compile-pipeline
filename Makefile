# Makefile for WLP4 Compiler Web Server
# Compiles C++ webserver and all compiler stages

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -O2 -pthread

# Target executable
TARGET = webserver

# Source files
SOURCES = webserver.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

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
	@echo "All compiler worker stages built successfully!"

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET)
	cd 01_scan && make clean
	cd 02_parse && make clean
	cd 03_codegen && make clean

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

# Phony targets
.PHONY: all clean run run-port test install uninstall compiler-stages

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the API server and all compiler worker stages (default)"
	@echo "  webserver  - Build API server (automatically builds workers)"
	@echo "  compiler-stages - Build all compiler worker stages"
	@echo "  clean      - Remove generated files"
	@echo "  run        - Run the API server with workers on port 5000"
	@echo "  run-port   - Run the API server with workers on a custom port"
	@echo "  test       - Test API server endpoints"
	@echo "  install    - Install to /usr/local/bin/"
	@echo "  uninstall  - Remove from /usr/local/bin/"
	@echo "  help       - Show this help message"
