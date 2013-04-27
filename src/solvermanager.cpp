// -*- Mode:C++ -*-
//
// Implementation of the SolverManager class

#include <algorithm>
#include "solvermanager.h"
#include <stdexcept>

using namespace std;
using Minisat::Solver;
using Minisat::Var;
using Minisat::vec;
using Minisat::lbool;
using Minisat::Lit;

Minisat::Var SolverManager::allocateNew = ~(Minisat::Var)0;

// Constructor
SolverManager::SolverManager() :
  successfulRun(false)
{
  ;
}

// Destructor
SolverManager::~SolverManager() {
  ;
}

// Reserve some variables.  Returns a variable corresponding to the literal reserved.
// Results are undefined if numReservations is less than 0.
Var SolverManager::newVars(unsigned int numReservations) {
  if ( numReservations <= 0 ) {
    return solver.nVars();
  }

  Var firstVar = solver.newVar();
  while ( --numReservations ) {
    solver.newVar();
  }
  return firstVar;
}

// Register a single requirement
void SolverManager::require(const Requirement& req) {
  copy(req.begin(), req.end(), getIter());
}

// Get an insertion iterator for adding requirements.
SolverIter SolverManager::getIter() {
  return SolverIter(solver);
}

// Solve
bool SolverManager::solve() {
  solver.simplify();
  return successfulRun = solver.solve();
}
bool SolverManager::solve(Lit lit) {
  solver.simplify();
  return successfulRun = solver.solve(lit);
}
bool SolverManager::solve(Lit lit1, Lit lit2) {
  solver.simplify();
  return successfulRun = solver.solve(lit1, lit2);
}
bool SolverManager::solve(Lit lit1, Lit lit2, Lit lit3) {
  solver.simplify();
  return successfulRun = solver.solve(lit1, lit2, lit3);
}
bool SolverManager::solve(const DualClause& assumptions) {
  vec<Lit> vecAssump(assumptions.size());
  for ( auto iter = assumptions.begin(); iter != assumptions.end(); iter++ ) {
     vecAssump.push(~*iter);
  }
  return successfulRun = solver.solve(vecAssump);
}


// Find out whether the last run was successful
bool SolverManager::okay() const {
  return solver.okay();
}

// Query the value of a particular variable.
bool SolverManager::modelValue(Var var) const {
  // Check for bad conditions.
  if ( !successfulRun ) {
    throw logic_error("SolverManager::modelValue called, but no model is ready. Must follow a call to solve() which was satisfiable.");
  }
  if ( var >= solver.nVars() || var < 0 ) {
    throw out_of_range("SolverManager::modelValue called requesting a variable out of range");
  }
  return solver.modelValue(var) == l_True;
}
