// -*- Mode:C++ -*-
//
// Implementation of the Ordinal class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ordinal.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
typedef Ordinal::value_type value_type;
using namespace std;

// Creates an object representing a ordinal.  Does not register requirements.
Ordinal::Ordinal(SolverManager& _manager, value_type _min, value_type _max, Var& _startingVar) :
  min(_min),
  max(_max),
  manager(_manager),
  startingVar(_startingVar == SolverManager::allocateNew ? _manager.newVars(getNumLiterals()) : _startingVar)
{
  if ( _max <= _min ) {
    throw domain_error("Cannot create an ordinal with an empty range of possible values.");
  }
  if ( _startingVar == SolverManager::allocateNew ) {
    manager.require(typeRequirement());
  } else {
    _startingVar += getNumLiterals();
  }
}

// The corresponding requirement of being a ordinal --
// must take a value between min and max, and cannot take two values simultaneously.
Requirement Ordinal::typeRequirement() const {
  Requirement result;
  for (int i = min; i < max-2; i++ ) {
    result &= implication(*this <= i, *this <= i+1);
  }
  return result;
}

// The number of (contiguous) literals required to represent this ordinal.
// Equal to max-min.
unsigned int Ordinal::getNumLiterals() const {
  return max-min-1;
}


// Copy constructor.  Does not register requirements.
Ordinal::Ordinal(const Ordinal& copy) :
    manager(copy.manager),
    startingVar(copy.startingVar),
    min(copy.min),
    max(copy.max)
{
  ;
}

// Addition of a ordinal by a constant.  Surprisingly easy to implement, and useful.
// If scl is a Ordinal, then scl+1 returns a ordinal that is equal to n+1 iff scl is equal to n.
// Uses no additional literals or requirements.
Ordinal Ordinal::operator+(const value_type rhs) const {
  Var var = startingVar;
  return Ordinal(manager, min+rhs, max+rhs, var);
}
Ordinal Ordinal::operator-(const value_type rhs) const {
  return *this + (-rhs);
}

// Simple literals indicating equality with a specific ordinal rhs.  If rhs is out of bounds,
// behavior is undefined.
DualClause Ordinal::operator==(value_type rhs) const {
  if ( rhs < min || rhs >= max ) {
    ostringstream sout;
    sout << "Incorrect value comparison for Ordinal.  This ordinal min=" << min << " and max=" << max << " but " << rhs << " requested.";
    throw domain_error(sout.str());
  }

  DualClause result;

  if ( rhs > min )
    result &= *this >= rhs;

  if ( rhs < max-1 )
    result &= *this <= rhs;

  return result;
}
Clause Ordinal::operator!=(value_type rhs) const {
  return ~(*this == rhs);
}

// Comparison operators
Lit Ordinal::operator>(value_type rhs) const {
  return ~(*this <= rhs);
}

Lit Ordinal::operator>=(value_type rhs) const {
  return ~(*this < rhs);
}

Lit Ordinal::operator<(value_type rhs) const {
  if ( rhs <= min || rhs >= max ) {
    ostringstream sout;
    sout << "Incorrect value comparison for Ordinal.  This ordinal is in the interval [" << min << ", " << max << ") but less than " << rhs << " requested.";
    throw domain_error(sout.str());
  }

  return mkLit(rhs-1-min+startingVar);
}

Lit Ordinal::operator<=(value_type rhs) const {
  return *this < rhs+1;
}

Lit operator>(value_type lhs, const Ordinal& rhs) {
  return rhs < lhs;
}

Lit operator>=(value_type lhs, const Ordinal& rhs) {
  return rhs <= lhs;
}

Lit operator<(value_type lhs, const Ordinal& rhs) {
  return rhs > lhs;
}

Lit operator<=(value_type lhs, const Ordinal& rhs) {
  return rhs >= lhs;
}

Requirement Ordinal::operator>(const Ordinal& rhs) const {
  return rhs < *this;
}

Requirement Ordinal::operator>=(const Ordinal& rhs) const {
  return rhs <= *this;
}

Requirement Ordinal::operator<(const Ordinal& rhs) const {
  return *this+1 <= rhs;
}

Requirement Ordinal::operator<=(const Ordinal& rhs) const {
  const Ordinal& lhs = *this;
  if ( rhs.max <= lhs.min ) {
    // Here we have lhs.min <= lhs <= rhs < rhs.max <= lhs.min,
    // a contradiction
    return Clause(); // FALSE.
  }
  
  Requirement result;
  
  // lhs.min <= lhs <= rhs
  if ( lhs.min > rhs.min ) {
    result &= lhs.min <= rhs;
  }

  // If rhs <= i, then lhs <= rhs <= i.  Do this for all sensical values of i,
  // that is, from max(lhs.min, rhs.min) to min(lhs.max, rhs.max)-1.
  for ( int i = ::max(lhs.min, rhs.min); i < ::min(lhs.max, rhs.max)-1; i++ ) {
    result &= implication(rhs <= i, lhs <= i);
  }

  // lhs <= rhs < rhs.max
  if ( lhs.max > rhs.max ) {
    result &= lhs < rhs.max;
  }

  return result;
}

// Requirements that two Numbers be equal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each ordinal to be the same, or even
// overlap.
Requirement Ordinal::operator==(const Ordinal& rhs) const {
  return *this <= rhs & *this >= rhs;
}

// Requirements that two Numbers be nonequal, whatever values they take.  Requires that both Numbers
// have the same manager.  Does not require the range for each ordinal to be the same, or even
// overlap.
Requirement Ordinal::operator!=(const Ordinal& rhs) const {
  const Ordinal& lhs = *this;

  Requirement result;

  for ( int i = ::max(lhs.min, rhs.min); i < ::min(lhs.max, rhs.max); i++ ) {
    result &= (lhs != i | rhs != i);
  }

  return result;
}

DualClause operator==(value_type lhs, const Ordinal& rhs) {
  return rhs == lhs;
}

Clause operator!=(value_type lhs, const Ordinal& rhs) {
  return rhs != lhs;
}

// The value assigned in the model, after solving, if a solution is available.
value_type Ordinal::modelValue() const {
  for ( int i = 0; i < max-1-min; i++ ) {
    if ( manager.modelValue(Var(startingVar + i) ) == true ) {
      return min + i;
    }
  }
  return max-1;
}

// After a solution has been found, a requirement for a different solution
Clause Ordinal::diffSolnReq() const {
  return (*this) != this->modelValue();
}

DualClause Ordinal::currSolnReq() const {
  return (*this) == this->modelValue();
}

Ordinal::operator value_type() const {
  return modelValue();
}
