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
// Like a Cardinal represents an unknown integer quantity.  Unlike a
// Cardinal, is optimized for order comparisons instead of equality
// comparisons, but nonetheless supports the same operations.
//
// In particular, Ordinal::operator< returns a literal,
// Ordinal::operator== returns a dual clause.  In contrast,
// Cardinal::operator< returns a clause, and Cardinal::operator== returns
// a literal.
//
// Like Cardinal, uses a bound of "min" and "max" (inclusive of min,
// exclusive of max).
//
// When the Ordinal is created, it allocates enough literals to
// indicate (however it likes) the value the ordinal takes.

#ifndef ORDINAL_H
#define ORDINAL_H

#include <iostream>
#include "requirement.h"
#include "solver.h"

class Ordinal {
public:
  // Builds a ordinal object with the corresponding parameters.  With
  // the default startingVar, registers requirements and allocates a
  // new object using the next available literals.  With a non-default
  // startingVar, acts as a reference-like object for a already
  // created object with the given startingvar, does not register new
  // requirements, and increments allocateNew by the number of
  // literals used.
  Ordinal(Solver* solver,
	  int min, 
	  int max);

  Ordinal() = delete;
  Ordinal(const Ordinal& copy) = default;
  Ordinal(Ordinal&& move) = default;
  Ordinal& operator=(const Ordinal& copy) = default;
  Ordinal& operator=(Ordinal&& move) = default;

  // After a solution has been found, a requirement for the current/a different solution
  Clause     diffSolnReq() const;
  DualClause currSolnReq() const;

  // Addition of a ordinal by a constant.  Surprisingly easy to
  // implement, and useful.  If ord is a Ordinal, then ord+1 returns a
  // ordinal that is equal to n+1 iff ord is equal to n.  Uses no
  // additional literals or requirements.
  Ordinal operator+(const int rhs) const;
  Ordinal operator-(const int rhs) const;

  // Negation of ordinal
  Ordinal operator-() const;

  // Equality requirements
  DualClause operator==(int rhs) const;
  Clause     operator!=(int rhs) const;

  // Ordering requirements
  Atom operator>(int rhs) const;
  Atom operator>=(int rhs) const;
  Atom operator<(int rhs) const;
  Atom operator<=(int rhs) const;

  // Requirements that two Ordinals be equal, whatever values they
  // take.  Requires that both Ordinals have the same solver.  Does
  // not require the range for each ordinal to be the same, or even
  // overlap.
  Requirement operator==(const Ordinal& rhs) const;
  Requirement operator!=(const Ordinal& rhs) const;

  // Requirements that two ordinals take a particular order
  Requirement operator>(const Ordinal& rhs) const;
  Requirement operator>=(const Ordinal& rhs) const;
  Requirement operator<(const Ordinal& rhs) const;
  Requirement operator<=(const Ordinal& rhs) const;

  // The corresponding requirement of being a ordinal
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this
  // ordinal.  Equal to max()-min()-1.
  unsigned int numLiterals() const;

  // The minimum and maximum allowable values
  int min() const;
  int max() const;

  // The value assigned in the model, after solving, if a solution is
  // available.
  int modelValue() const;
  explicit operator int() const;

private:
  Solver* mSolver;
  int mMin;
  int mMax;
  int mStartingVar;

  bool mNegated;

  Ordinal(Solver* solver,
	  int min, 
	  int max,
	  const unsigned int startingVar,
	  bool negated);

};

// Ordinal arithmetic (by constants)
Ordinal operator+(const int rhs, const Ordinal& lhs);
Ordinal operator-(const int rhs, const Ordinal& lhs);

// Ordering requirements
Atom operator>(int lhs, const Ordinal& rhs);
Atom operator>=(int lhs, const Ordinal& rhs);
Atom operator<(int lhs, const Ordinal& rhs);
Atom operator<=(int lhs, const Ordinal& rhs);

// Comparison operators
DualClause operator==(int lhs, const Ordinal& rhs);
Clause     operator!=(int lhs, const Ordinal& rhs);

std::ostream& operator<<(std::ostream& out, const Ordinal& rhs);

#endif // ORDINAL_H
