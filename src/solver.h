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
};

#endif // SOLVER_H
