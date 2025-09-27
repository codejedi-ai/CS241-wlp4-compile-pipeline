CXX = g++
CXXFLAGS = -g -std=c++14 -Wall
EXEC = main-p2
OBJECTS = wrapper-p2.o a10p2.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}



.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
