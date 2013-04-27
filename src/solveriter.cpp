// -*- Mode:C++ -*-
//
// A simple output iterator designed for adding vectors of literals to a solver
// This class is a simple wrapper class.  It is designed to facilitate unit testing
// of classes that add clauses to a solver.

#include <algorithm>
#include "solveriter.h"

using Minisat::Solver;
using Minisat::vec;
using Minisat::Lit;

// Constructor
SolverIter::SolverIter(Solver& _solver) :
  solver(_solver)
{
  
}

// Return assignable object
const SolverIter& SolverIter::operator*() const {
  return *this;
}

// Assign
const vec<Lit>& SolverIter::operator=(const vec<Lit>& clause) const {
  for (int i=0; i < clause.size(); i++ ) {
    while ( solver.nVars() <= var(clause[i]) ) {
      solver.newVar();
    }
  }
  solver.addClause(clause);

  return clause;
}

// Assign
Lit SolverIter::operator=(const Lit unit) const {
  vec<Lit> vecClause;
  vecClause.push(unit);
  
  *this = vecClause;

  return unit;
}

// Assign
const Clause& SolverIter::operator=(const Clause& clause) const {
  vec<Lit> vecClause;
  for ( auto iter = clause.begin(); iter != clause.end(); iter++) {
    vecClause.push(*iter);
  }

  *this = vecClause;
  return clause;
}

const Requirement& SolverIter::operator=(const Requirement& req) const {
  std::copy(req.begin(), req.end(), *this);
  return req;
}


// Increment (no-op, as usual for output iterators)
const SolverIter& SolverIter::operator++() const {
  return *this;
}

// Increment (no-op, as usual for output iterators)
const SolverIter& SolverIter::operator++(int) const {
  return *this;
}
