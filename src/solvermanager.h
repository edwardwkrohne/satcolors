// -*- Mode:C++ -*-
//
// Wraps a solver, provides an iterator, and helps with managing variables.
// Also handles interpreting the solver output in a clean, convenient way.
//
// Member functions are virtual for eventual mocking purposes.  This
// also makes it possible to avoid linking a particular solver library
// (like minisat), and still test code that writes requirements.

#ifndef SOLVERMANAGER_H
#define SOLVERMANAGER_H

#include <iostream>
#include "requirement.h"
#include <minisat/core/Solver.h>

class SolverManager {
public:
  // Constructor
  SolverManager();
  virtual ~SolverManager();

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual unsigned int newVars(unsigned int numReservations);

  // Register a single requirement
  virtual void require(const Requirement& req);
  void require(const DualClause& clause) { require(Requirement(clause)); };
  void require(const Clause& clause);
  void require(Literal lit) { require(Clause(lit)); };

  // Get an insertion iterator for adding requirements.
  //  virtual SolverIter getIter();

  // Solve
  virtual bool solve();
  virtual bool solve(Literal lit);
  virtual bool solve(Literal lit1, Literal lit2);
  virtual bool solve(Literal lit1, Literal lit2, Literal lit3);
  virtual bool solve(const DualClause& assumptions);

  // Find out whether the last run was successful
  virtual bool okay() const;

  // Query the value of a particular variable.
  virtual bool modelValue(unsigned int var) const;

  // Use this as startingVar when you want to allocate new variables.  Objects that take
  //  this shouldn't update it.  If bugs crop up where they do, I can replace this
  //  relatively easily with an object that disallows writing.
  static unsigned int allocateNew;
private:
  bool successfulRun;
  Minisat::Solver solver;
};

#endif // SOLVERMANAGER_H
