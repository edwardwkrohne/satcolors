// -*- Mode:C++ -*-
//
// A Requirement is a conjunction of clauses.
//
// Optimized for readability and ease of use; speed is not a concern
// since everything gets converted to Minisat vecs before the heavy
// lifting happens.
//
// I use "&" to conjoin two Requirements, since each Requirement is a
// conjunction.
//
// It is easy to make a Clause into a Requirement, but hard
// and expensive (and not yet implemented) to do the reverse.
//
#ifndef REQUIREMENT_H
#define REQUIREMENT_H

#include <utility>

#include "clause.h"
#include "dualclause.h"

class Requirement : public std::list<Clause> {
public:
  // Default constructor
  Requirement();

  // Copy constructor
  Requirement(const Requirement& other);

  // Move constructor
  Requirement(Requirement&& other);

  // Create a requirement from a single element
  Requirement(Literal lit);
  Requirement(Clause clause);
  Requirement(DualClause clause);

  // Assignment
  Requirement& operator=(Requirement rhs);

  // Conjoin and assign
  Requirement& operator&=(Literal rhs);
  Requirement& operator&=(Clause rhs);
  Requirement& operator&=(DualClause rhs);
  Requirement& operator&=(Requirement rhs);

  // Disjoin and assign
  Requirement& operator|=(Literal rhs);
  Requirement& operator|=(Clause rhs);
  Requirement& operator|=(DualClause rhs);
  Requirement& operator|=(Requirement rhs);
};

// Operator & for conjunction
Requirement operator&(Literal lhs, Clause rhs);
Requirement operator&(Literal lhs, Requirement rhs);
Requirement operator&(Clause lhs,       Literal rhs);
Requirement operator&(Clause lhs,       Clause rhs);
Requirement operator&(Clause lhs,       DualClause rhs);
Requirement operator&(Clause lhs,       Requirement rhs);
Requirement operator&(DualClause lhs,   Clause rhs);
Requirement operator&(DualClause lhs,   Requirement rhs);
Requirement operator&(Requirement lhs,  Literal rhs);
Requirement operator&(Requirement lhs,  Clause rhs);
Requirement operator&(Requirement lhs,  DualClause rhs);
Requirement operator&(Requirement lhs,  Requirement rhs);

// Operator | for disjunction
Requirement operator|(Literal lhs, DualClause rhs);
Requirement operator|(Literal lhs, Requirement rhs);
Requirement operator|(Clause lhs,       DualClause rhs);
Requirement operator|(Clause lhs,       Requirement rhs);
Requirement operator|(DualClause lhs,   Literal rhs);
Requirement operator|(DualClause lhs,   Clause rhs);
Requirement operator|(DualClause lhs,   DualClause rhs);
Requirement operator|(DualClause lhs,   Requirement rhs);
Requirement operator|(Requirement lhs,  Literal rhs);
Requirement operator|(Requirement lhs,  Clause rhs);
Requirement operator|(Requirement lhs,  DualClause rhs);
Requirement operator|(Requirement lhs,  Requirement rhs);

// Clauses are equal if they have the same elements
// N log N + M log M comparisons, where N is the size of lhs and M is the size of rhs.
// The comparisons themselves do not have constant time complexity either.
bool operator==(Requirement rhs, Requirement lhs);
bool operator!=(Requirement rhs, Requirement lhs);
bool operator<(Requirement rhs, Requirement lhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, Requirement rhs);

template<class Lhs, class Rhs>
auto implication(Lhs lhs, Rhs rhs) -> decltype(~lhs | rhs) {
  return ~std::move(lhs) | std::move(rhs);
};

template<class Lhs, class Rhs>
auto equivalence(Lhs lhs, Rhs rhs) -> decltype((~lhs | rhs) & (~rhs | lhs)) {
  auto reverse = (~rhs | lhs);
  auto forward = (~std::move(lhs) | std::move(rhs));
  return move(reverse) & std::move(forward);
};

#endif // REQUIREMENT_H
