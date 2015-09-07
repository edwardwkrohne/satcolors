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
// A slightly more powerful version of "Literal".  Atoms, or atomic formulas,
// consist of one of the following:
// * A literal
// * TRUE
// * FALSE
//
// That is, they are requirements that may not have any instances of "and" or "or".
//
// This affords  flexibility to functions that would return a literal, but need
//  TRUE or FALSE as well for situations such as arguments out of bounds.

#ifndef ATOM_H
#define ATOM_H

#include <list>
#include <iostream>

#include "literal.h"

class Atom {
public:
  // Assignment and constructors (all default, all trivial)
  Atom() = default;
  Atom(const Atom& other) = default;
  Atom(Atom&& other) = default;
  Atom& operator=(const Atom&rhs ) = default;
  Atom& operator=(Atom&& rhs) = default;

  // Construction from integer
  explicit Atom(Literal lit);
  Atom& operator= (Literal lit);

  bool isTruth() const;
  bool isFalsity() const;
  bool isLiteral() const;
  Literal getLiteral() const;

  // Only one basic logic operation that doesn't use more complicated objects like clauses or dualclauses.
  Atom operator~() const;

  // Equality (primarily to support std::list objects)
  bool operator==(Atom rhs) const;
  bool operator!=(Atom rhs) const;
  bool operator<(Atom rhs) const;

  // Output a "nice" representation (for debugging, primarily)
  friend std::ostream& operator<<(std::ostream& out, Atom rhs);

  static const Atom truth;
  static const Atom falsity;
private:
  enum EAtomType {
    falsityType,
    truthType,
    literalType,
  } atomType;

  Atom(EAtomType at);

  Literal lit;
};

#endif // ATOM_H
