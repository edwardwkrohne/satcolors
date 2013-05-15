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
#include "cardinal.h"
#include "matrix.h"

template<typename Scalar>
class Matrix;

template<typename Scalar = Cardinal>
class MatrixView {
public:
  typedef typename Matrix<Scalar>::value_type value_type;
  typedef typename Matrix<Scalar>::size_type size_type;

  MatrixView(const Matrix<Scalar>& other); 
  MatrixView(const MatrixView& copy) = default;
  //  MatrixView(MatrxView&& move) = default;
  MatrixView& operator=(const MatrixView& copy) = default;
  MatrixView& operator=(MatrixView&& copy) = default;

  MatrixView(const Matrix<Scalar>* mat, size_type startRow, size_type startCol, size_type endRow, size_type endCol);

  // Return a submatrix of the current matrix
  MatrixView view(size_type startRow, size_type startCol, size_type endRow, size_type endCol) const;

  // Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
  SubscriptWrapper<Scalar> operator[](size_type index) const;

  // Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
  // depend on constraints.
  SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> operator[](Scalar row) const;

  size_type  height() const;
  size_type  width() const;
private:
  const Matrix<Scalar>* baseMatrix;
  size_type mStartRow;
  size_type mStartCol;
  size_type mEndRow;
  size_type mEndCol;
};

// Output operator
template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const MatrixView<Scalar>& matrix) {
  typedef typename MatrixView<Scalar>::size_type size_type;
  for ( size_type i = 0; i < matrix.height(); i++) {
    out << "    ";
    for ( size_type j = 0; j < matrix.width(); j++ ) {
      out << matrix[i] << " ";
    }
    out << std::endl;
  }

  return out;
}

template<typename Scalar>
MatrixView<Scalar>::MatrixView(const Matrix<Scalar>* mat, 
			       size_type startRow, 
			       size_type startCol, 
			       size_type endRow, 
			       size_type endCol) :
  baseMatrix(mat),
  mStartRow(startRow),
  mStartCol(startCol),
  mEndRow(endRow),
  mEndCol(endCol)
{
  if ( mStartRow > mEndRow || mStartCol > mEndCol ) {
    throw std::invalid_argument("Degenerate view requested.");
  }

  if ( mStartRow < 0 || mStartCol < 0 || mEndRow >= mat->height() || mEndCol >= mat->width() ) {
    throw std::out_of_range("Bad dimensions for matrix view; out of bounds.");
  }

}

template<typename Scalar>
MatrixView<Scalar>::MatrixView(const Matrix<Scalar>& mat) :
  baseMatrix(&mat),
  mStartRow(0),
  mStartCol(0),
  mEndRow(mat.height()),
  mEndCol(mat.width())  
{

}

// Return a submatrix of the current matrix
template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::view(size_type startRow, 
					    size_type startCol, 
					    size_type endRow, 
					    size_type endCol) const {
  return MatrixView<Scalar>(baseMatrix, mStartRow+startRow, mStartCol+startCol, mStartRow+endRow, mStartCol+endCol);
}

// Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
template<typename Scalar>
SubscriptWrapper<Scalar> MatrixView<Scalar>::operator[](size_type row) const {
  auto closure = [=] (size_type col) {      
    return (*baseMatrix)[row + mStartRow][col + mStartCol];
  };

  return SubscriptWrapper<Scalar>(closure);
}

// Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
// depend on constraints.
template<typename Scalar>
SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> MatrixView<Scalar>::operator[](Scalar row) const {
  auto closure = [=] (Scalar col) {
    return PairIndexedScalar<Scalar>(baseMatrix, row + mStartRow, col + mStartCol);
  };
  
  return SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar>(closure);
}

template<typename Scalar>
typename MatrixView<Scalar>::size_type MatrixView<Scalar>::height() const {
  return mEndRow - mStartRow;
}

template<typename Scalar>
typename MatrixView<Scalar>::size_type MatrixView<Scalar>::width() const {
  return mEndCol - mStartCol;
}

#endif // MATRIXVIEW_H
