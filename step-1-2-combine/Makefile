# Simple Makefile for CS241 wlp4 compile pipeline
# Builds C and C++ sources into the `wlp4compile` executable

CC = gcc
CXX = g++
AR = ar

CFLAGS = -std=c11 -O2 -Wall -Wextra -g
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -g
LDFLAGS =

SRCS_C = decoder.c
SRCS_CC = asm.cc scanner.cc wlp4compile.cc main.cc
HDRS = asm.h decoder.h scanner.h wlp4compile.h

OBJS = $(SRCS_C:.c=.o) $(SRCS_CC:.cc=.o)

TARGET = wlp4compile

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Convenience: show variables
vars:
	@echo CC=$(CC)
	@echo CXX=$(CXX)
	@echo CFLAGS=$(CFLAGS)
	@echo CXXFLAGS=$(CXXFLAGS)
