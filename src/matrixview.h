// -*- Mode:C++ -*-
//
// Views of matrices support essentially a subset of matrix features, so
//  putting common features in a new class, MatrixViewView, and MatrixView-only features
//  in MatrixView simplifies implementation.
//
// Consider the fact that MatrixViewView is implemented in terms of MatrixView to be an
//  implementation detail.  There is no reason to rely on it.
//

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"
#include "vector.h"

class MatrixView {
public:
  typedef Vector::value_type value_type;
  typedef Vector::size_type size_type;

  // Copy constructor.  Does not register requirements.
  MatrixView(const MatrixView& copy);

  // Return a submatrix of the current matrix
  MatrixView view(size_type startRow, size_type startCol, size_type endRow, size_type endCol) const;

  // Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
  Vector operator[](size_type index) const;

  class ScalarIndexedVector;
  class PairIndexedScalar;

  // Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
  // depend on constraints.
  ScalarIndexedVector operator[](Scalar row) const;

  // An intermediate class for arriving at a PairIndexedScalar.
  // May be best, at some point, to replace this with a SubscriptWrapper.
  class ScalarIndexedVector {
    friend ScalarIndexedVector MatrixView::operator[](Scalar row) const;
  public:
    // Continue indexing -- we just want to get to the PairIndexedScalar.
    PairIndexedScalar operator[](Scalar col) const;

  private:
    ScalarIndexedVector(const MatrixView& matrix, Scalar row);

    const MatrixView& matrix;
    Scalar row;
  };

  // A class (intended as a temporary) which can establish restrictions on
  // scalars in situations where the exact location of the scalar depends
  // on other constraints
  class PairIndexedScalar {
    friend PairIndexedScalar MatrixView::ScalarIndexedVector::operator[](Scalar col) const;
  public:
    // Value requirements on PairIndexedScalars
    Requirement operator ==(value_type rhs) const;
    Requirement operator !=(value_type rhs) const;

    Requirement operator == (const Scalar& rhs) const;
    Requirement operator != (const Scalar& rhs) const;

  private:
    PairIndexedScalar(const MatrixView& matrix, Scalar row, Scalar col);

    const MatrixView& matrix;
    Scalar row;
    Scalar col;
  };

  const size_type height;
  const size_type width;
  const value_type min;
  const value_type max;
  const size_type pitch; // Distance between rows

  SolverManager& manager;
  const Minisat::Var startingVar;

protected:
  // Creates an object representing a matrix.  Does not register requirements.
  // Does NOT update startingVar; Matrix can do that.
  MatrixView(SolverManager& manager, Minisat::Var startingVar, size_type height, size_type width, size_type pitch, value_type min, value_type max);
};

// Output operator
std::ostream& operator<<(std::ostream& out, const MatrixView& matrix);

Requirement operator==(MatrixView::value_type lhs, const MatrixView::PairIndexedScalar& rhs);
Requirement operator!=(MatrixView::value_type lhs, const MatrixView::PairIndexedScalar& rhs);
Requirement operator==(const Scalar& lhs, const MatrixView::PairIndexedScalar& rhs);
Requirement operator!=(const Scalar& lhs, const MatrixView::PairIndexedScalar& rhs);

#endif // MATRIXVIEW_H
