CC = gcc
CXX = g++
CFLAGS = -std=c99 -Wall -Wextra -O2
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = build/mips_emulator
EXAMPLE_TARGET = build/example_decoder
C_SOURCES = mips_emulator.c decoder.c
CPP_SOURCE = main.cc

$(TARGET): $(C_SOURCES) $(CPP_SOURCE) mipscpu.h decoder.h | build
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(CPP_SOURCE) $(C_SOURCES)

$(EXAMPLE_TARGET): example_decoder.c decoder.c decoder.h | build
	$(CC) $(CFLAGS) -o $(EXAMPLE_TARGET) example_decoder.c decoder.c

build:
	mkdir -p build

clean:
	rm -rf build/

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: clean run install build