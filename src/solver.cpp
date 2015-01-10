// -*- Mode:C++ -*-
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
