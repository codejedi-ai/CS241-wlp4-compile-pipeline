CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Iinclude
EXEC = /home/darcy/binmerl
SRCDIR = src
BUILDDIR = build
OBJECTS = $(BUILDDIR)/scanner.o $(BUILDDIR)/main.o $(BUILDDIR)/assembler.o $(BUILDDIR)/symbol_table.o $(BUILDDIR)/merl_entries.o $(BUILDDIR)/code_generator.o $(BUILDDIR)/instructions.o $(BUILDDIR)/mips_instructions.o $(BUILDDIR)/merl_file.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(BUILDDIR)
	${CXX} ${CXXFLAGS} -c $< -o $@

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -rf ${BUILDDIR} ${EXEC}
