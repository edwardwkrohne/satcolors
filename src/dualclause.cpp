// -*- Mode:C++ -*-
//
// Implementation of the DualClause class

#include <iostream>
#include "dualclause.h"
#include "clause.h"

using namespace std;

// Default constructor
DualClause::DualClause() {

}

// Constructor from one literal
DualClause::DualClause(const Literal lit) {
  push_back(~lit);
}

// Copy constructor
DualClause::DualClause(const DualClause& other) :
  list<Literal>(other)
{
}

// Move constructor
DualClause::DualClause(DualClause&& other) :
  list<Literal>(move(other))
{
}

// Constructor from a set
DualClause::DualClause(std::list<Literal>&& other) :
    list<Literal>(move(other))
{
}

// Concatenation of a clause and a literal
DualClause& DualClause::operator&=(const Literal rhs) {
  push_back(~rhs);
  return *this;
}

// Concatenation of a clause and a clause
DualClause& DualClause::operator&=(DualClause rhs) {
  splice(end(), rhs);
  return *this;
}

// Concatenation of clauses
DualClause operator&(DualClause lhs, DualClause rhs) {
  lhs &= move(rhs);
  return move(lhs);
}

// Concatenation of a literal and a clause
DualClause operator&(Literal lhs, DualClause rhs) {
  rhs &= lhs;
  return move(rhs);
}

// Concatenation of a clause and a literal
DualClause operator&(DualClause lhs, Literal rhs) {
  lhs &= rhs;
  return move(lhs);
}

// Concatenation of literals
DualClause operator&(Literal lhs, Literal rhs) {
  DualClause dClause(lhs);
  dClause &= rhs;
  return dClause;
}

DualClause& DualClause::operator=(DualClause other) {
  swap(other);
  return *this;
}

Clause operator~(DualClause dual) {
  Clause clause(move(dual));
  return clause;
}

DualClause operator~(Clause clause) {
  DualClause dual(move(clause));
  return dual;
}

bool operator==(DualClause rhs, DualClause lhs) {
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
  return rhs_ref == lhs_ref;
}
bool operator!=(DualClause rhs, DualClause lhs) {
  return ~(rhs == lhs);
}
bool operator<(DualClause rhs, DualClause lhs) {
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
  return rhs_ref == lhs_ref;
}
// Output a clause
ostream& operator<<(ostream& out, DualClause rhs) {
  // Do nothing for an empty list.
  if ( rhs.size() == 0 ) return out;

  // Normalize the list to make it easy to see differences
  rhs.sort();
  rhs.unique();

  // Print all but the last element
  decltype(rhs.begin()) precursor;
  decltype(rhs.begin()) cursor;

  for ( cursor = rhs.begin(), precursor = rhs.begin(), precursor++;
  	cursor != rhs.end() && precursor != rhs.end(); 
  	cursor++, precursor++ ) {
    out << ~*cursor << " & ";
  }

  // Print the last element
  out << ~*cursor;

  return out;
}
