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
// The De Morgan dual of a clause, i.e., a conjunction of literals.
//
// Designed to play well with Clause, Requirement, Literal, etc.

#ifndef DUALCLAUSE_H
#define DUALCLAUSE_H

#include <list>
#include "clause.h"

class DualClause : public std::list<Literal> {
public:
  // Usual constructors
  DualClause();
  DualClause(const DualClause& other) = default;
  DualClause(DualClause&& other) = default;
  DualClause& operator=(const DualClause& other) = default;
  DualClause& operator=(DualClause&& other) = default;

  // Create a clause from a single element
  DualClause(Literal lit);
  DualClause(Atom at);

  // Concatenate and assign
  DualClause& operator&=(Literal rhs);
  DualClause& operator&=(Atom rhs);
  DualClause& operator&=(DualClause rhs);

  friend DualClause operator~(Clause clause);
  friend Clause     operator~(DualClause dual);
  friend bool operator==(DualClause lhs, DualClause rhs);
  friend bool operator<(DualClause lhs, DualClause rhs);

  static const DualClause truth;
  static const DualClause falsity;
private:
  DualClause(bool falsityFlag);

  bool falsityFlag; // True if this dualclause represents "falsity".
};

// Negate a clause to get a dual clause.
DualClause operator~(Clause dual);

// Negate a dual clause to get a clause.
Clause operator~(DualClause dual);

// Operator & for conjoining two literals or dualclauses
DualClause operator&(Literal lhs,    Literal rhs);
DualClause operator&(Literal lhs,    Atom rhs);
DualClause operator&(Literal lhs,    DualClause rhs);
DualClause operator&(Atom lhs,       Literal rhs);
DualClause operator&(Atom lhs,       Atom rhs);
DualClause operator&(Atom lhs,       DualClause rhs);
DualClause operator&(DualClause lhs, Literal rhs);
DualClause operator&(DualClause lhs, Atom rhs);
DualClause operator&(DualClause lhs, DualClause rhs);

// Clauses are equal if they have the same elements
// N log N + M log M complexity, where N is the size of lhs and M is the size of rhs.
bool operator==(DualClause rhs, DualClause lhs);
bool operator!=(DualClause rhs, DualClause lhs);
bool operator<(DualClause rhs, DualClause lhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, DualClause rhs);

#endif // DUALCLAUSE_H
