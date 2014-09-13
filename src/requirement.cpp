// -*- Mode:C++ -*-
//
// Implementation of the Requirement class

#include <cmath>
#include <iostream>
#include "requirement.h"

using namespace std;

Requirement::Requirement() {

}

Requirement::Requirement(const Requirement& requirement) :
  list<Clause>(requirement)
{
}

Requirement::Requirement(Requirement&& requirement) :
  list<Clause>(move(requirement))
{
}

Requirement::Requirement(DualClause clause) {
  *this &= move(clause);
}

Requirement::Requirement(Clause clause) {
  *this &= move(clause);
}

Requirement::Requirement(Literal lit) {
  *this &= lit;
}

Requirement& Requirement::operator=(Requirement other) {
  swap(other);
  return *this;
}

Requirement& Requirement::operator&=(Requirement rhs) {
  splice(end(),rhs);
  return *this;
}

Requirement& Requirement::operator&=(DualClause rhs) {
  while ( rhs.size() != 0 ) {
    // Carefully preserve (steal) the allocated memory for *rhs.begin().
    Clause unitClause;
    unitClause.splice(unitClause.end(), rhs, rhs.begin());

    // DualClauses are stored with their elements negated.  Fix it.
    *unitClause.begin() = ~*unitClause.begin();

    // Add to the requirement
    *this &= move(unitClause);
  }

  return *this;
}

Requirement& Requirement::operator&=(Clause rhs) {
  push_back(move(rhs));
  return *this;
}

Requirement& Requirement::operator&=(Literal rhs) {
  *this &= (Clause)move(rhs);
  return *this;
}

Requirement operator&(Literal lhs, Clause rhs) {
  Requirement req(move(rhs));
  req &= move(lhs);
  return req;
}

Requirement operator&(Literal lhs, Requirement rhs) {
  rhs &= move(lhs);
  return move(rhs);
}

Requirement operator&(Clause lhs, Literal rhs) {
  Requirement req(move(lhs));
  req &= move(rhs);
  return req;
}

Requirement operator&(Clause lhs, Clause rhs) {
  Requirement req(move(lhs));
  req &= move(rhs);
  return req;
}

Requirement operator&(Clause lhs, DualClause rhs) {
  Requirement req(move(rhs));
  req &= move(lhs);
  return req;
}

Requirement operator&(Clause lhs, Requirement rhs) {
  rhs &= move(lhs);
  return move(rhs);
}

Requirement operator&(DualClause lhs, Clause rhs) {
  Requirement req(move(lhs));
  req &= move(rhs);
  return req;
}

Requirement operator&(DualClause lhs, Requirement rhs) {
  rhs &= move(lhs);
  return move(rhs);
}

Requirement operator&(Requirement lhs, Literal rhs) {
  lhs &= move(rhs);
  return move(lhs);
}

Requirement operator&(Requirement lhs, Clause rhs) {
  lhs &= move(rhs);
  return move(lhs);
}

Requirement operator&(Requirement lhs, DualClause rhs) {
  lhs &= move(rhs);
  return move(lhs);
}

Requirement operator&(Requirement lhs, Requirement rhs) {
  lhs &= move(rhs);
  return move(lhs);
}

Requirement& Requirement::operator|=(Requirement rhs) {
  // Don't even mess with possibly invalid iterators if there's nothing here.
  if ( size() == 0 )
    return *this;

  Requirement result;

  // Hold on to a clause to be treated specially later
  auto iter = begin();
  Clause& savedClause = *iter++;

  // Run through *this and add requirements to result
  while ( iter != end() ) {
    // We can consume the clause as part of *this since we'll never need it again,
    // but we should make copies of rhs.
    result &= move(*iter++) | rhs;
  }

  // Insert a requirement using rhs itself to avoid wasting rhs.
  result &= move(savedClause) | move(rhs);

  // The memory allocated by *this (but not its children) is dealloated here.
  swap(result);
  return *this;
}

Requirement& Requirement::operator|=(DualClause rhs) {
  Requirement req(move(rhs));
  *this |= move(req);
  return *this;
}

