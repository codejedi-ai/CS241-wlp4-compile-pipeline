CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = wlp4scan
OBJECTS = wlp4scan.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}



.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
