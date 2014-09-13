// -*- Mode:C++ -*-
//
// A simple output iterator designed for adding vectors of literals to a solver
// This class is a simple wrapper class.  It is designed to facilitate unit testing
// of classes that add clauses to a solver.

#include <algorithm>
#include "solveriter.h"

using namespace std;

using Minisat::Solver;
using Minisat::vec;
using Minisat::var;

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
const vec<Minisat::Lit>& SolverIter::operator=(const vec<Minisat::Lit>& clause) const {
  for (int i=0; i < clause.size(); i++ ) {
    while ( solver.nVars() <= abs(var(clause[i])) ) {
      solver.newVar();
    }
  }
  solver.addClause(clause);

  return clause;
}

// Assign
Literal SolverIter::operator=(const Literal unit) const {
  vec<Minisat::Lit> vecClause;
  vecClause.push(Minisat::mkLit(unit.getVar(), unit.isPos()));
  
  *this = vecClause;

  return unit;
}

// Assign
const Clause& SolverIter::operator=(const Clause& clause) const {
  vec<Minisat::Lit> vecClause;
  for ( auto lit : clause ) {
    vecClause.push(Minisat::mkLit(lit.getVar(), lit.isPos()));
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
