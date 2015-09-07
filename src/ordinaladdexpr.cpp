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
// Implementation of the OrdinalAddExpr class
//
// Implementation may be greatly simplified by using the Atom class, which I
//  just wrote.  The code, as it stands, assumes everything is Literals, which
//  are less powerful.  This results in extreme extra intricacy. -- EK 9/15
#include <algorithm>
#include "ordinaladdexpr.h"

OrdinalAddExpr::OrdinalAddExpr(const Ordinal& ord1, const Ordinal& ord2) :
  ord1(ord1),
  ord2(ord2)
{

}

OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2) {
  return OrdinalAddExpr(ord1, ord2);
}

OrdinalAddExpr operator-(const Ordinal& ord1, const Ordinal& ord2) {
  return ord1 + (-ord2);
}

Requirement OrdinalAddExpr::operator<=(const Ordinal& ord3) const {
  // Suppose ord1.min > ord3.max-ord2.min.  Then it is impossible that
  // ord1 + ord2 <= ord3.  Indeed, if so, we would have 
  // ord1.min + ord2.min <= ord1 + ord2 <= ord3 < ord3.max,
  // whence ord1.min <= ord3.max - ord2.min;
  if ( ord1.min() > ord3.max()-ord2.min()) {
    return Clause(); // FALSE
  }

  Requirement result;

  // It may be helpful to view the test suite in ordinaltest.cpp
  // (possibly moved to ordinaladdexprtest.cpp in the future) to
  // understand this intricate loop.

  // Loop through each legal value x of ord1
  for ( int x = std::max(ord1.min(), ord3.min()-ord2.max());
	x     < std::min(ord1.max(), ord3.max()-ord2.min()+1);
	x++ ) {

    // Here, writing ord1.min <= ord1 is trivial, and illegal (no
    // literal corresponds to this).
    if ( x == ord1.min() )      
      result &=                (x + ord2 <= ord3);
    else                        
      result &= ~(x <= ord1) | (x + ord2 <= ord3);
  }
  
  return result;
}

Requirement OrdinalAddExpr::operator >= (const Ordinal& ord3) const {
  return -ord1 + (-ord2) <= -ord3;
}

Requirement OrdinalAddExpr::operator <  (const Ordinal& ord3) const {
  return *this <= ord3 - 1;
}

Requirement OrdinalAddExpr::operator >  (const Ordinal& ord3) const {
  return *this >= ord3 + 1;
}

Requirement OrdinalAddExpr::operator == (const Ordinal& ord3) const {
  return *this <= ord3 & *this >= ord3;
}

Requirement operator <= (const Ordinal& ord3, const OrdinalAddExpr& expr) {
  return expr >= ord3;
}

Requirement operator >= (const Ordinal& ord3, const OrdinalAddExpr& expr) {
  return expr <= ord3;
}

Requirement operator <  (const Ordinal& ord3, const OrdinalAddExpr& expr) {
  return expr > ord3;
}

Requirement operator >  (const Ordinal& ord3, const OrdinalAddExpr& expr) {
  return expr < ord3;
}

Requirement operator == (const Ordinal& ord3, const OrdinalAddExpr& expr) {
  return expr == ord3;
}

Requirement OrdinalAddExpr::operator <= (const int bound) const {
  return ord1 <= bound - ord2;
}

Requirement OrdinalAddExpr::operator >= (const int bound) const {
  return ord1 >= bound - ord2;
}

Requirement OrdinalAddExpr::operator <  (const int bound) const {
  return *this <= bound-1;
}

Requirement OrdinalAddExpr::operator >  (const int bound) const {
  return *this >= bound+1;
}

Requirement OrdinalAddExpr::operator == (const int equality) const {
  return *this <= equality & *this >= equality;
}

Requirement operator <  (const int bound, const OrdinalAddExpr& expr) {
  return expr > bound;
}

Requirement operator >  (const int bound, const OrdinalAddExpr& expr) {
  return expr < bound;
}

Requirement operator <= (const int bound, const OrdinalAddExpr& expr) {
  return expr >= bound;
}

Requirement operator >= (const int bound, const OrdinalAddExpr& expr) {
  return expr <= bound;
}

Requirement operator == (const int equality, const OrdinalAddExpr& expr) {
  return expr == equality;
}

