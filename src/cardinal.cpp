// -*- Mode:C++ -*-
//
// Implementation of the Cardinal class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "cardinal.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
typedef Cardinal::value_type value_type;
using namespace std;

// Creates an object representing a cardinal.
Cardinal::Cardinal(SolverManager& _manager, value_type _min, value_type _max, Var& _startingVar) :
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

// The corresponding requirement of being a cardinal --
// must take a value between min and max, and cannot take two values simultaneously.
Requirement Cardinal::typeRequirement() const {
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

// The number of (contiguous) literals required to represent this cardinal.
// Equal to max-min.
unsigned int Cardinal::getNumLiterals() const {
  return max-min;
}


// Copy constructor.  Does not register requirements.
Cardinal::Cardinal(const Cardinal& copy) :
    manager(copy.manager),
    startingVar(copy.startingVar),
    min(copy.min),
    max(copy.max)
{
  ;
}

void Cardinal::checkDomain(const value_type arg) const {
  if ( arg < min || arg >= max ) {
    ostringstream sout;
    sout << "Incorrect value comparison for Cardinal.  This cardinal min=" << min << " and max=" << max << " but " << arg << " requested.";
    throw domain_error(sout.str());
  }
}

// Addition of a cardinal by a constant.  Surprisingly easy to implement, and useful.
// If idx is a Cardinal, then idx+1 returns a cardinal that is equal to n+1 iff idx is equal to n.
// Uses no additional literals or requirements.
Cardinal Cardinal::operator+(const value_type rhs) const {
  Var var = startingVar;
  return Cardinal(manager, min+rhs, max+rhs, var);
}
Cardinal Cardinal::operator-(const value_type rhs) const {
  return *this + (-rhs);
}

// Simple literals indicating equality with a specific cardinal rhs.  If rhs is out of bounds,
// behavior is undefined.
Minisat::Lit Cardinal::operator==(value_type rhs) const {
  checkDomain(rhs);
  return mkLit( Var(rhs - min) + startingVar);
}
Minisat::Lit Cardinal::operator!=(value_type rhs) const {
  checkDomain(rhs);
  return ~(*this == rhs);
}

// Comparison operators
Clause Cardinal::operator>(value_type rhs) const {
  return *this >= rhs+1;
}

Clause Cardinal::operator>=(value_type rhs) const {
  Clause result;
  value_type start = rhs < min ? min : rhs;
  for ( value_type i = start; i < max; i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Cardinal::operator<(value_type rhs) const {
  Clause result;
  value_type end = rhs < max ? rhs : max;
  for ( value_type i = min; i < end; i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Cardinal::operator<=(value_type rhs) const {
  return *this < rhs+1;
}

Clause operator>(value_type lhs, const Cardinal& rhs) {
  return rhs < lhs;
}

Clause operator>=(value_type lhs, const Cardinal& rhs) {
  return rhs <= lhs;
}

Clause operator<(value_type lhs, const Cardinal& rhs) {
  return rhs > lhs;
}

Clause operator<=(value_type lhs, const Cardinal& rhs) {
  return rhs >= lhs;
}


// Requirements that two Numbers be equal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each cardinal to be the same, or even
// overlap.
Requirement Cardinal::operator==(const Cardinal& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Cardinal& lhs = *this;

  Requirement result;

  result &= lhs >= rhs.min;
  result &= lhs < rhs.max;
  result &= rhs >= lhs.min;
  result &= rhs < lhs.max;

  value_type start = lhs.min < rhs.min ? rhs.min : lhs.min;
  value_type end   = lhs.max > rhs.max ? rhs.max : lhs.max;

  // Run through all possible values that either cardinal can take.
  for ( value_type val = start; val < end; val++ ) {
    result &= lhs != val | rhs == val;
    result &= lhs == val | rhs != val;
  }

  return result;
}

// Requirements that two Numbers be nonequal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each cardinal to be the same, or even
// overlap.
Requirement Cardinal::operator!=(const Cardinal& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Cardinal& lhs = *this;

  Requirement result;

  value_type maxMin = lhs.min < rhs.min ? rhs.min : lhs.min;
  value_type minMax = lhs.max > rhs.max ? rhs.max : lhs.max;

  // Run through all values that BOTH cardinals can take
  for ( value_type val = maxMin; val < minMax; val++ ) {
    result &= lhs != val | rhs != val;
  }

  return result;
}
// The value assigned in the model, after solving, if a solution is available.
value_type Cardinal::modelValue() const {
  for (value_type value = 0; value < max-min; value++ ) {
    if ( manager.modelValue(Var(value + startingVar) ) == true ) {
      return value+min;
    }
  }

  // Should never reach here.  Results undefined.
  return -1;
}

// After a solution has been found, a requirement for a different solution
Lit Cardinal::diffSolnReq() const {
  return (*this) != this->modelValue();
}
Lit Cardinal::currSolnReq() const {
  return (*this) == this->modelValue();
}

Cardinal::operator value_type() const {
  return modelValue();
}

Lit operator==(value_type lhs, const Cardinal& rhs) {
  return rhs == lhs;
}

Lit operator!=(value_type lhs, const Cardinal& rhs) {
  return rhs == lhs;
}
