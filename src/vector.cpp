// -*- Mode:C++ -*-
//
// Implementation of the Vector class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "vector.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
using namespace::std;
typedef Vector::size_type size_type;
typedef Vector::value_type value_type;

// Creates an object representing a vector.  Does not register new requirements.
Vector::Vector(SolverManager& _manager, size_type _size, value_type _min, value_type _max, Var& _startingVar) :
  size(_size),
  min(_min),
  max(_max),
  manager(_manager),
  startingVar(_startingVar == SolverManager::allocateNew ? _manager.newVars(getNumLiterals()) : _startingVar)
{
  if ( _startingVar == SolverManager::allocateNew ) {
    manager.require(typeRequirement());
  } else {
    _startingVar += getNumLiterals();
  }
}

// Copy constructor.  Does not register requirements.
Vector::Vector(const Vector& copy) :
    manager(copy.manager),
    startingVar(copy.startingVar),
    size(copy.size),
    min(copy.min),
    max(copy.max)
{

}

// The corresponding requirement of being a vector --
// each element is a index
Requirement Vector::typeRequirement() const {
  Requirement result;

  for ( size_type index = 0; index < size; index++ ) {
    result &= (*this)[index].typeRequirement();
  }

  return result;
}

// Index into the index
Index Vector::operator[](size_type index) const {
  // Check that the index is in range.
  if ( index < 0 || index >= size ) {
    ostringstream sout;
    sout << "Incorrect index comparison for Vector.  This matrix size=" << size << " but " << index << " requested.";
    throw out_of_range(sout.str());
  }


  // A bit awkward to have to create a dummy index to figure out how many literals, and then do it again,
  // but these objects are small and we'd be doing basically the same operations even with a more
  // straightforward implementation.
  Var var = 0;
  Index dummyIndex(manager, min, max, var);
  var = startingVar + index*dummyIndex.getNumLiterals();
  return Index(manager, min, max, var);
}

// The vector of (contiguous) literals required to represent this vector.
// Equal to size*(max-min).
unsigned int Vector::getNumLiterals() const {
  Var var = 0;
  Index dummyIndex(manager, min, max, var);
  return size*dummyIndex.getNumLiterals();
}

// After a solution has been found, return a clause requiring a different solution
Clause Vector::diffSolnReq() const {
  Clause clause;
  for ( size_type i=0; i < size; i++ ) {
    clause |= (*this)[i].diffSolnReq();
  }
  return clause;
}

DualClause Vector::currSolnReq() const {
  return ~diffSolnReq();
}


ostream& operator <<(ostream& out, const Vector& vector) {
  if ( vector.size > 0 )
    out << vector[0].modelValue();

  for ( size_type i=1; i < vector.size; i++) {
    out << " " << vector[i].modelValue();
  }

  return out;
}
