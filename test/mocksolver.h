// -*- Mode:C++ -*-
//
// Mock solver object with which to do unit tests
//
#ifndef MOCKSOLVER_H
#define MOCKSOLVER_H

#include "../src/solver.h"

class MockSolver : public Solver {
public:
  // Constructor
  MockSolver();

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual unsigned int newVars(unsigned int numReservations) override;

  // Register a single requirement
  using Solver::require;
  virtual void require(const Clause& clause) override;

  // Solve
  using Solver::solve;
  virtual bool solve(const DualClause& assumptions) override;

  // Find out whether the last run was successful
  virtual bool okay() const override;

  // Query the value of a particular variable.
  virtual bool modelValue(unsigned int var) const override;

  // Get the requirements required so far (returns const reference)
  const Requirement& getRequirements() const;

private:
  unsigned int varSpaceSize;
  Requirement requirements;
};

#endif // MINISATSOLVER_H
