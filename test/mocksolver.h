// -*- Mode: C++ -*-
////////////////////////////////////////////////////////////////////////////
//
//  satcolors -- for writably and readably building problems for SAT-solving
//  Copyright (C) 2014 Edward W. Krohne III
//
//  The research that led to this product was partially supported by the
//  U.S. NSF grants DMS-0943870 and DMS-1201290.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////
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
