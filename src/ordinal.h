// -*- Mode:C++ -*-
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
  Ordinal(SolverManager* manager,
	  value_type min, 
	  value_type max, 
	  Minisat::Var& startingVar = SolverManager::allocateNew);

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
  Ordinal operator+(const value_type rhs) const;
  Ordinal operator-(const value_type rhs) const;

  // Negation of ordinal
  Ordinal operator-() const;

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
  unsigned int numLiterals() const;

  // The minimum and maximum allowable values
  value_type min() const;
  value_type max() const;

  // The value assigned in the model, after solving, if a solution is
  // available.
  value_type modelValue() const;
  explicit operator value_type() const;

private:
  SolverManager* mManager;
  value_type mMin;
  value_type mMax;
  Minisat::Var mStartingVar;

  bool mNegated;

  Ordinal(SolverManager* manager,
	  value_type min, 
	  value_type max,
	  const Minisat::Var startingVar,
	  bool negated);

};

// Ordinal arithmetic (by constants)
Ordinal operator+(const Ordinal::value_type rhs, const Ordinal& lhs);
Ordinal operator-(const Ordinal::value_type rhs, const Ordinal& lhs);

// Ordering requirements
Minisat::Lit operator>(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator>=(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator<(Ordinal::value_type lhs, const Ordinal& rhs);
Minisat::Lit operator<=(Ordinal::value_type lhs, const Ordinal& rhs);

// Comparison operators
DualClause operator==(Ordinal::value_type lhs, const Ordinal& rhs);
Clause     operator!=(Ordinal::value_type lhs, const Ordinal& rhs);

#endif // ORDINAL_H
