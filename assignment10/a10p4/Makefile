CXX = g++
CXXFLAGS = -g -std=c++14 -Wall
EXEC = main-p4
OBJECTS = malloc.o a10p1.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}



.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
