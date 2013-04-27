// -*- Mode:C++ -*-
//
// Like a scalar (to be renamed "index" in future versions),
// represents an unknown integer quantity.  Unlike a scalar, is
// optimized for order comparisons instead of equality comparisons,
// but nonetheless supports the same operations.
//
// In particular, Ordinal::operator< returns a literal,
// Ordinal::operator== returns a dual clause.  In contrast,
// Scalar::operator< returns a clause, and Scalar::operator== returns
// a literal.
//
// Like scalar, uses a bound of "min" and "max" (inclusive of min,
// exclusive of max).
//
// When the Ordinal is created, it allocates enough literals to
// indicate (however it likes) the value the ordinal takes.

#ifndef ORDINAL_H
#define ORDINAL_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"

class Ordinal {
public:
  typedef signed int value_type;

  // Builds a ordinal object with the corresponding parameters.  With
  // the default startingVar, registers requirements and allocates a
  // new object using the next available literals.  With a non-default
  // startingVar, acts as a reference-like object for a already
  // created object with the given startingvar, does not register new
  // requirements, and increments allocateNew by the number of
  // literals used.
  Ordinal(SolverManager& manager,
	  value_type min, 
	  value_type max, 
	  Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  Ordinal(const Ordinal& copy);

  // After a solution has been found, a requirement for the current/a different solution
  Clause     diffSolnReq() const;
  DualClause currSolnReq() const;

  // Addition of a ordinal by a constant.  Surprisingly easy to
  // implement, and useful.  If ord is a Ordinal, then ord+1 returns a
  // ordinal that is equal to n+1 iff ord is equal to n.  Uses no
  // additional literals or requirements.
  Ordinal operator+(const value_type rhs) const;
  Ordinal operator-(const value_type rhs) const;

  // Equality requirements
  DualClause operator==(value_type rhs) const;
  Clause     operator!=(value_type rhs) const;

  // Ordering requirements
  Minisat::Lit operator>(value_type rhs) const;
  Minisat::Lit operator>=(value_type rhs) const;
  Minisat::Lit operator<(value_type rhs) const;
  Minisat::Lit operator<=(value_type rhs) const;

  // Requirements that two Ordinals be equal, whatever values they
  // take.  Requires that both Ordinals have the same manager.  Does
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
  // ordinal.  Equal to max-min.
  // TODO make this a constant in all solver objects.
  unsigned int getNumLiterals() const;

  // The value assigned in the model, after solving, if a solution is
  // available.
  value_type modelValue() const;
  operator value_type() const;

  const value_type min;
  const value_type max;
  SolverManager& manager;
  const Minisat::Var startingVar;
};

// Ordinal arithmetic (by constants)
Minisat::Lit operator+(const Ordinal::value_type rhs, const Ordinal& lhs);

// Ordering requirements
Minisat::Lit operator>(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator>=(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator<(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator<=(Ordinal::value_type lhs, const Ordinal& rhs);

// Comparison operators
DualClause operator==(Ordinal::value_type lhs, const Ordinal& rhs);
Clause     operator!=(Ordinal::value_type lhs, const Ordinal& rhs);

#endif // ORDINAL_H
