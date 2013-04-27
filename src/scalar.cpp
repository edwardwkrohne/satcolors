// -*- Mode:C++ -*-
//
// Implementation of the Scalar class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "scalar.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
typedef Scalar::value_type value_type;
using namespace std;

// Creates an object representing a scalar.  Does not register requirements.
Scalar::Scalar(SolverManager& _manager, value_type _min, value_type _max, Var& _startingVar) :
  min(_min),
  max(_max),
  manager(_manager),
  startingVar(_startingVar == SolverManager::allocateNew ? _manager.newVars(getNumLiterals()) : _startingVar)
{
  if ( _startingVar == SolverManager::allocateNew ) {
    manager.require(typeRequirement());
  } else {
    _startingVar += getNumLiterals();
  }
}

// The corresponding requirement of being a scalar --
// must take a value between min and max, and cannot take two values simultaneously.
Requirement Scalar::typeRequirement() const {
  Requirement result;

  // Build the clause requiring at least one value
  Clause atLeastOneValue;
  for ( value_type value = min; value < max; value++ ) {
    atLeastOneValue |= *this == value;
  }
  result &= move(atLeastOneValue);

  // Build clauses requiring not more than one value
  for ( value_type value1 = min; value1 < max; value1++ ) {
    for ( value_type value2 = value1+1; value2 < max; value2++ ) {
      result &= *this != value1 | *this != value2;
    }
  }

  return result;
}

// The number of (contiguous) literals required to represent this scalar.
// Equal to max-min.
unsigned int Scalar::getNumLiterals() const {
  return max-min;
}


// Copy constructor.  Does not register requirements.
Scalar::Scalar(const Scalar& copy) :
    manager(copy.manager),
    startingVar(copy.startingVar),
    min(copy.min),
    max(copy.max)
{
  ;
}

void Scalar::checkDomain(const value_type arg) const {
  if ( arg < min || arg >= max ) {
    ostringstream sout;
    sout << "Incorrect value comparison for Scalar.  This scalar min=" << min << " and max=" << max << " but " << arg << " requested.";
    throw domain_error(sout.str());
  }
}

// Addition of a scalar by a constant.  Surprisingly easy to implement, and useful.
// If scl is a Scalar, then scl+1 returns a scalar that is equal to n+1 iff scl is equal to n.
// Uses no additional literals or requirements.
Scalar Scalar::operator+(const value_type rhs) const {
  Var var = startingVar;
  return Scalar(manager, min+rhs, max+rhs, var);
}
Scalar Scalar::operator-(const value_type rhs) const {
  return *this + (-rhs);
}

// Simple literals indicating equality with a specific scalar rhs.  If rhs is out of bounds,
// behavior is undefined.
Minisat::Lit Scalar::operator==(value_type rhs) const {
  checkDomain(rhs);
  return mkLit( Var(rhs - min) + startingVar);
}
Minisat::Lit Scalar::operator!=(value_type rhs) const {
  checkDomain(rhs);
  return ~(*this == rhs);
}

// Comparison operators
Clause Scalar::operator>(value_type rhs) const {
  return *this >= rhs+1;
}

Clause Scalar::operator>=(value_type rhs) const {
  Clause result;
  value_type start = rhs < min ? min : rhs;
  for ( value_type i = start; i < max; i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Scalar::operator<(value_type rhs) const {
  Clause result;
  value_type end = rhs < max ? rhs : max;
  for ( value_type i = min; i < end; i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Scalar::operator<=(value_type rhs) const {
  return *this < rhs+1;
}

Clause operator>(value_type lhs, const Scalar& rhs) {
  return rhs < lhs;
}

Clause operator>=(value_type lhs, const Scalar& rhs) {
  return rhs <= lhs;
}

Clause operator<(value_type lhs, const Scalar& rhs) {
  return rhs > lhs;
}

Clause operator<=(value_type lhs, const Scalar& rhs) {
  return rhs >= lhs;
}


// Requirements that two Numbers be equal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each scalar to be the same, or even
// overlap.
Requirement Scalar::operator==(const Scalar& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Scalar& lhs = *this;

  Requirement result;

  result &= lhs >= rhs.min;
  result &= lhs < rhs.max;
  result &= rhs >= lhs.min;
  result &= rhs < lhs.max;

  value_type start = lhs.min < rhs.min ? rhs.min : lhs.min;
  value_type end   = lhs.max > rhs.max ? rhs.max : lhs.max;

  // Run through all possible values that either scalar can take.
  for ( value_type val = start; val < end; val++ ) {
    result &= lhs != val | rhs == val;
    result &= lhs == val | rhs != val;
  }

  return result;
}

// Requirements that two Numbers be nonequal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each scalar to be the same, or even
// overlap.
Requirement Scalar::operator!=(const Scalar& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Scalar& lhs = *this;

  Requirement result;

  value_type maxMin = lhs.min < rhs.min ? rhs.min : lhs.min;
  value_type minMax = lhs.max > rhs.max ? rhs.max : lhs.max;

  // Run through all values that BOTH scalars can take
  for ( value_type val = maxMin; val < minMax; val++ ) {
    result &= lhs != val | rhs != val;
  }

  return result;
}
// The value assigned in the model, after solving, if a solution is available.
value_type Scalar::modelValue() const {
  for (value_type value = 0; value < max-min; value++ ) {
    if ( manager.modelValue(Var(value + startingVar) ) == true ) {
      return value+min;
    }
  }

  // Should never reach here.  Results undefined.
  return -1;
}

// After a solution has been found, a requirement for a different solution
Lit Scalar::diffSolnReq() const {
  return (*this) != this->modelValue();
}
Lit Scalar::currSolnReq() const {
  return (*this) == this->modelValue();
}

Scalar::operator value_type() const {
  return modelValue();
}

Lit operator==(value_type lhs, const Scalar& rhs) {
  return rhs == lhs;
}

Lit operator!=(value_type lhs, const Scalar& rhs) {
  return rhs == lhs;
}
