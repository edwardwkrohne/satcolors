// -*- Mode:C++ -*-
//
// Implementation of the SolverManager class

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "solvermanager.h"

using namespace std;
using Minisat::Solver;
using Minisat::vec;
using Minisat::var;
using Minisat::lbool;

unsigned int SolverManager::allocateNew = ~(unsigned int)0;

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
unsigned int SolverManager::newVars(unsigned int numReservations) {
  if ( numReservations <= 0 ) {
    return solver.nVars();
  }

  unsigned int firstVar = solver.newVar();
  while ( --numReservations ) {
    solver.newVar();
  }
  return firstVar;
}

// Register a single requirement
void SolverManager::require(const Clause& clause) {
  vec<Minisat::Lit> vecClause;
  
  for ( auto lit : clause ) {
    if ( solver.nVars() <= abs(lit.getVar()) ) {
      std::ostringstream sout;
      sout << "solver's variable space does not accommodate new literal "
	   << "(" << solver.nVars() << " -> " << abs(lit.getVar()) << ")." << endl
	   << "This should not be necessary, why is this not already done?.";
      throw std::out_of_range(sout.str());
    }
  
    vecClause.push(Minisat::mkLit(lit.getVar(), lit.isPos()));
  }
  
  solver.addClause(vecClause);
}


// Register a single requirement
void SolverManager::require(const Requirement& req) {
  for ( auto clause : req ) {
    require(clause);
  }
}

// Solve
bool SolverManager::solve() {
  solver.simplify();
  return successfulRun = solver.solve();
}
bool SolverManager::solve(Literal lit) {
  solver.simplify();
  return successfulRun = solver.solve(Minisat::mkLit(lit.getVar(), lit.isPos()));
}
bool SolverManager::solve(Literal lit1, Literal lit2) {
  solver.simplify();
  return successfulRun = solver.solve(Minisat::mkLit(lit1.getVar(), lit1.isPos()), 
				      Minisat::mkLit(lit2.getVar(), lit2.isPos()));
}
bool SolverManager::solve(Literal lit1, Literal lit2, Literal lit3) {
  solver.simplify();
  return successfulRun = solver.solve(Minisat::mkLit(lit1.getVar(), lit1.isPos()), 
				      Minisat::mkLit(lit2.getVar(), lit2.isPos()), 
				      Minisat::mkLit(lit3.getVar(), lit3.isPos()));
}
bool SolverManager::solve(const DualClause& assumptions) {
  vec<Minisat::Lit> vecAssumps(0);
  for ( auto assump : assumptions ) {
    vecAssumps.push(Minisat::mkLit(assump.getVar(), !assump.isPos()));
  }
  return successfulRun = solver.solve(vecAssumps);
}


// Find out whether the last run was successful
bool SolverManager::okay() const {
  return solver.okay();
}

// Query the value of a particular variable.
bool SolverManager::modelValue(unsigned int var) const {
  // Check for bad conditions.
  if ( !successfulRun ) {
    throw logic_error("SolverManager::modelValue called, but no model is ready. Must follow a call to solve() which was satisfiable.");
  }
  if ( var >= solver.nVars() || var < 0 ) {
    throw out_of_range("SolverManager::modelValue called requesting a variable out of range");
  }
  // Somewhat counterintuitive, but I blame this on minisat's
  // interface.  False is true, and true is false.  I could
  // equivalently negate all the literals going into the solver, but
  // it's easier to negate variables coming out.
  return solver.modelValue(var) == l_False;
}
