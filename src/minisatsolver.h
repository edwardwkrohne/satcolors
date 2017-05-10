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
// Solver object using minisat to do the solving.
//
#ifndef MINISATSOLVER_H
#define MINISATSOLVER_H

#include <minisat/core/Solver.h>
#include <chrono>
#include "solver.h"

class MinisatSolver : public Solver {
public:
  // Constructor
  MinisatSolver();

  // Reserve some variables.  Returns a variable corresponding to the literal reserved.
  virtual unsigned int newVars(unsigned int numReservations) override;

  // Register a single requirement
  using Solver::require;
  virtual void require(const Clause& clause) override;

  // Solve
  virtual bool solve() override;
  virtual bool solve(Literal lit) override;
  virtual bool solve(Literal lit1, Literal lit2) override;
  virtual bool solve(Literal lit1, Literal lit2, Literal lit3) override;
  virtual bool solve(std::chrono::microseconds dur, const DualClause& assumptions = DualClause());
  virtual bool solve(const DualClause& assumptions) override;

  // Find out whether the last run was successful
  virtual bool okay() const override;
  virtual bool interrupted() const;

  // Query the value of a particular variable.
  virtual bool modelValue(unsigned int var) const override;

private:
  enum truefalseinterrupt {
    tfiFalse = 0,
    tfiTrue = 1,
    tfiInterrupt = 2,
  };
  
  truefalseinterrupt successfulRun;
  Minisat::Solver solver;
};

#endif // MINISATSOLVER_H
