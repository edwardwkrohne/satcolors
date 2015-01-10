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
// Implementation of the Literal class

#include <iostream>
#include <stdexcept>
#include "literal.h"

using namespace std;

Literal::Literal(unsigned lit, bool pos) :
  m_lit((signed)lit)
{
  *this = pos ? *this : ~*this;
}

unsigned int Literal::getVar() const {
  return (unsigned) (isPos() ? m_lit : ~m_lit );
}

bool Literal::isPos() const {
  return m_lit >= 0;
}

Literal Literal::operator~() const {
  Literal retVal;
  retVal.m_lit = ~m_lit; // (-1)-retVal.m_lit  
  return retVal;
}

bool Literal::operator==(Literal other) const {
  return m_lit == other.m_lit;
}

bool Literal::operator!=(Literal other) const {
  return m_lit != other.m_lit;
}

bool Literal::operator<(Literal other) const {
  // The exact order in question is tested by the unit tests, because
  // the order is used by Clause's cout << function.
  if ( getVar() != other.getVar() ) {
    return getVar() < other.getVar();
  }

  return isPos() < other.isPos();
}

ostream& operator<<(std::ostream& out, Literal rhs) {
  if ( rhs.m_lit < 0 ) {
    out << "~" << (~rhs).m_lit;
  } else {
    out << rhs.m_lit;
  }
  return out;
}
