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
// Implementation of the Atom class

#include <iostream>
#include <stdexcept>
#include "atom.h"

using namespace std;

const Atom Atom::truth(Atom::truthType);
const Atom Atom::falsity(Atom::falsityType);

Atom::Atom(Literal _lit) :
  lit(_lit),
  atomType(literalType)
{
}

Atom::Atom(EAtomType at) :
  atomType(at),
  lit(0)
{
  
}

Atom& Atom::operator= (Literal _lit) {
  atomType = literalType;
  lit = _lit;
}

bool Atom::isTruth() const {
  return atomType == truthType;
}

bool Atom::isFalsity() const {
  return atomType == falsityType;
}

bool Atom::isLiteral() const {
  return atomType == literalType;
}

Literal Atom::getLiteral() const {
  if ( !isLiteral() ) {
    throw std::logic_error("Attempt to get literal from a non-literal atom.");
  }

  return lit;
}

Atom Atom::operator~() const {
  switch(atomType) {
  case falsityType:
    return Atom::truth;
  case truthType:
    return Atom::falsity;
  case literalType:
    return Atom(~lit);
  default:
    throw std::logic_error("This code should never be reached.");
  }
}

bool Atom::operator==(Atom other) const {
  return atomType == other.atomType && ( !isLiteral() || lit == other.lit );
}

bool Atom::operator!=(Atom other) const {
  return !(*this == other);
}

bool Atom::operator<(Atom other) const {
  if ( atomType == other.atomType ) return lit < other.lit;

  return atomType < other.atomType;
}

ostream& operator<<(std::ostream& out, Atom rhs) {
  switch(rhs.atomType) {
  case Atom::falsityType:
    out << "falsity";
    break;
  case Atom::truthType:
    out << "truth";
    break;
  case Atom::literalType:
    out << rhs.lit;
    break;
  default:
    throw std::logic_error("This code should never be reached.");
  }

  return out;
}
