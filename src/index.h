// -*- Mode:C++ -*-
//
// Represents an unknown number between "min" and "max" (inclusive of min,
// exclusive of max). Thus, if min is 0, and max is 3, then the index can
// take the values 0,1,2.
//
// When the Index is created, it allocates enough literals
//  to indicate (however it likes) the value the index takes.


#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"

class Index {
public:
  typedef signed int value_type;

  // Builds a index object with the corresponding parameters.
  // With the default startingVar, registers requirements and allocates a new object using the next available literals.
  // With a non-default startingVar, acts as a reference-like object for a already created object with the given
  //  startingvar, does not register new requirements, and increments allocateNew by the number of literals used.
  Index(SolverManager& manager, value_type min, value_type max, Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  Index(const Index& copy);

  // After a solution has been found, a requirement for the current/a different solution
  Minisat::Lit diffSolnReq() const;
  Minisat::Lit currSolnReq() const;

  // Addition of a index by a constant.  Surprisingly easy to implement, and useful.
  // If idx is a Index, then idx+1 returns a index that is equal to n+1 iff idx is equal to n.
  // Uses no additional literals or requirements.
  Index operator+(const value_type rhs) const;
  Index operator-(const value_type rhs) const;

  // Simple literals indicating equality with a specific index rhs.  If rhs is out of bounds,
  // behavior is undefined.
  Minisat::Lit operator==(value_type rhs) const;
  Minisat::Lit operator!=(value_type rhs) const;

  // Ordering requirements
  Clause operator>(value_type rhs) const;
  Clause operator>=(value_type rhs) const;
  Clause operator<(value_type rhs) const;
  Clause operator<=(value_type rhs) const;

  // Requirements that two Indices be equal, whatever values they take.  Requires that both Indices
  // have the same manager.  Does not require the range for each index to be the same, or even
  // overlap.
  Requirement operator==(const Index& rhs) const;
  Requirement operator!=(const Index& rhs) const;

  // Requirements that two indices take a particular order
  Requirement operator>(const Index& rhs) const;
  Requirement operator>=(const Index& rhs) const;
  Requirement operator<(const Index& rhs) const;
  Requirement operator<=(const Index& rhs) const;

  // The corresponding requirement of being a index
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this index.
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

// Arithmetic (by constants)
Index operator+(const Index::value_type rhs, const Index& lhs);

// Ordering requirements
Clause operator>(Index::value_type lhs, const Index& rhs);
Clause operator>=(Index::value_type lhs, const Index& rhs);
Clause operator<(Index::value_type lhs, const Index& rhs);
Clause operator<=(Index::value_type lhs, const Index& rhs);

// Comparison operators
Minisat::Lit operator==(Index::value_type lhs, const Index& rhs);
Minisat::Lit operator!=(Index::value_type lhs, const Index& rhs);

#endif // INDEX_H
