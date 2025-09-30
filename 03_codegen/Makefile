# Makefile for wlp4gen compiler
# Compiles C++ source to executable

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -O2

# Target executable
TARGET = wlp4gen

# Source files
SOURCES = wlp4gen.cc

# Object files
OBJECTS = $(SOURCES:.cc=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install target (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall target (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Phony targets
.PHONY: all clean install uninstall

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the wlp4gen compiler (default)"
	@echo "  clean    - Remove generated files"
	@echo "  install  - Install to /usr/local/bin/"
	@echo "  uninstall- Remove from /usr/local/bin/"
	@echo "  help     - Show this help message"
