// -*- Mode:C++ -*-
//
// Solver object using minisat to do the solving.
//
#ifndef MINISATSOLVER_H
#define MINISATSOLVER_H

#include <iostream>
#include "requirement.h"
#include "solver.h"
#include <minisat/core/Solver.h>

class MinisatSolver : public Solver {
public:
  // Constructor
  MinisatSolver();

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual unsigned int newVars(unsigned int numReservations) override;

  // Register a single requirement (do I need to explicitly state the extra overloads of require?)
  using Solver::require;
  virtual void require(const Clause& clause) override;

  // Solve
  virtual bool solve() override;
  virtual bool solve(Literal lit) override;
  virtual bool solve(Literal lit1, Literal lit2) override;
  virtual bool solve(Literal lit1, Literal lit2, Literal lit3) override;
  virtual bool solve(const DualClause& assumptions) override;

  // Find out whether the last run was successful
  virtual bool okay() const override;

  // Query the value of a particular variable.
  virtual bool modelValue(unsigned int var) const override;

private:
  bool successfulRun;
  Minisat::Solver solver;
};

#endif // MINISATSOLVER_H
