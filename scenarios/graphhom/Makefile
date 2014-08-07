# NOTE: Sourced by toplevel Makefile, and therefore "inherits" several
# global variables and targets defined by that Makefile.
#
# Edit the file Makefile, instead of Makefile.gen.*.  The latter is
# automatically generated from the former.  The following remarks will
# also only make sense if viewed from Makefile and not Makefile.gen.*.
#
# The only change in the generated file is to replace ${SCENNAME}
# wherever it occurs with the name of the actual scenario.  Thus, so
# long as the current scenario name is never mentioned explicitly
# (i.e., ${SCENNAME} is used religiously), it is possible to add a
# whole new scenario simply by copying an existing scenario directory
# at the command line, e.g.,
#
#  > cp -R scenarios/sudoku scenarios/sudoku2 
#
# This Makefile should, at a minimum, define the variable
# DEPENDENCIES-${SCENNAME} with a list of files to build.  The
# toplevel makefile depends on this list when defining the target
# solve-${SCENNAME} (among other things).  Resolving these
# dependencies should constitute running the scenario; the
# dependencies will usually be output data files.
#
# The scenario Makefile can add additional targets at its discretion,
# but ${SCENNAME} should appear in these targets to avoid namespace
# conflicts.
#
# In any rule, to ensure that all toplevel sources are compiled and
# related unit tests are run, depend on ${BIN}/test.touch.
#
# All directories are relative to the satcolors toplevel directory.
#

GRAPHNAME=Z8

DEPENDENCIES-${SCENNAME}:=${SOLUTIONS}/${SCENNAME}/${GRAPHNAME}.sltn

# The solver.  This program does the work and is the point of compiling.
${SOLUTIONS}/${SCENNAME}/solve: ${SCENARIOS}/${SCENNAME}/solve.cpp ${OBJS} ${BIN}/test.touch
	mkdir -p ${SOLUTIONS}/${SCENNAME}
	${GPP} ${SCENARIOS}/${SCENNAME}/solve.cpp ${CPPFLAGS} ${OBJS} ${LIBMINISAT} -o $@

${SOLUTIONS}/${SCENNAME}/${GRAPHNAME}.sltn: ${SOLUTIONS}/${SCENNAME}/solve ${DATA}/${GRAPHNAME}.mtx
	$^ $@

.PHONY: plot
plot-${SCENNAME}: ${PYTHONDIR}/visualize.py ${SOLUTIONS}/${SCENNAME}/${GRAPHNAME}.sltn ${DATA}/${GRAPHNAME}.palette
	${PYTHON} $^

