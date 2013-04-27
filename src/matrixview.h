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

  class IndexIndexedVector;
  class PairIndexedIndex;

  // Allows indexing of Indices by Indices.  Thus, the location of the Index indexed can
  // depend on constraints.
  IndexIndexedVector operator[](Index row) const;

  // An intermediate class for arriving at a PairIndexedIndex.
  // May be best, at some point, to replace this with a SubscriptWrapper.
  class IndexIndexedVector {
    friend IndexIndexedVector MatrixView::operator[](Index row) const;
  public:
    // Continue indexing -- we just want to get to the PairIndexedIndex.
    PairIndexedIndex operator[](Index col) const;

  private:
    IndexIndexedVector(const MatrixView& matrix, Index row);

    const MatrixView& matrix;
    Index row;
  };

  // A class (intended as a temporary) which can establish restrictions on
  // indices in situations where the exact location of the index depends
  // on other constraints
  class PairIndexedIndex {
    friend PairIndexedIndex MatrixView::IndexIndexedVector::operator[](Index col) const;
  public:
    // Value requirements on PairIndexedIndices
    Requirement operator ==(value_type rhs) const;
    Requirement operator !=(value_type rhs) const;

    Requirement operator == (const Index& rhs) const;
    Requirement operator != (const Index& rhs) const;

  private:
    PairIndexedIndex(const MatrixView& matrix, Index row, Index col);

    const MatrixView& matrix;
    Index row;
    Index col;
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

Requirement operator==(MatrixView::value_type lhs, const MatrixView::PairIndexedIndex& rhs);
Requirement operator!=(MatrixView::value_type lhs, const MatrixView::PairIndexedIndex& rhs);
Requirement operator==(const Index& lhs, const MatrixView::PairIndexedIndex& rhs);
Requirement operator!=(const Index& lhs, const MatrixView::PairIndexedIndex& rhs);

#endif // MATRIXVIEW_H
