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
// Implementation of the Clause class

#include <cmath>
#include <iostream>
#include "clause.h"

using namespace std;

const Clause Clause::truth(true); // Use the special private constructor to construct "truth"
const Clause Clause::falsity;   // Falsity is just an empty clause

// Default constructor
Clause::Clause(void) :
  truthFlag(false)
{

}

// Constructor from one literal
Clause::Clause(const Literal lit) :
  truthFlag(false)
{
  *this |= lit;
}

// Constructor from one atom
Clause::Clause(const Atom at) :
  truthFlag(false)
{
  *this |= at;
}

// Construct a truth/falsity clause
Clause::Clause(bool _truthFlag) :
  truthFlag(_truthFlag)
{

}

// Concatenation of two clauses
Clause operator|(Clause lhs, Clause rhs) {
  lhs |= move(rhs);
  return move(lhs);
}

// Concatenation of a clause and a literal
Clause operator|(Literal lhs, Clause rhs) {
  rhs |= lhs;
  return move(rhs);
}

// Concatenation of a clause and a literal
Clause operator|(Clause lhs, Literal rhs) {
  lhs |= rhs;
  return move(lhs);
}

// Concatenation of a clause and an atom
Clause operator|(Atom lhs, Clause rhs) {
  rhs |= lhs;
  return move(rhs);
}

// Concatenation of a clause and an atom
Clause operator|(Clause lhs, Atom rhs) {
  lhs |= rhs;
  return move(lhs);
}


// Concatenation of two literals and/or atoms
Clause operator|(Literal lhs, Literal rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of two literals and/or atoms
Clause operator|(Atom lhs, Literal rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of two literals and/or atoms
Clause operator|(Literal lhs, Atom rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of two literals and/or atoms
Clause operator|(Atom lhs, Atom rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of a clause and a literal
Clause& Clause::operator|=(const Literal rhs) {
  push_back(rhs);
  return *this;
}

// Concatenation of a clause and an atom
Clause& Clause::operator|=(const Atom rhs) {
  // Truth dominates a disjunction
  if ( *this == Clause::truth || rhs == Atom::truth ) {
    return *this = Clause::truth;
  } 
  
  // Falsity preserves a disjunction
  else if ( rhs == Atom::falsity ) {
    return *this;
  } 

  // We're dealing wiht a literal, so treat it like one.
  else {
    return *this |= rhs.getLiteral();
  }
}

// Concatenation of a clause and a clause
Clause& Clause::operator|=(Clause rhs) {
  if ( *this == Clause::truth || rhs == Clause::truth ) { // Truth dominates a disjunction
    return *this = Clause::truth;
  } else {
    splice(end(), rhs);
  }
  return *this;
}

bool operator==(Clause lhs, Clause rhs) {
  if ( rhs.truthFlag || lhs.truthFlag ) {
    return rhs.truthFlag == lhs.truthFlag;
  }
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
  return rhs_ref == lhs_ref;
}

bool operator!=(Clause rhs, Clause lhs)  {
  return !(rhs == lhs);
}

bool operator<(Clause rhs, Clause lhs)  {
  if ( rhs.truthFlag || lhs.truthFlag ) {
    return rhs.truthFlag < lhs.truthFlag;
  }
  rhs.sort();
  rhs.unique();
  lhs.sort();
  lhs.unique();
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
  return rhs_ref < lhs_ref;
}

// Output a clause
ostream& operator<<(ostream& out, Clause rhs) {
  if ( rhs == Clause::truth ) {
    return out << "truth";
  }

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
