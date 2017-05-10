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
// Implementation of the MinisatSolver

#include <stdexcept>
#include <sstream>
#include <future>
#include <chrono>
#include "minisatsolver.h"
#include "manipulators.h"

using namespace std;
using Minisat::vec;
using Minisat::var;
using Minisat::lbool;
using Minisat::l_False;
using Minisat::l_True;

// Constructor
MinisatSolver::MinisatSolver() :
  successfulRun(tfiFalse)
{
  ;
}

// Reserve some variables.  Returns a variable corresponding to the literal reserved.
// Results are undefined if numReservations is less than 0.
unsigned int MinisatSolver::newVars(unsigned int numReservations) {
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
void MinisatSolver::require(const Clause& clause) {
  vec<Minisat::Lit> vecClause;
  
  for ( auto lit : clause ) {
    if ( solver.nVars() <= lit.getVar() ) {
      std::ostringstream sout;
      sout << "solver's variable space does not accommodate new literal "
	   << "(" << solver.nVars() << " -> " << lit.getVar() << ")." << endl
	   << "This should not be necessary, why is this not already done?.";
      throw std::out_of_range(sout.str());
    }
  
    vecClause.push(Minisat::mkLit(lit.getVar(), lit.isPos()));
  }
  
  solver.addClause(vecClause);
}

// Solve
bool MinisatSolver::solve() {
  return solve(DualClause());
}
bool MinisatSolver::solve(Literal lit) {
  return solve(DualClause(lit));
}
bool MinisatSolver::solve(Literal lit1, Literal lit2) {
  return solve (lit1 & lit2);
}
bool MinisatSolver::solve(Literal lit1, Literal lit2, Literal lit3) {
  return solve(lit1 & lit2 & lit3);
}

bool MinisatSolver::solve(std::chrono::microseconds dur, const DualClause& assumptions) {
  auto func = [&] {return this->solve(assumptions);};

  std::future<bool> result(std::async(std::launch::async, func));
  switch ( result.wait_for(dur) ) {
  case future_status::ready:
    return result.get();
  default:
    solver.interrupt();
    result.wait();
    solver.clearInterrupt();
    successfulRun = tfiInterrupt;
    return okay();
  }
}

bool MinisatSolver::solve(const DualClause& assumptions) {
  // Load the assumptions into a minisat-style "vec"
  vec<Minisat::Lit> vecAssumps(0);
  for ( auto assump : assumptions ) {
    vecAssumps.push(Minisat::mkLit(assump.getVar(), !assump.isPos()));
  }

  // simplify
  solver.simplify();

  successfulRun = solver.solve(vecAssumps) ? tfiTrue : tfiFalse;
  return okay();
}


// Find out whether the last run was successful
bool MinisatSolver::okay() const {
  return successfulRun == tfiTrue;
}

bool MinisatSolver::interrupted() const {
  return successfulRun == tfiInterrupt;
}

// Query the value of a particular variable.
bool MinisatSolver::modelValue(unsigned int var) const {
  // Check for bad conditions.
  if ( successfulRun != tfiTrue ) {
    throw logic_error("MinisatSolver::modelValue called, but no model is ready. Must follow a call to solve() which was satisfiable.");
  }
  if ( var >= solver.nVars() || var < 0 ) {
    throw out_of_range("MinisatSolver::modelValue called requesting a variable out of range");
  }
  // Somewhat counterintuitive, but I blame this on minisat's
  // interface.  False is true, and true is false.  I could
  // equivalently negate all the literals going into the solver, but
  // it's easier to negate variables coming out.
  return solver.modelValue(var) == l_False;
}
