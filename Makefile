# Directories and options
GPP:=g++
SRC:=src
TESTSRC:=test
BIN:=bin
CPPFLAGS:=-g -std=c++11 -D__STDC_FORMAT_MACROS
LIBCPPUNIT:=-lcppunit -ldl
LIBMINISAT:=-lminisat

# Needed to run from eclipse
PATH:=${PATH}:${CPPUNIT}/src/cppunit/.libs

# Header files and headers used by tests
HEADERS:=$(wildcard ${SRC}/*.h)

# Sources and sources involving tests.
SOURCES:=$(wildcard ${SRC}/*.cpp)
TEST_SOURCES:=$(filter-out ${SRC}/Solve.cpp,${SOURCES}) $(wildcard ${TESTSRC}/*test.cpp) ${TESTSRC}/runtests.cpp

# Corresponding object lists
OBJS:=$(SOURCES:.cpp=.o)
TEST_OBJS:=$(TEST_SOURCES:.cpp=.o)

# Default target
all: ${BIN}/solve
.PHONY: build
build: all

# Compile regular source files (crudely assume all sources depend on all headers)
${SRC}/%.o: ${SRC}/%.cpp ${HEADERS}
	${GPP} $< -c ${CPPFLAGS} -o ${SRC}/$*.o

# Compile test files
${TESTSRC}/%.o: ${TESTSRC}/%.cpp ${HEADERS}
	${GPP} $< -c ${CPPFLAGS} -o ${TESTSRC}/$*.o

# The test runner is a special source file not depending on headers.
${TESTSRC}/runtests.o: ${TESTSRC}/runtests.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${TESTSRC}/runtests.o

${BIN}/runtests: ${TEST_OBJS}
	${GPP} $^ -o $@ ${LIBCPPUNIT} ${LIBMINISAT}

# The solver.  This program does the work and is the point of compiling.
${BIN}/solve: ${OBJS} ${BIN}/test.touch
	${GPP} ${OBJS} ${LIBMINISAT} -o $@

# Gnuplot file describing the solution
.PRECIOUS: ${BIN}/plot.gnu
${BIN}/plot.gnu: ${BIN}/solve input.dat
	./${BIN}/solve

out.pdf: ${BIN}/plot.gnu
	gnuplot ${BIN}/plot.gnu

# Run tests.  Leaves a touchfile to record when tests were run.
.PHONY: test
test: ${BIN}/test.touch

${BIN}/test.touch: ${BIN}/runtests
	./${BIN}/runtests
	touch ${BIN}/test.touch

# Debugging for this makefile
.PHONY: makefile-debug
makefile-debug:
	echo OBJS: ${OBJS}
	echo TEST_OBJS: ${TEST_OBJS}

# Run the solver
.PHONY: solve
solve: ${BIN}/solve
	./${BIN}/solve

# Run the solver and plot
.PHONY: plot
plot: out.pdf

# Eliminate generated files and backups.
.PHONY: clean
clean:
	-rm -rf ${BIN}/* *.pdf */*.o *~ */*~ *.stackdump */*.stackdump
