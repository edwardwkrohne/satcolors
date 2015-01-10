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
#include "literal.h"

class DualClause;

class Clause : public std::list<Literal> {
public:
  // Default constructor
  Clause();

  // Copy constructor
  Clause(const Clause& other);

  // Move constructor
  Clause(Clause&& other);

  // Create a clause from a single element
  Clause(Literal lit);

  // Assignment
  Clause& operator=(Clause rhs);

  // Concatenate and assign
  Clause& operator|=(Clause rhs);

  // Concatenate and assign
  Clause& operator|=(Literal rhs);

  friend Clause operator~(DualClause dual);

private:
  // Fast way of manufacturing a clause from a DualClause
  Clause(std::list<Literal>&& dual);
};

// Operator | for concatenation (disjunction)
// Concatenate two Clauses to produce a third
Clause operator|(Literal rhs, Literal lhs);
Clause operator|(Clause rhs, Literal lhs);
Clause operator|(Literal rhs, Clause lhs);
Clause operator|(Clause rhs, Clause lhs);

// Clauses are equal if they have the same elements
// N log N + M log M complexity, where N is the size of lhs and M is the size of rhs.
bool operator==(Clause rhs, Clause lhs);
bool operator!=(Clause rhs, Clause lhs);
bool operator<(Clause rhs, Clause lhs);

// Output a "nice" representation (for debugging, primarily)
std::ostream& operator<<(std::ostream& out, Clause rhs);

#endif // CLAUSE_H
