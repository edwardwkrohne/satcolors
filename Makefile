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
SCENARIOS:=scenarios
SOLUTIONS:=solutions

# Sources and sources involving tests.
SOURCES:=$(wildcard ${SRC}/*.cpp)
TEST_SOURCES:=$(filter-out ${SRC}/Solve.cpp,${SOURCES}) $(wildcard ${TESTSRC}/*test.cpp) ${TESTSRC}/runtests.cpp

# Corresponding object lists
OBJS:=$(SOURCES:.cpp=.o)
TEST_OBJS:=$(TEST_SOURCES:.cpp=.o)

# Include the targets from the Makefiles for the scenarios.  Take care
# to define a special variable telling each Makefile what it needs to
# do, as well as some boilerplate.
define SCENARIO_INCLUDE_TEMPLATE
.PHONY: solve-$(1)

# Include a version of the scenario-specific makefile, but with
# ${SCENNAME} replaced with the actual name of the scenario.  This
# improves writability.
include ${SCENARIOS}/$(1)/Makefile.gen.$(1)
${SCENARIOS}/$(1)/Makefile.gen.$(1): ${SCENARIOS}/$(1)/Makefile
	cat $$< | sed 's/$$$${SCENNAME}/$(1)/g' > $$@ || rm $$@
endef
### SCENARIO_INCLUDE_TEMPLATE

SCENARIO_LIST:=$(notdir $(wildcard ${SCENARIOS}/*))
$(foreach scen, ${SCENARIO_LIST}, $(eval $(call SCENARIO_INCLUDE_TEMPLATE,$(scen))))

# Default target
all: ${BIN}/solve
.PHONY: build
build: all

.PHONY: all-scenarios
all-scenarios: $(addprefix solve-,${SCENARIO_LIST})

# Compile regular source files
${SRC}/%.o: ${SRC}/%.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${SRC}/$*.o

# Compile test files
${TESTSRC}/%.o: ${TESTSRC}/%.cpp
	${GPP} $< -c ${CPPFLAGS} -o ${TESTSRC}/$*.o

# The test runner is a special source file not depending on headers.
# TODO: stop treating this specially?
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


.PHONY: makefile-debug
makefile-debug:
	true

# Eliminate generated files and backups.
.PHONY: clean
clean:
	-rm -rf \
	${BIN}/* \
	${DATA}/*.mtx \
	${DATA}/*.palette \
	${DATA}/*.sltn \
	*.pdf \
	*/*.o \
	*/*.d \
	*~ \
	*/*~ \
	*/*/*~ \
	*.stackdump \
	*/*.stackdump \
	${SCENARIOS}/*/Makefile.gen.* \
	${SOLUTIONS}

# Bring in the dependencies
-include $(TEST_SOURCES:.cpp=.d)
