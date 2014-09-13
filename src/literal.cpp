// -*- Mode:C++ -*-
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