Requirement& Requirement::operator|=(Clause rhs) {
  // Don't even mess with possibly invalid iterators if there's nothing here.
  if ( size() == 0 )
    return *this;

  // Get the first clause of *this; we will want to treat it specially in a moment
  auto iter = begin();
  Clause& savedClause = *iter++;

  // Run through the requirement and lengthen all the clauses.
  while ( iter != end() ) {
    // Disjoin with a copy of rhs
    *iter++ |= rhs;
  }

  // Finally disjoin the saved clause with rhs itself to avoid wasting
  // (allocating/deallocating) memory.
  savedClause |= move(rhs);

  return *this;
}

Requirement& Requirement::operator|=(Literal rhs) {
  Clause clause(rhs);
  *this |= move(clause);
  return *this;
}

// Disjoin a requirement with a clause (all disjunction operations are defined in terms of
// this function).  Disjoin each clause in the requirement with rhs.
Requirement operator|(Requirement lhs, Clause rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

// Disjoin two requirements.  This uses distribution, which can result in explosion
// of clauses
Requirement operator|(Requirement lhs, Requirement rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

// Disjoin a requirement and a dual clause
Requirement operator|(Requirement lhs, DualClause rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

Requirement operator|(Requirement lhs, Literal rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

Requirement operator|(DualClause lhs, DualClause rhs) {
  Requirement req(move(lhs));
  req |= move(rhs);
  return req;
}

Requirement operator|(DualClause lhs, Clause rhs) {
  Requirement req(move(lhs));
  req |= move(rhs);
  return req;
}

Requirement operator|(DualClause lhs, Literal rhs) {
  Requirement req(move(lhs));
  req |= move(rhs);
  return req;
}

Requirement operator|(Clause lhs, DualClause rhs) {
  Requirement req(move(rhs));
  req |= move(lhs);
  return req;
}

Requirement operator|(DualClause lhs, Requirement rhs) {
  rhs |= move(lhs);
  return move(rhs);
}

Requirement operator|(Clause lhs, Requirement rhs) {
  rhs |= move(lhs);
  return move(rhs);
}

Requirement operator|(Literal lhs, Requirement rhs) {
  rhs |= move(lhs);
  return move(rhs);
}

Requirement operator|(Literal lhs, DualClause rhs) {
  Requirement req(move(rhs));
  req |= move(lhs);
  return req;
}

bool operator==(Requirement rhs, Requirement lhs) {
  // Normalize output for comparison
  // Grossly inefficient, considering that Clauses themselves sort on
  // EACH compare.
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Clause>& rhs_ref = rhs;
  list<Clause>& lhs_ref = lhs;
  return rhs_ref == lhs_ref;
}

bool operator!=(Requirement rhs, Requirement lhs)  {
  return !(rhs == lhs);
}

bool operator<(Requirement rhs, Requirement lhs)  {
  // Normalize output for comparison
  // Grossly inefficient, considering that Clauses themselves sort on
  // EACH compare.
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Clause>& rhs_ref = rhs;
  list<Clause>& lhs_ref = lhs;
  return rhs_ref < lhs_ref;
}

ostream& operator<<(ostream& out, Requirement rhs) {
  typedef Requirement::iterator iterator;
  // An empty requirement is always "TRUE"
  if ( rhs.empty() ) {
    return out << " TRUE ";
  }

  // Normalize output to make the output easier to read and test
  // Grossly inefficient, considering that Clauses themselves sort on
  // EACH compare. But I don't care about fast output and could
  // do something smarter if I did.
  rhs.sort();
  rhs.unique();

  for ( iterator iter = rhs.begin(); iter != rhs.end(); iter++ ) {
    // Put a separator everywhere but the beginning
    if ( iter != rhs.begin() ) {
      out << " & ";
    }

    // An empty clause is always "FALSE"
    if ( iter->empty() ) {
      out << "FALSE";
      continue;
    }

    // Don't bother with parenthesis for a clause of length 1
    if ( iter->size() == 1 ) {
      out << *iter;
      continue;
    }

    // Write the clause
    out << "(" << *iter << ")";
  }
  return out;
}
