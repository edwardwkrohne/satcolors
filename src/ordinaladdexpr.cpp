// -*- Mode:C++ -*-
//
// Implementation of the OrdinalAddExpr class

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


Requirement OrdinalAddExpr::operator<=(const Ordinal& ord3) {
  // Suppose ord1.min > ord3.max-ord2.min.  Then it is impossible that
  // ord1 + ord2 <= ord3.  Indeed, if so, we would have 
  // ord1.min + ord2.min <= ord1 + ord2 <= ord3 < ord3.max,
  // whence ord1.min <= ord3.max - ord2.min;
  if ( ord1.min > ord3.max-ord2.min) {
    return Clause(); // FALSE
  }

  Requirement result;

  // It may be helpful to view the test suite in ordinaltest.cpp
  // (possibly moved to ordinaladdexprtest.cpp in the future) to
  // understand this intricate loop.

  // Loop through each legal value x of ord1
  for ( int x = std::max(ord1.min, ord3.min-ord2.max);
	x     < std::min(ord1.max, ord3.max-ord2.min+1);
	x++ ) {

    // Here, writing ord1.min <= ord1 is trivial, and illegal (no
    // literal corresponds to this).
    if ( x == ord1.min )      
      result &=                (x + ord2 <= ord3);
    else                        
      result &= ~(x <= ord1) | (x + ord2 <= ord3);
  }
  
  return result;
}

