// -*- Mode:C++ -*-
//
// Represents an unknown scalar between "min" and "max" (inclusive of min,
// exclusive of max). Thus, if min is 0, and max is 3, then the scalar can
// take the values 0,1,2.
//
// When the Scalar is created, it allocates enough literals
//  to indicate (however it likes) the value the scalar takes.


#ifndef SCALAR_H
#define SCALAR_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"

class Scalar {
public:
  typedef signed int value_type;

  // Builds a scalar object with the corresponding parameters.
  // With the default startingVar, registers requirements and allocates a new object using the next available literals.
  // With a non-default startingVar, acts as a reference-like object for a already created object with the given
  //  startingvar, does not register new requirements, and increments allocateNew by the number of literals used.
  Scalar(SolverManager& manager, value_type min, value_type max, Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  Scalar(const Scalar& copy);

  // After a solution has been found, a requirement for the current/a different solution
  Minisat::Lit diffSolnReq() const;
  Minisat::Lit currSolnReq() const;

  // Addition of a scalar by a constant.  Surprisingly easy to implement, and useful.
  // If scl is a Scalar, then scl+1 returns a scalar that is equal to n+1 iff scl is equal to n.
  // Uses no additional literals or requirements.
  Scalar operator+(const value_type rhs) const;
  Scalar operator-(const value_type rhs) const;

  // Simple literals indicating equality with a specific scalar rhs.  If rhs is out of bounds,
  // behavior is undefined.
  Minisat::Lit operator==(value_type rhs) const;
  Minisat::Lit operator!=(value_type rhs) const;

  // Ordering requirements
  Clause operator>(value_type rhs) const;
  Clause operator>=(value_type rhs) const;
  Clause operator<(value_type rhs) const;
  Clause operator<=(value_type rhs) const;

  // Requirements that two Scalars be equal, whatever values they take.  Requires that both Scalars
  // have the same manager.  Does not require the range for each scalar to be the same, or even
  // overlap.
  Requirement operator==(const Scalar& rhs) const;
  Requirement operator!=(const Scalar& rhs) const;

  // Requirements that two scalars take a particular order
  Requirement operator>(const Scalar& rhs) const;
  Requirement operator>=(const Scalar& rhs) const;
  Requirement operator<(const Scalar& rhs) const;
  Requirement operator<=(const Scalar& rhs) const;

  // The corresponding requirement of being a scalar
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this scalar.
  // Equal to max-min.
  // TODO make this a constant in all solver objects.
  unsigned int getNumLiterals() const;

  // The value assigned in the model, after solving, if a solution is available.
  value_type modelValue() const;
  operator value_type() const;

  const value_type min;
  const value_type max;
  SolverManager& manager;
  const Minisat::Var startingVar;

private:

  void checkDomain(value_type arg) const;
};

// Scalar arithmetic (by constants)
Scalar operator+(const Scalar::value_type rhs, const Scalar& lhs);

// Ordering requirements
Clause operator>(Scalar::value_type lhs, const Scalar& rhs);
Clause operator>=(Scalar::value_type lhs, const Scalar& rhs);
Clause operator<(Scalar::value_type lhs, const Scalar& rhs);
Clause operator<=(Scalar::value_type lhs, const Scalar& rhs);

// Comparison operators
Minisat::Lit operator==(Scalar::value_type lhs, const Scalar& rhs);
Minisat::Lit operator!=(Scalar::value_type lhs, const Scalar& rhs);

#endif // SCALAR_H
