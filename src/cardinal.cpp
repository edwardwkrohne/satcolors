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
// Implementation of the Cardinal class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "cardinal.h"

using namespace std;

// Creates an object representing a cardinal.
Cardinal::Cardinal(Solver* _solver, int _min, int _max) :
  mSolver(_solver),
  mMin(_min),
  mMax(_max),
  inverted(false),
  mStartingVar(_solver->newVars(numLiterals()))
{
  mSolver->require(typeRequirement());
}

// The corresponding requirement of being a cardinal --
// must take a value between min and max, and cannot take two values simultaneously.
Requirement Cardinal::typeRequirement() const {
  Requirement result;

  // Build the clause requiring at least one value
  Clause atLeastOneValue;
  for ( int value = min(); value < max(); value++ ) {
    atLeastOneValue |= *this == value;
  }
  result &= move(atLeastOneValue);

  // Build clauses requiring not more than one value
  for ( int value1 = min(); value1 < max(); value1++ ) {
    for ( int value2 = value1+1; value2 < max(); value2++ ) {
      result &= *this != value1 | *this != value2;
    }
  }

  return result;
}

// The number of (contiguous) literals required to represent this cardinal.
// Equal to max-min.
unsigned int Cardinal::numLiterals() const {
  return max()-min();
}

int Cardinal::max() const {
  return mMax;
}

int Cardinal::min() const {
  return mMin;
}

unsigned int Cardinal::startingVar() const {
  return mStartingVar;
}

void Cardinal::checkDomain(const int arg) const {
  if ( arg < min() || arg >= max() ) {
    ostringstream sout;
    sout << "Incorrect value comparison for Cardinal.  This cardinal min=" << min() << " and max=" << max() << " but " << arg << " requested.";
    throw domain_error(sout.str());
  }
}

// Negation. If idx is a Cardinal, then -idx returns a cardinal that is
// equal to n iff idx is equal to -n.
Cardinal Cardinal::operator-() const {
  Cardinal retVal(*this);
  retVal.inverted = !retVal.inverted;
  std::swap(retVal.mMin, retVal.mMax);
  retVal.mMin = -retVal.mMin + 1;
  retVal.mMax = -retVal.mMax + 1;

  return retVal; 
}

Cardinal operator+(const int rhs, const Cardinal& lhs) {
  return lhs + rhs;
}

Cardinal operator-(const int rhs, const Cardinal& lhs) {
  return (-lhs) + rhs;
}

// Addition of a cardinal by a constant.  Surprisingly easy to implement, and useful.
// If idx is a Cardinal, then idx+1 returns a cardinal that is equal to n+1 iff idx is equal to n.
// Uses no additional literals or requirements.
Cardinal Cardinal::operator+(const int rhs) const {
  Cardinal retVal(*this);
  retVal.mMin += rhs;
  retVal.mMax += rhs;
  return retVal;
}
Cardinal Cardinal::operator-(const int rhs) const {
  return *this + (-rhs);
}

// Simple literals indicating equality with a specific cardinal rhs.  If rhs is out of bounds,
// behavior is undefined.
Literal Cardinal::operator==(int rhs) const {
  checkDomain(rhs);
  if ( !inverted ) {
    return Literal( rhs - min() + mStartingVar);
  } else {
    return Literal( max()-1-rhs + mStartingVar);
  }
}
Literal Cardinal::operator!=(int rhs) const {
  checkDomain(rhs);
  return ~(*this == rhs);
}

// Comparison operators
Clause Cardinal::operator>(int rhs) const {
  return *this >= rhs+1;
}

Clause Cardinal::operator>=(int rhs) const {
  Clause result;
  int start = rhs < min() ? min() : rhs;
  for ( int i = start; i < max(); i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Cardinal::operator<(int rhs) const {
  Clause result;
  int end = rhs < max() ? rhs : max();
  for ( int i = min(); i < end; i++ ) {
    result |= *this == i;
  }
  return result;
}

Clause Cardinal::operator<=(int rhs) const {
  return *this < rhs+1;
}

Clause operator>(int lhs, const Cardinal& rhs) {
  return rhs < lhs;
}

Clause operator>=(int lhs, const Cardinal& rhs) {
  return rhs <= lhs;
}

Clause operator<(int lhs, const Cardinal& rhs) {
  return rhs > lhs;
}

Clause operator<=(int lhs, const Cardinal& rhs) {
  return rhs >= lhs;
}


// Requirements that two Cardinals be equal, whatever values they take.  Requires that both Cardinals
// have the same solver.  Does not require the range for each cardinal to be the same, or even
// overlap.
Requirement Cardinal::operator==(const Cardinal& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Cardinal& lhs = *this;

  Requirement result;

  result &= lhs >= rhs.min();
  result &= lhs < rhs.max();
  result &= rhs >= lhs.min();
  result &= rhs < lhs.max();

  int start = lhs.min() < rhs.min() ? rhs.min() : lhs.min();
  int end   = lhs.max() > rhs.max() ? rhs.max() : lhs.max();

  // Run through all possible values that either cardinal can take.
  for ( int val = start; val < end; val++ ) {
    result &= lhs != val | rhs == val;
    result &= lhs == val | rhs != val;
  }

  return result;
}

// Requirements that two Cardinals be nonequal, whatever values they take.  Requires that both Cardinals
// have the same solver.  Does not require the range for each cardinal to be the same, or even
// overlap.
Requirement Cardinal::operator!=(const Cardinal& rhs) const {
  // Treat lhs and rhs symmetrically.
  const Cardinal& lhs = *this;

  Requirement result;

  int maxMin = lhs.min() < rhs.min() ? rhs.min() : lhs.min();
  int minMax = lhs.max() > rhs.max() ? rhs.max() : lhs.max();

  // Run through all values that BOTH cardinals can take
  for ( int val = maxMin; val < minMax; val++ ) {
    result &= lhs != val | rhs != val;
  }

  return result;
}
// The value assigned in the model, after solving, if a solution is available.
int Cardinal::modelValue() const {
  for (int value = 0; value < max()-min(); value++ ) {
    if ( mSolver->modelValue(value + mStartingVar ) == true ) {
      return value+min();
    }
  }

  // Should never reach here.  Results undefined.
  return -1;
}

// After a solution has been found, a requirement for a different solution
Literal Cardinal::diffSolnReq() const {
  return (*this) != this->modelValue();
}
Literal Cardinal::currSolnReq() const {
  return (*this) == this->modelValue();
}

Cardinal::operator int() const {
  return modelValue();
}

Literal operator==(int lhs, const Cardinal& rhs) {
  return rhs == lhs;
}

Literal operator!=(int lhs, const Cardinal& rhs) {
  return rhs == lhs;
}

ostream& operator<<(ostream& out, const Cardinal& rhs) {
  out << (int)rhs;
}
