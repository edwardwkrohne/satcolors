// -*- Mode:C++ -*-
//
// Wraps a solver, provides an iterator, and helps with managing variables.
// Also handles interpreting the solver output in a clean, convenient way.
//
// Member functions are virtual for eventual mocking purposes.  Also, the way
// the Makefile is set up, the tests won't link if these functions aren't
// virtual.


#ifndef SOLVERMANAGER_H
#define SOLVERMANAGER_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solveriter.h"

class SolverManager {
public:
  // Constructor
  SolverManager();
  virtual ~SolverManager();

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual Minisat::Var newVars(unsigned int numReservations);

  // Register a single requirement
  virtual void require(const Requirement& req);
  void require(const DualClause& clause) { require(Requirement(clause)); };
  void require(const Clause& clause) { require(Requirement(clause)); };
  void require(Minisat::Lit lit) { require(Requirement(lit)); };

  // Get an insertion iterator for adding requirements.
  virtual SolverIter getIter();

  // Solve
  virtual bool solve();
  virtual bool solve(Minisat::Lit lit);
  virtual bool solve(Minisat::Lit lit1, Minisat::Lit lit2);
  virtual bool solve(Minisat::Lit lit1, Minisat::Lit lit2, Minisat::Lit lit3);
  virtual bool solve(const DualClause& assumptions);

  // Find out whether the last run was successful
  virtual bool okay() const;

  // Query the value of a particular variable.
  virtual bool modelValue(Minisat::Var var) const;

  // Use this as startingVar when you want to allocate new variables.  Objects that take
  //  this shouldn't update it.  If bugs crop up where they do, I can replace this
  //  relatively easily with an object that disallows writing.
  static Minisat::Var allocateNew;
private:
  bool successfulRun;
  Minisat::Solver solver;
};

#endif // SOLVERMANAGER_H
