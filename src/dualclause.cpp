// -*- Mode: C++ -*-
////////////////////////////////////////////////////////////////////////////
//
//  satcolors -- for writably and readably building problems for SAT-solving
//  Copyright (C) 2014 Edward W. Krohne III
//
//  The research that led to this product was partially supported by the
//  U.S. NSF grants DMS-0943870 and DMS-1201290.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////
//
// Implementation of the DualClause class

#include <iostream>
#include "dualclause.h"
#include "clause.h"

using namespace std;

const DualClause DualClause::truth;
const DualClause DualClause::falsity(true); // Use the special private constructor

// Default constructor
DualClause::DualClause(void) :
  falsityFlag(false)
{

}

// Constructor from one literal
DualClause::DualClause(const Literal lit) :
  falsityFlag(false)
{
  *this &= lit;
}

// Constructor from one atom
DualClause::DualClause(const Atom at) :
  falsityFlag(false)
{
  *this &= at;
}

// Construct a truth/falsity
DualClause::DualClause(bool _falsityFlag) :
  falsityFlag(_falsityFlag)
{
}

// Concatenation of a clause and a literal
DualClause& DualClause::operator&=(const Literal rhs) {
  push_back(~rhs);
  return *this;
}

// Concatenation of a clause and an atom
DualClause& DualClause::operator&=(const Atom rhs) {
  // falsity dominates a conjunction
  if ( *this == DualClause::falsity || rhs == Atom::falsity ) {
    return *this = DualClause::falsity;
  } 

  // truth preserves a conjunction
  else if ( rhs == Atom::truth ) {
    return *this;
  }

  // Atoms that are literals can be handed normally
  else {
    return *this &= rhs.getLiteral();
  }
}

// Concatenation of a clause and a clause
DualClause& DualClause::operator&=(DualClause rhs) {
  // falsity dominates a conjunction
  if ( *this == DualClause::falsity || rhs == DualClause::falsity ) {
    return *this = DualClause::falsity;
  } else {
    splice(end(), rhs);
  }
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

// Concatenation of a clause and a atom
DualClause operator&(DualClause lhs, Literal rhs) {
  lhs &= rhs;
  return move(lhs);
}

DualClause operator&(DualClause lhs, Atom rhs) {
  lhs &= rhs;
  return move(lhs);
}

// Concatenation of literals and/or atoms
DualClause operator&(Literal lhs, Literal rhs) {
  DualClause dClause(lhs);
  dClause &= rhs;
  return dClause;
}

DualClause operator&(Atom lhs, Literal rhs) {
  DualClause dClause(lhs);
  dClause &= rhs;
  return dClause;
}

DualClause operator&(Literal lhs, Atom rhs) {
  DualClause dClause(lhs);
  dClause &= rhs;
  return dClause;
}

DualClause operator&(Atom lhs, Atom rhs) {
  DualClause dClause(lhs);
  dClause &= rhs;
  return dClause;
}

Clause operator~(DualClause dual) {
  Clause clause;
  // Assign and negate the flag
  clause.truthFlag = dual.falsityFlag;

  // Assign the clause to the dual clause as lists.  Dualclauses store
  // their literals negated, so this "just works".
  ((list<Literal>&)clause) = move((list<Literal>&)dual);
  return clause;
}

DualClause operator~(Clause clause) {
  DualClause dual;
  // Assign and negate the flag
  dual.falsityFlag = clause.truthFlag;

  // Assign the clause to the dual clause as lists.  Dualclauses store
  // their literals negated, so this "just works".
  ((list<Literal>&)dual) = move((list<Literal>&)clause);
  return dual;
}

bool operator==(DualClause rhs, DualClause lhs) {
  if ( rhs.falsityFlag || lhs.falsityFlag ) {
    return rhs.falsityFlag == lhs.falsityFlag;
  }
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
  if ( rhs.falsityFlag || lhs.falsityFlag ) {
    return rhs.falsityFlag < lhs.falsityFlag;
  }
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
