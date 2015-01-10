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
// Implementation of the MockSolver

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "mocksolver.h"

using std::string;

// Constructor
MockSolver::MockSolver() :
  varSpaceSize(0)
{
  ;
}

// Reserve some variables.  Returns a variable corresponding to the literal reserved.
// Results are undefined if numReservations is less than 0.
unsigned int MockSolver::newVars(unsigned int numReservations) {
  if ( numReservations < 0 ) {
    CPPUNIT_FAIL("Requested " + std::to_string(numReservations) + 
                 " variables with MockSolver::newVars, which is negative.");
  }

  unsigned int firstVar = varSpaceSize;
  varSpaceSize += numReservations;
  return firstVar;
}

// Register a single requirement
void MockSolver::require(const Clause& clause) {
  for ( auto lit : clause ) {
    if ( abs(lit.getVar()) >= varSpaceSize ) {
      CPPUNIT_FAIL("Attempted to require a clause with literal " + 
                   std::to_string(abs(lit.getVar())) + 
                   " but the variable space is only size " + std::to_string(varSpaceSize));
    }
  }
  requirements &= clause;
}

// Solve
bool MockSolver::solve(const DualClause& assumptions) {
  CPPUNIT_FAIL("MockSolver::solve not implemented");
  return false;
}


// Find out whether the last run was successful
bool MockSolver::okay() const {
  CPPUNIT_FAIL("MockSolver::okay not implemented.");
  return false;
}

// Query the value of a particular variable.
bool MockSolver::modelValue(unsigned int var) const {
  CPPUNIT_FAIL("MockSolver::modelValue not implemented.");
  return false;
}

const Requirement& MockSolver::getRequirements() const {
  return requirements;
}
