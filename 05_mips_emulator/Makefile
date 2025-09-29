CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = mips_emulator
SOURCE = mips_emulator.cc

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

.PHONY: clean
