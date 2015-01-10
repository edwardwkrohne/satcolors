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
// Implementation of the OrdinalMinExpr class

#include <algorithm>
#include <stdexcept>
#include "ordinalminexpr.h"

using namespace std;

OrdinalMinExpr::OrdinalMinExpr(const Ordinal& ord1, const Ordinal& ord2) :
  ord1(ord1),
  ord2(ord2)
{

}

OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2) {
  return OrdinalMinExpr(ord1, ord2);
}

Clause OrdinalMinExpr::operator <= (const int bound) const {
  if ( bound >= ord1.max()-1 || bound >= ord2.max()-1 ) {
    throw domain_error("Requested a clause that would always be true, which is impossible to represent.");
  }

  Clause result;
  if ( bound >= ord1.min() ) {
    result |= ord1 <= bound;
  }

  if ( bound >= ord2.min() ) {
    result |= ord2 <= bound;
  }

  return result;
}

DualClause OrdinalMinExpr::operator >= (const int bound) const {
  return ~(*this < bound);
}

Clause OrdinalMinExpr::operator <  (const int bound) const {
  return *this <= bound-1;
}

DualClause OrdinalMinExpr::operator >  (const int bound) const {
  return *this >= bound+1;
}

Requirement OrdinalMinExpr::operator == (const int equality) const {
  return *this <= equality & *this >= equality;
}

DualClause operator <  (const int bound, const OrdinalMinExpr& expr) {
  return expr > bound;
}

Clause operator >  (const int bound, const OrdinalMinExpr& expr) {
  return expr < bound;
}

DualClause operator <= (const int bound, const OrdinalMinExpr& expr) {
  return expr >= bound;
}

Clause operator >= (const int bound, const OrdinalMinExpr& expr) {
  return expr <= bound;
}

Requirement operator == (const int equality, const OrdinalMinExpr& expr) {
  return expr == equality;
}

Requirement OrdinalMinExpr::operator<=(const Ordinal& ord3) const {
  if (ord3.max() <= std::min(ord1.min(), ord2.min())) {
    return Clause(); // FALSE
  }

  Requirement result;

  // It may be helpful to view the test suite in
  // ordinalminexprtest.cpp to understand this intricate loop.

  // Loop through each legal value x of ord3, except ord3.max-1
  for ( int x = std::max(ord3.min(), std::min(ord2.min(), ord1.min())-1);
	x < ord3.max()-1 && x < ord2.max()-1 && x < ord1.max()-1; 
	x++ ) {
    result &= implication(ord3 <= x, *this <= x);
  }

  // It is always the case that ord3 <= ord3.max-1, and requesting
  // that literal is illegal.  So the conclusion of that implication
  // (for x=ord3.max-1) should just be required.
  if ( ord3.max()-1 < ord2.max()-1 && ord3.max()-1 < ord1.max()-1) {
    result &= *this <= ord3.max()-1;
  }
  
  return result;
}

Requirement OrdinalMinExpr::operator >= (const Ordinal& ord3) const {
  return ord1 >= ord3 & ord2 >= ord3;
}

Requirement OrdinalMinExpr::operator <  (const Ordinal& ord3) const {
  return *this <= ord3 - 1;
}

Requirement OrdinalMinExpr::operator >  (const Ordinal& ord3) const {
  return *this >= ord3 + 1;
}

Requirement OrdinalMinExpr::operator == (const Ordinal& ord3) const {
  return *this <= ord3 & *this >= ord3;
}

Requirement operator <= (const Ordinal& ord3, const OrdinalMinExpr& expr) {
  return expr >= ord3;
}

Requirement operator >= (const Ordinal& ord3, const OrdinalMinExpr& expr) {
  return expr <= ord3;
}

Requirement operator <  (const Ordinal& ord3, const OrdinalMinExpr& expr) {
  return expr > ord3;
}

Requirement operator >  (const Ordinal& ord3, const OrdinalMinExpr& expr) {
  return expr < ord3;
}

Requirement operator == (const Ordinal& ord3, const OrdinalMinExpr& expr) {
  return expr == ord3;
}
