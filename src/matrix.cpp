// -*- Mode:C++ -*-
//
// Implementation of the Matrix class

#include <cmath>
#include <iostream>
#include "matrix.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
using namespace::std;
typedef Matrix::size_type size_type;
typedef Matrix::value_type value_type;

// Creates an object representing a matrix, using existing literals starting at startingVar.  Does
// NOT register requirements; assumes that appropriate requirements have already been registered.
Matrix::Matrix(SolverManager& _manager, size_type _height, size_type _width, value_type _min, value_type _max, Var& _startingVar) :
    MatrixView(_manager, _height, _width, _width, _min, _max, _startingVar == SolverManager::allocateNew ? _manager.newVars(0) : _startingVar )
{
  // Just do what we would do if we were creating a vector of length height*width.
  Vector underlyingVector(manager, height*width, min, max, _startingVar);
}

// Copy constructor.  Does not register requirements.
Matrix::Matrix(const Matrix& copy) :
    MatrixView(copy)
{

}

// The corresponding requirement of being a matrix --
// each element is a index
Requirement Matrix::typeRequirement() const {
  Requirement result;

  Var var = startingVar;
  Vector underlyingVector(manager, height*width, min, max, var);
  return underlyingVector.typeRequirement();
}

// The matrix of (contiguous) literals required to represent this matrix.
// Equal to height*width*(max-min).
unsigned int Matrix::getNumLiterals() const {
  Var var = startingVar;
  Vector underlyingVector(manager, height*width, min, max, var);
  return underlyingVector.getNumLiterals();
}

// After a solution has been found, get a requirement for a different solution
Clause Matrix::diffSolnReq() const {
  Var var = startingVar;
  Vector underlyingVector(manager, height*width, min, max, var);
  return underlyingVector.diffSolnReq();
}

DualClause Matrix::currSolnReq() const {
  return ~diffSolnReq();
}
