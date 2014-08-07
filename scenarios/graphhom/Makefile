# NOTE: Sourced by toplevel Makefile, and therefore inherits several
# global variables defined by that Makefile
#
# Edit the file Makefile, instead of Makefile.gen.*.  The latter is
# automatically generated from the former.  The following remarks will
# also only make sense if viewed from Makefile and not Makefile.gen.*.
#
# The only change in the generated file is to replace ${SCENNAME}
# wherever it occurs with the name of the actual scenario.  Thus, so
# long as the current scenario name is never mentioned explicitly
# (${SCENNAME} is used religiously), it is possible to add a whole new
# scenario simply by copying an existing scenario directory at the
# command line, e.g.,
#
#  > cp -R scenarios/sudoku scenarios/sudoku2 
#
# This Makefile should define a target solve-${SCENNAME}, already
# declared phony elsewhere, which does whatever is needed to to define
# the scenario.  To ensure that all toplevel sources are compiled and
# related unit tests are run, depend on ${BIN}/test.touch.
#
# All directories are relative to the satcolors toplevel directory.

solve-${SCENNAME}: ${SOLUTIONS}/${SCENNAME}/touchfile

${SOLUTIONS}/${SCENNAME}/touchfile: ${BIN}/test.touch
	mkdir -p ${SOLUTIONS}/${SCENNAME}
	touch ${SOLUTIONS}/${SCENNAME}/touchfile
	$(info Inside of ${SCENNAME}/Makefile)
