# WLP4 Compiler Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2 -I.
LDFLAGS = 

# Source files
WLP4_SRC = wlp4compile.cc

# Object files
WLP4_OBJ = wlp4compile.o

# Executable
EXEC = wlp4compiler

# Default target
all: $(EXEC)

# Main executable
$(EXEC): $(WLP4_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Object file compilation
$(WLP4_OBJ): $(WLP4_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -f $(WLP4_OBJ) $(EXEC)
	rm -f *.o *.d

# Phony targets
.PHONY: all clean
