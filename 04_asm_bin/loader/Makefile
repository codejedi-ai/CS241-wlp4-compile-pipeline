# Loader (MERL Processor) Makefile
CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = merl
EXEC_PROPER = merl_proper
OBJECTS = merl.o
OBJECTS_PROPER = merl_proper.o
DEPENDS = ${OBJECTS:.o=.d} ${OBJECTS_PROPER:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

${EXEC_PROPER}: ${OBJECTS_PROPER}
	${CXX} ${CXXFLAGS} ${OBJECTS_PROPER} -o ${EXEC_PROPER}

# Build both versions
all: ${EXEC} ${EXEC_PROPER}

-include ${DEPENDS}

.PHONY: clean
clean:
	rm ${OBJECTS} ${OBJECTS_PROPER} ${EXEC} ${EXEC_PROPER} ${DEPENDS}