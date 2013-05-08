// -*- Mode:C++ -*-
//
// Implementation of the OrdinalMinExpr class

#include <algorithm>
#include <stdexcept>
#include "ordinalminexpr.h"

using namespace std;
typedef Ordinal::value_type value_type;

OrdinalMinExpr::OrdinalMinExpr(const Ordinal& ord1, const Ordinal& ord2) :
  ord1(ord1),
  ord2(ord2)
{

}

OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2) {
  return OrdinalMinExpr(ord1, ord2);
}

Clause OrdinalMinExpr::operator <= (const value_type bound) const {
  if ( bound >= ord1.max || bound >= ord2.max ) {
    throw domain_error("Requested a clause that would always be true, which is impossible to represent.");
  }

  Clause result;
  if ( bound >= ord1.min ) {
    result |= ord1 <= bound;
  }

  if ( bound >= ord2.min ) {
    result |= ord2 <= bound;
  }

  return result;
}

DualClause OrdinalMinExpr::operator >= (const value_type bound) const {
  return ~(*this < bound);
}

Clause OrdinalMinExpr::operator <  (const value_type bound) const {
  return *this <= bound-1;
}

DualClause OrdinalMinExpr::operator >  (const value_type bound) const {
  return *this >= bound+1;
}

Requirement OrdinalMinExpr::operator == (const value_type equality) const {
  return *this <= equality & *this >= equality;
}

DualClause operator <  (const value_type bound, const OrdinalMinExpr& expr) {
  return expr > bound;
}

Clause operator >  (const value_type bound, const OrdinalMinExpr& expr) {
  return expr < bound;
}

DualClause operator <= (const value_type bound, const OrdinalMinExpr& expr) {
  return expr >= bound;
}

Clause operator >= (const value_type bound, const OrdinalMinExpr& expr) {
  return expr <= bound;
}

Requirement operator == (const value_type equality, const OrdinalMinExpr& expr) {
  return expr == equality;
}

Requirement OrdinalMinExpr::operator<=(const Ordinal& ord3) const {
  Requirement result;

  // It may be helpful to view the test suite in
  // ordinalminexprtest.cpp to understand this intricate loop.

  // Loop through each legal value x of ord3, except ord3.max-1
  for ( int x = ord3.min; x < ord3.max-1; x++ ) {
    result &= implication(ord3 <= x, *this <= x);
  }

  // It is always the case that ord3 <= ord3.max-1, and requesting
  // that literal is illegal.  So the conclusion of that implication
  // (for x=ord3.max-1) should just be required.
  result &= *this <= ord3.max-1;
  
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
