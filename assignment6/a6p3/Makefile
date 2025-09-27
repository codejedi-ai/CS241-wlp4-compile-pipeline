CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = lr
OBJECTS = lr.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}



.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
