# Loader (MERL Processor) Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = main
MERL_LIB = merl.o
MAIN_OBJECTS = main.o
DEPENDS = ${MERL_LIB:.o=.d} ${MAIN_OBJECTS:.o=.d}

${EXEC}: ${MAIN_OBJECTS} ${MERL_LIB}
	${CXX} ${CXXFLAGS} ${MAIN_OBJECTS} ${MERL_LIB} -o ${EXEC}

merl.o: merl.cc merl.h
	${CXX} ${CXXFLAGS} -c merl.cc -o merl.o

main.o: main.cc merl.h
	${CXX} ${CXXFLAGS} -c main.cc -o main.o

# Build the main executable
all: ${EXEC}

-include ${DEPENDS}

.PHONY: clean
clean:
	rm ${MERL_LIB} ${MAIN_OBJECTS} ${EXEC} ${DEPENDS}