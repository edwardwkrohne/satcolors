satcolors
=========

Easy to use high-level objects for SAT solving, including logical expressions, scalars, and matrices.

Installation and Getting Started
=================================
satcolors requires minisat v2.2, and cppunit v1.13.1; these can be obtained on Ubuntu, as of the time of this writing, via

> sudo apt-get install minisat

> sudo apt-get install libcppunit-dev

satcolors also requires a C++0x compatible compiler. g++ 4.8 is recommended; others may work, but as of the time of this writing, C++0x support is a work in progress for most compilers.

IMPORTANT: C++0x introduces new syntax that breaks files included from minisat.  Running

> sudo patch-minisat.sh

will patch minisat to correct this problem assuming its includes can be found at /usr/include/minisat.

After obtaining the code via git (github.com/edwardwkrohne/satcolors), you can compile all code but the examples, and run unit tests with

> make -j

Compiling and running the two example scenarios can be done with

> make -j solve-sudoku solve-tetrominoes

This command is cumulative with simply "make -j".  There are many more targets in the makefile; care has been taken that, with the exception of "clean" all targets can appear on the command line of the same run of make without incurring redundant operations.

Implementing New Scenarios
==============================
Two example scenarios, "sudoku" and "tetrominoes" have been provided; they live in their respective subdirectories of the "scenarios" directory.  Running "make solve-sodoku" will solve the sodoku problem given in data/sudoku; zeroes are considered blank spaces and numbers are filled in spaces.  It is the simpler of the two examples, and demonstrates setting up the solver, instantiating matrices, establishing constraints, running, and retrieving a solution.  It should run instantaneously.

Running "make solve-tetrominoes" will attempt to solve the problem of arranging two copies each of the five free tetrominoes into a 5x8 grid.  This scenario demonstrates all the features above, as well as matrix views, addressing matrix elements with Cardinal objects (to place requirements on the indices), and using incremental solving to efficiently obtain multiple solutions to the same problem.  It finds the first five solutions to the problem and runs in about five minutes on the author's (Ed Krohne's) home PC.

To make your own scenario, you can simply copy one of the existing scenarios.  That is,

> cp scenarios/sudoku scenarios/my-sat-hello-world

will create a new scenario called my-sat-hello-world.  Then, running

> make solve-my-sat-hello-world

will run this scenario.  Modify scenarios/my-sat-hello-world/Makefile and scenarios/my-sat-hello-world/solve.cpp to add your own custom behavior.  It is intended that code in "scenarios" should behave like a "quick script", setting up the solver and running, whereas code in "src" should be stable code that is broadly applicable.  

The author's most recent scenario, graphhom, is available, but due to its constantly changing nature, is not worth attempting to document here.

Extending satcolors
====================
Satcolors is intended to be an extensible framework of mutually compatible objects upon which requirements can be made.  Any custom objects should:
* be default constructible, copy-assignable, and copy-constructible.
* implement a constructor
MyObject::MyObject(SolverManager* mgr, {args}, unsigned int Var& startingVar = SolverManager::allocateNew)
Note that the last argument is a non-const reference with very particular semantics; see cardinal.h for a detailed description.
* implement diffSolnReq() which should return, after the solver has successfully found at least one solution, a requirement that the object's state be different on the next run.
* implement currSolnReq(), the negation of diffSolnReq().
* implement typeRequirement(), which returns any requirements defining the object (e.g., a Cardinal cannot take two values simultaneously, and it must take at least one value)
* implement startingVar(), the address in the solver's space of variables of the first boolean variable used by the object
* implement numLiterals(), the numer of boolean variables used by the object.
* if applicable, implement modelValue(), which returns some meaningful representation of the object after the solver has found a solution.  For example, Cardinal::modelValue() returns the number (int) that cardinal has been assigned to. 

Cardinal and Ordinal are both reasonably legible examples of objects that follow this pattern.  Since many of these methods are interrelated, the object in compoundobject.h can be used to simplify the task of creating new objects which are simply compounds of existing objects, but possibly with additional requirements.  An example of a class created with CompoundObject is AlmostLines, which was needed as part of a scenario the author worked on some time ago.

Archive
========
The intended workflow is to repeatedly modify one or more source files in a scenario subdirectory and run.  In running many variations of the same experiment, it is easy to lose track of old solutions.  Thus, the code automatically copies the source files, data, and output of any run to the "archive" directory, indexed by run time.  In theory, one should be able to reconstruct any previous run using the information contained there, as long as the code in src remains sufficiently stable.

Python Code
============
The python code is intended to work with the graphhom scenario; it performs some pre-processing of the graphml files and visualizes solutions.  It is very much a work in progress; in the future I will either fix it up or remove it from the repository.
