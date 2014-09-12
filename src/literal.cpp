// -*- Mode:C++ -*-
//
// Implementation of the Literal class

#include <iostream>
#include <stdexcept>
#include "literal.h"

using namespace std;

Literal::Literal(unsigned int var) :
  m_lit(var)
{
}

Literal Literal::operator~() const {
  if ( m_lit == 0 ) {
    throw std::logic_error("The zero literal is invalid and cannot be used.");
  }
  Literal retVal(*this);
  retVal.m_lit = -retVal.m_lit;
  return retVal;
}

bool Literal::operator==(Literal other) const {
  return m_lit == other.m_lit;
}

bool Literal::operator!=(Literal other) const {
  return m_lit != other.m_lit;
}

bool Literal::operator<(Literal other) const {
  return m_lit < other.m_lit;
}

ostream& operator<<(std::ostream& out, Literal rhs) {
  if ( rhs.m_lit < 0 ) {
    out << "~" << (~rhs).m_lit;
  } else {
    out << rhs.m_lit;
  }
  return out;
}
