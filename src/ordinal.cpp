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
// Implementation of the Ordinal class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ordinal.h"

using namespace std;

// Creates an object representing an ordinal.
Ordinal::Ordinal(Solver* _solver, int _min, int _max) :
  mMin(_min),
  mMax(_max),
  mSolver(_solver),
  mStartingVar(_solver->newVars(numLiterals())),
  mNegated(false)
{
  if ( max() <= min() ) {
    throw domain_error("Cannot create an ordinal with an empty range of possible values.");
  }
  
  mSolver->require(typeRequirement());
}

Ordinal::Ordinal(Solver* _solver, 
		 int _min, 
		 int _max, 
		 const unsigned int _startingVar, 
		 bool _negated) :
  mMin(_min),
  mMax(_max),
  mSolver(_solver),
  mStartingVar(_startingVar),
  mNegated(_negated)
{
}

// The corresponding requirement of being a ordinal --
// must take a value between min and max, and cannot take two values simultaneously.
Requirement Ordinal::typeRequirement() const {
  Requirement result;
  for (int i = min(); i < max()-2; i++ ) {
    result &= implication(*this <= i, *this <= i+1);
  }
  return result;
}

// The number of (contiguous) literals required to represent this ordinal.
// Equal to max-min.
unsigned int Ordinal::numLiterals() const {
  return max()-min()-1;
}

// Min and max
int Ordinal::min() const {
  return mMin;
}

int Ordinal::max() const {
  return mMax;
}

// Addition of a ordinal by a constant.  Surprisingly easy to implement, and useful.
// If scl is a Ordinal, then scl+1 returns a ordinal that is equal to n+1 iff scl is equal to n.
// Uses no additional literals or requirements.
Ordinal Ordinal::operator+(const int rhs) const {
  unsigned int var = mStartingVar;
  return Ordinal(mSolver, min()+rhs, max()+rhs, var, mNegated);
}
Ordinal Ordinal::operator-(const int rhs) const {
  return *this + (-rhs);
}

Ordinal operator+(const int lhs, const Ordinal& rhs) {
  return rhs + lhs;
}
Ordinal operator-(const int lhs, const Ordinal& rhs) {
  return lhs + (-rhs);
}

// Negation of an ordinal.
Ordinal Ordinal::operator-() const {
  return Ordinal(mSolver, -max()+1, -min()+1, mStartingVar, !mNegated);
}

// Simple literals indicating equality with a specific ordinal rhs.  If rhs is out of bounds,
// behavior is undefined.
DualClause Ordinal::operator==(int rhs) const {
  // If out of bounds, just return falsity
  if ( rhs < min() || rhs >= max() ) {
    return DualClause::falsity;
  }

  DualClause result;

  // This kind of fiddling, ensuring that rhs is in range before
  // grabbing specific parts of the clause, is probably no longer
  // necessary now that operator< returns atoms, but no reason not to
  // leave it in.
  if ( rhs > min() )
    result &= *this >= rhs;

  if ( rhs < max()-1 )
    result &= *this <= rhs;

  return result;
}
Clause Ordinal::operator!=(int rhs) const {
  return ~(*this == rhs);
}

// Comparison operators
Atom Ordinal::operator>(int rhs) const {
  return ~(*this <= rhs);
}

Atom Ordinal::operator>=(int rhs) const {
  return ~(*this < rhs);
}

Atom Ordinal::operator<(int rhs) const {
  if ( rhs <= min() ) return Atom::falsity;
  if ( rhs >= max() ) return Atom::truth;
  
  if ( !mNegated ) {
    return  Atom(Literal(rhs-1-min()+mStartingVar));
  } else {
    return Atom(~Literal(max()-rhs-1+mStartingVar));
  }
}

Atom Ordinal::operator<=(int rhs) const {
  return *this < rhs+1;
}

Atom operator>(int lhs, const Ordinal& rhs) {
  return rhs < lhs;
}

Atom operator>=(int lhs, const Ordinal& rhs) {
  return rhs <= lhs;
}

Atom operator<(int lhs, const Ordinal& rhs) {
  return rhs > lhs;
}

Atom operator<=(int lhs, const Ordinal& rhs) {
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
  if ( rhs.max() <= lhs.min() ) {
    // Here we have lhs.min <= lhs <= rhs < rhs.max <= lhs.min,
    // a contradiction
    return Clause(); // FALSE.
  }
  
  Requirement result;
  
  // lhs.min <= lhs <= rhs
  if ( lhs.min() > rhs.min() ) {
    result &= lhs.min() <= rhs;
  }

  // If rhs <= i, then lhs <= rhs <= i.  Do this for all sensical values of i,
  // that is, from max(lhs.min, rhs.min) to min(lhs.max, rhs.max)-1.
  for ( int i = ::max(lhs.min(), rhs.min()); i < ::min(lhs.max(), rhs.max())-1; i++ ) {
    result &= implication(rhs <= i, lhs <= i);
  }

  // lhs <= rhs < rhs.max
  if ( lhs.max() > rhs.max() ) {
    result &= lhs < rhs.max();
  }

  return result;
}

// Requirements that two Numbers be equal, whatever values they take.  Requires that both Numbers
// have the same solver.  Does not require the range for each ordinal to be the same, or even
// overlap.
Requirement Ordinal::operator==(const Ordinal& rhs) const {
  return *this <= rhs & *this >= rhs;
}

// Requirements that two Numbers be nonequal, whatever values they take.  Requires that both Numbers
// have the same solver.  Does not require the range for each ordinal to be the same, or even
// overlap.
Requirement Ordinal::operator!=(const Ordinal& rhs) const {
  const Ordinal& lhs = *this;

  Requirement result;

  for ( int i = ::max(lhs.min(), rhs.min()); i < ::min(lhs.max(), rhs.max()); i++ ) {
    result &= (lhs != i | rhs != i);
  }

  return result;
}

DualClause operator==(int lhs, const Ordinal& rhs) {
  return rhs == lhs;
}

Clause operator!=(int lhs, const Ordinal& rhs) {
  return rhs != lhs;
}

// The value assigned in the model, after solving, if a solution is available.
int Ordinal::modelValue() const {
  if ( !mNegated ) {
    for ( int i = 0; i < max()-min()-1; i++ ) {
      if ( mSolver->modelValue(mStartingVar + i) == true ) {
	return min() + i;
      }
    }
    return (max()-1);
  } 

  else {
    for ( int i = 0; i < max()-min()-1; i++ ) {
      if ( mSolver->modelValue(mStartingVar + i) == true ) {
	return (max()-1) - i;
      }
    }
    return min();
  }

}

// After a solution has been found, a requirement for a different solution
Clause Ordinal::diffSolnReq() const {
  return (*this) != this->modelValue();
}

DualClause Ordinal::currSolnReq() const {
  return (*this) == this->modelValue();
}

Ordinal::operator int() const {
  return modelValue();
}

ostream& operator<<(ostream& out, const Ordinal& rhs) {
  out << (int)rhs;
}
