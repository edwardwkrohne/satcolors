// -*- Mode:C++ -*-
//
// A class making manipulation of clauses and literals easier while
// building a problem.
//
// Optimized for readability and ease of use; speed is not a concern
// since everything gets converted to Minisat vecs before the heavy
// lifting happens.
//
// I use "|" to concatenate two Clauses, since each Clause is a
// disjunction.  I also use ">>" to represent implication; i.e., X >>
// Y is the same as ~X | Y. I am not of the school of thought that
// operator overloading is evil.  That said, my use of ">>" for
// implication is certainly nonstandard; I nonetheless think it will
// help make the code readable, especially for two-element clauses.
//
// When I write logic, I usually make implication (>>)
// right-associative.  Sadly, (>>) is left-associative, so I can't
// write X >> Y >> Z to mean (X & Y) >> Z the way I want.

#ifndef CLAUSE_H
#define CLAUSE_H

#include <list>
#include <iostream>
#include <minisat/core/SolverTypes.h>

class DualClause;

class Clause : public std::list<Minisat::Lit> {
public:
  // Default constructor
  Clause();

  // Copy constructor
  Clause(const Clause& other);

  // Move constructor
  Clause(Clause&& other);

  // Create a clause from a single element
  Clause(Minisat::Lit lit);

  // Assignment
  Clause& operator=(Clause rhs);

  // Concatenate and assign
  Clause& operator|=(Clause rhs);

  // Concatenate and assign
  Clause& operator|=(Minisat::Lit rhs);

  friend Clause operator~(DualClause dual);

private:
  // Fast way of manufacturing a clause from a DualClause
  Clause(std::list<Minisat::Lit>&& dual);
};

// Operator | for concatenation (disjunction)
// Concatenate two Clauses to produce a third
Clause operator|(Minisat::Lit rhs, Minisat::Lit lhs);
Clause operator|(Clause rhs, Minisat::Lit lhs);
Clause operator|(Minisat::Lit rhs, Clause lhs);
Clause operator|(Clause rhs, Clause lhs);

// Clauses are equal if they have the same elements
// N log N + M log M complexity, where N is the size of lhs and M is the size of rhs.
bool operator==(Clause rhs, Clause lhs);
bool operator!=(Clause rhs, Clause lhs);
bool operator<(Clause rhs, Clause lhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, Minisat::Lit rhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, Clause rhs);

#endif // CLAUSE_H
