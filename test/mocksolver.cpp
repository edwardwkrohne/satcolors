// -*- Mode:C++ -*-
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
