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
// Implementation of the Solver class

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "solver.h"

// Register a single requirement
void Solver::require(const Requirement& req) {
  for ( auto clause : req ) {
    require(clause);
  }
}

void Solver::require(const DualClause& dClause) {
  require(Requirement(dClause));
}

void Solver::require(Literal lit) {
  require(Clause(lit));
}

// Solve
bool Solver::solve() {
  solve(DualClause());
}
bool Solver::solve(Literal lit) {
  solve(DualClause(lit));
}
bool Solver::solve(Literal lit1, Literal lit2) {
  solve(lit1 & lit2);
}
bool Solver::solve(Literal lit1, Literal lit2, Literal lit3) {
  solve(lit1 & lit2 & lit3);
}
