// -*- Mode:C++ -*-
//
// Abstract class to wrap a solver and helps with managing variables.
// Also handles interpreting the solver output in a clean, convenient way.
//
// Provides sensible default implementations of several methods, which
// call the pure virtual methods.

#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include "requirement.h"
#include <minisat/core/Solver.h>

class Solver {
public:
  // Constructor
  Solver() = default;
  virtual ~Solver() = default;

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual unsigned int newVars(unsigned int numReservations) = 0;

  // Register a single requirement
  virtual void require(const Requirement& req);
  virtual void require(const DualClause& dClause);
  virtual void require(Literal lit);
  virtual void require(const Clause& clause) = 0;

  // Solve
  virtual bool solve();
  virtual bool solve(Literal lit);
  virtual bool solve(Literal lit1, Literal lit2);
  virtual bool solve(Literal lit1, Literal lit2, Literal lit3);
  virtual bool solve(const DualClause& assumptions) = 0;

  // Find out whether the last run was successful
  virtual bool okay() const = 0;

  // Query the value of a particular variable.
  virtual bool modelValue(unsigned int var) const = 0;

  // Use this as startingVar when you want to allocate new variables.  Objects that take
  //  this shouldn't update it.  If bugs crop up where they do, I can replace this
  //  relatively easily with an object that disallows writing.
  static unsigned int allocateNew;
};

#endif // SOLVER_H
