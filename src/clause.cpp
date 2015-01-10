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

// Default constructor
Clause::Clause() {

}

// Constructor from one literal
Clause::Clause(const Literal lit) {
  push_back(lit);
}

// Copy onstructor
Clause::Clause(const Clause& other) :
  list<Literal>(other)
{
}

// Move constructor
Clause::Clause(Clause&& other) :
  list<Literal>(move(other))
{
}

// Constructor from another list of literals (like a dualClause)
Clause::Clause(std::list<Literal>&& other) :
  list<Literal>(move(other))
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

// Concatenation of two literals
Clause operator|(Literal lhs, Literal rhs) {
  Clause clause(rhs);
  clause |= move(lhs);
  return move(clause);
}

// Concatenation of a clause and a literal
Clause& Clause::operator|=(const Literal rhs) {
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
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
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
  list<Literal>& rhs_ref = rhs;
  list<Literal>& lhs_ref = lhs;
  return rhs_ref < lhs_ref;
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
