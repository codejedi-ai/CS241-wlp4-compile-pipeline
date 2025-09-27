CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = wlp4gen
OBJECTS = wlp4gen.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}



.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
