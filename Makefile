# Directories and options
GPP:=g++
PYTHON=python
SRC:=src
TESTSRC:=test
BIN:=bin
DATA:=data
PYTHONDIR:=python
CPPFLAGS:=-g -std=c++0x -D__STDC_FORMAT_MACROS -MMD -MP
LIBCPPUNIT:=-lcppunit -ldl
LIBMINISAT:=-lminisat

# Needed to run from eclipse
PATH:=${PATH}:${CPPUNIT}/src/cppunit/.libs

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
${SRC}/%.o: ${SRC}/%.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${SRC}/$*.o

# Compile test files
${TESTSRC}/%.o: ${TESTSRC}/%.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${TESTSRC}/$*.o

# The test runner is a special source file not depending on headers.
${TESTSRC}/runtests.o: ${TESTSRC}/runtests.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${TESTSRC}/runtests.o

${BIN}/runtests: ${TEST_OBJS}
	${GPP} $^ -o $@ ${LIBCPPUNIT} ${LIBMINISAT}

# Run tests.  Leaves a touchfile to record when tests were run.
.PHONY: test
test: ${BIN}/test.touch

${BIN}/test.touch: ${BIN}/runtests
	./${BIN}/runtests
	touch ${BIN}/test.touch

# TODO: archive the results, clean up variables and variable
# inheritance (add exports?) and make sure this code handles files of
# arbitrary depth below scenarios/% (possibly with find?)
solve/%: ${OBJS} ${BIN}/test.touch scenarios/%/*
	mkdir -p solve
	${MAKE} -C scenarios/$(@F) solve
	touch $@

# The solver.  This program does the work and is the point of compiling.
${BIN}/solve: ${OBJS} ${BIN}/test.touch
	${GPP} ${OBJS} ${LIBMINISAT} -o $@

.PRECIOUS: ${DATA}/%.mtx ${DATA}/%.palette
${DATA}/%.mtx ${DATA}/%.palette: ${DATA}/%.graphml ${PYTHONDIR}/parsegraphml.py
	${PYTHON} ${PYTHONDIR}/parsegraphml.py ${DATA}/$*.graphml ${DATA}/$*.mtx ${DATA}/$*.palette

${DATA}/%.sltn: ${DATA}/%.mtx ${DATA}/%.palette ${BIN}/solve
	${BIN}/solve ${DATA}/$*.mtx ${DATA}/$*.sltn
	mkdir tempdir
	cp ${DATA}/$*.sltn ${DATA}/$*.palette ${DATA}/$*.mtx ${DATA}/$*.graphml ${SRC}/Solve.cpp tempdir
	mv tempdir archive/`date +'%Y-%m-%d_%H-%M-%S'`


.PHONY: plot
plot: ${PYTHONDIR}/visualize.py
	for file in ${DATA}/*.sltn; do \
		${PYTHON} ${PYTHONDIR}/visualize.py $${file} $${file%%.*}.palette; \
	done;

# Debugging for this makefile
.PHONY: makefile-debug
makefile-debug:
	echo OBJS: ${OBJS}
	echo TEST_OBJS: ${TEST_OBJS}

# Eliminate generated files and backups.
.PHONY: clean
clean:
	-rm -rf ${BIN}/* ${DATA}/*.mtx ${DATA}/*.palette ${DATA}/*.sltn *.pdf */*.o */*.d *~ */*~ *.stackdump */*.stackdump solve/

# Bring in the dependencies
-include $(TEST_SOURCES:.cpp=.d)
