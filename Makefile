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
TEST_SOURCES:=${SOURCES} $(wildcard ${TESTSRC}/*test.cpp) ${TESTSRC}/runtests.cpp

# Corresponding object lists
OBJS:=$(SOURCES:.cpp=.o)
TEST_OBJS:=$(TEST_SOURCES:.cpp=.o)

# Default target
.PHONY: build
build: test 

# Include the targets from the Makefiles for the scenarios.  To help
# the writability of each Makefile, replace the ${SCENNAME} variable
# with the actual name of the scenario before including.

define SCENARIO_INCLUDE_TEMPLATE
# Include a version of the scenario-specific makefile, but with
# ${SCENNAME} replaced with the actual name of the scenario.  This
# improves writability.
include ${SCENARIOS}/$(1)/Makefile.gen.$(1)
${SCENARIOS}/$(1)/Makefile.gen.$(1): ${SCENARIOS}/$(1)/Makefile
	cat $$< | sed 's/$$$${SCENNAME}/$(1)/g' > $$@ || rm $$@

.PHONY: solve-$(1)
solve-$(1): $${DEPENDENCIES-$(1)} archive/archive-up-to-date-$(1).touch

archive/archive-up-to-date-$(1).touch: $${DEPENDENCIES-$(1)}
	mkdir archive/tempdir
	cp -R ${SCENARIOS}/$(1) archive/tempdir/scenario
	cp -R ${SOLUTIONS}/$(1) archive/tempdir/solution
	cp -R ${DATA} archive/tempdir/data
	chmod -R ago-w archive/tempdir
	mv archive/tempdir archive/`date '+%Y-%m-%d_%H-%M-%S'`_$(1)
	touch $$@

endef ### SCENARIO_INCLUDE_TEMPLATE

SCENARIO_LIST:=$(notdir $(wildcard ${SCENARIOS}/*))
$(foreach scen, ${SCENARIO_LIST}, $(eval $(call SCENARIO_INCLUDE_TEMPLATE,$(scen))))

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

# Generate incidence matrices (and palettes) from graphs in the data folder.
.PRECIOUS: ${DATA}/%.mtx ${DATA}/%.palette
${DATA}/%.mtx ${DATA}/%.palette: ${DATA}/%.graphml ${PYTHONDIR}/parsegraphml.py
	${PYTHON} ${PYTHONDIR}/parsegraphml.py ${DATA}/$*.graphml ${DATA}/$*.mtx ${DATA}/$*.palette

.PHONY: makefile-debug
makefile-debug:
	true

# Eliminate generated files and backups.
.PHONY: clean
clean:
	-rm -rf \
	archive/*.touch \
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
