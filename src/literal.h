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
// A class making manipulation of literals easier while building a
// problem.  This also in theory makes it easier to use a different
// engine, other than minisat, as the solving engine, by reducing the
// number of places that directly reference engine details.
//
// Optimized for readability and ease of use; speed takes second
// priority since everything gets converted to Minisat vecs before the
// heavy lifting happens.

#ifndef LITERAL_H
#define LITERAL_H

#include <list>
#include <iostream>

class Literal {
public:
  // Assignment and constructors (all default, all trivial)
  Literal() = default;
  Literal(const Literal& other) = default;
  Literal(Literal&& other) = default;
  Literal& operator=(const Literal&rhs ) = default;
  Literal& operator=(Literal&& rhs) = default;

  // Construction from integer
  explicit Literal(unsigned int lit, bool positive = true);

  unsigned int getVar() const;
  bool isPos() const;

  // Only one basic logic operation that doesn't use more complicated objects like clauses or dualclauses.
  Literal operator~() const;

  // Equality (primarily to support std::list objects)
  bool operator==(Literal rhs) const;
  bool operator!=(Literal rhs) const;
  bool operator<(Literal rhs) const;

  // Output a "nice" representation (for debugging, primarily)
  friend std::ostream& operator<<(std::ostream& out, Literal rhs);

private:
  int m_lit;
};

#endif // LITERAL_H
