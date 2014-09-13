// -*- Mode:C++ -*-
//
// The De Morgan dual of a clause, i.e., a conjunction of literals.
//
// Designed to play well with Clause, Requirement, Literal, etc.

#ifndef DUALCLAUSE_H
#define DUALCLAUSE_H

#include <list>
#include "clause.h"

class DualClause : public std::list<Literal> {
public:
  // Default constructor
  DualClause();

  // Copy constructor
  DualClause(const DualClause& other);

  // Move constructor
  DualClause(DualClause&& other);

  // Create a clause from a single element
  DualClause(Literal lit);

  // Concatenate and assign
  DualClause& operator&=(DualClause rhs);

  // Concatenate and assign
  DualClause& operator&=(Literal rhs);

  // Assignment operator
  DualClause& operator=(DualClause rhs);

  friend DualClause operator~(Clause dual);

private:
  // Fast way of manufacturing a dualClause from a Clause
  DualClause(std::list<Literal>&& other);
};

// Negate a clause to get a dual clause.
DualClause operator~(Clause dual);

// Negate a dual clause to get a clause.
Clause operator~(DualClause dual);

// Operator & for conjoining two literals or dualclauses
DualClause operator&(Literal lhs, Literal rhs);
DualClause operator&(Literal lhs, DualClause rhs);
DualClause operator&(DualClause lhs, Literal rhs);
DualClause operator&(DualClause lhs, DualClause rhs);

// Clauses are equal if they have the same elements
// N log N + M log M complexity, where N is the size of lhs and M is the size of rhs.
bool operator==(DualClause rhs, DualClause lhs);
bool operator!=(DualClause rhs, DualClause lhs);
bool operator<(DualClause rhs, DualClause lhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, DualClause rhs);

#endif // DUALCLAUSE_H
