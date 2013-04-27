// -*- Mode:C++ -*-
//
// An matrix of indices
//
// A 2-dimensional matrix of indices. Overloads the subscript operator
// to return a corresponding matrix in a particular location, upon which requirements
// can be established.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"
#include "vector.h"
#include "matrixview.h"

class Matrix : public MatrixView {
public:
  typedef Vector::value_type value_type;
  typedef Vector::size_type size_type;

  // Creates an object representing a matrix.  Does not register requirements.
  // Updates startingVar by adding getNumLiterals() to it, which makes it easy
  // to construct several objects at once.
  Matrix(SolverManager& manager, size_type height, size_type width, value_type min, value_type max, Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  Matrix(const Matrix& copy);

  // The corresponding requirement of being a matrix.
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this matrix.
  unsigned int getNumLiterals() const;

  // After a solution has been found, get a requirement for the current/a different solution
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;
};
#endif // MATRIX_H
