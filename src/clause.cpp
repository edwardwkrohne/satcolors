// -*- Mode:C++ -*-
//
// Implementation of the Clause class

#include <cmath>
#include <iostream>
#include "clause.h"

using Minisat::Lit;
using namespace std;

// Default constructor
Clause::Clause() {

}

// Constructor from one literal
Clause::Clause(const Lit lit) {
  push_back(lit);
}

// Copy onstructor
Clause::Clause(const Clause& other) :
  list<Lit>(other)
{
}

// Move constructor
Clause::Clause(Clause&& other) :
  list<Lit>(move(other))
{
}

// Constructor from another list of literals (like a dualClause)
Clause::Clause(std::list<Minisat::Lit>&& other) :
  list<Lit>(move(other))
{
}

// Concatenation of two clauses
Clause operator|(Clause lhs, Clause rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

// Concatenation of a clause and a literal
Clause operator|(Minisat::Lit lhs, Clause rhs) {
  rhs |= lhs;
  return move(rhs);
}

// Concatenation of a clause and a literal
Clause operator|(Clause lhs, Minisat::Lit rhs) {
  lhs |= rhs;
  return move(lhs);
}

// Concatenation of two literals
Clause operator|(Minisat::Lit lhs, Minisat::Lit rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of a clause and a literal
Clause& Clause::operator|=(const Lit rhs) {
  push_back(rhs);
  return *this;
}

// Concatenation of a clause and a clause
Clause& Clause::operator|=(Clause rhs) {
  splice(end(), rhs);
  return *this;
}

// Assignment
Clause& Clause::operator=(Clause rhs) {
  swap(rhs);
  return *this;
}

bool operator==(Clause rhs, Clause lhs) {
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Lit>& rhs_ref = rhs;
  list<Lit>& lhs_ref = lhs;
  return rhs_ref == lhs_ref;
}

bool operator!=(Clause rhs, Clause lhs)  {
  return !(rhs == lhs);
}

bool operator<(Clause rhs, Clause lhs)  {
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Lit>& rhs_ref = rhs;
  list<Lit>& lhs_ref = lhs;
  return rhs_ref < lhs_ref;
}

// Output a literal
ostream& operator<<(ostream& out, const Lit lit) {
  return out << (sign(lit) ? "~" : "") << var(lit);
}

// Output a clause
ostream& operator<<(ostream& out, Clause rhs) {
  // Do nothing for an empty list.
  if ( rhs.begin() == rhs.end() ) return out;

  // Normalize output to make it easier to compare
  rhs.sort();
  rhs.unique();

  // Print all but the last element
  decltype(rhs.begin()) precursor, cursor;
  for ( cursor = rhs.begin(), precursor = rhs.begin(), precursor++;
  	cursor != rhs.end() && precursor != rhs.end(); 
  	cursor++, precursor++ ) {
    out << *cursor << " | "; 
  }

  // Print the last element
  return out << *cursor;
}
