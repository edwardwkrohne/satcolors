// -*- Mode:C++ -*-
//
// Views of matrices support essentially a subset of matrix features, so
//  putting common features in a new class, MatrixView, and Matrix-only features
//  in MatrixView simplifies implementation.
//
// TODO: (serious) Combine Matrix and MatrixView.  The separation
// between Matrix and MatrixView made sense when classes weren't
// mutable.  Now that they are, having MatrixView reference an
// existing matrix opens the door to scenarios where the view becomes
// out of bounds when the underlying matrix changes.  MatrixViews
// should stand on their own.  But then, there's no reason to
// have a different class for MatrixView.
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

  // Return various related views
  MatrixView restrict(size_type startRow, size_type startCol, size_type endRow, size_type endCol) const;
  MatrixView rotCCW() const;
  MatrixView rotCW() const;
  MatrixView reflectH() const;
  MatrixView reflectV() const;
  MatrixView transpose() const;

  // Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
  SubscriptWrapper<Scalar> operator[](size_type index) const;

  // Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
  // depend on constraints.
  SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> operator[](Scalar row) const;

  size_type  height() const;
  size_type  width() const;

private:
  template<typename T> 
  T transformedRow(T row, T col) const;
  template<typename T>
  T transformedCol(T row, T col) const;

  const Matrix<Scalar>* baseMatrix;
  size_type mStartRow;
  size_type mStartCol;
  size_type mEndRow;
  size_type mEndCol;

  bool mFlipV;
  bool mFlipH;
  bool mTransposed;
  
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
MatrixView<Scalar>::MatrixView(const Matrix<Scalar>& mat) :
  baseMatrix(&mat),
  mStartRow(0),
  mStartCol(0),
  mEndRow(mat.height()),
  mEndCol(mat.width()),
  mTransposed(false),
  mFlipH(false),
  mFlipV(false)
{

}

template<typename Scalar>
template<typename T>
T MatrixView<Scalar>::transformedRow(T row, T col) const {
  if ( !mTransposed ) {
    return mFlipV ? mEndRow-1 - row : mStartRow + row;
  } else {
    return mFlipH ? mEndRow-1 - col : mStartRow + col;
  }  
}

template<typename Scalar>
template<typename T>
T MatrixView<Scalar>::transformedCol(T row, T col) const {
  if ( !mTransposed ) {
    return mFlipH ? mEndCol-1 - col : mStartCol + col;
  } else {
    return mFlipV ? mEndCol-1 - row : mStartCol + row;
  }  
}

// Return a submatrix of the current matrix
template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::restrict(size_type startRow, 
						size_type startCol, 
						size_type endRow, 
						size_type endCol) const {
  if ( startRow > endRow || startCol > endCol ) {
    throw std::invalid_argument("Degenerate view requested.");
  }

  if ( startRow < 0 || startCol <  0 || 
       endRow > height() || endCol > width() ) {
    throw std::out_of_range("Bad dimensions for matrix view; out of bounds.");
  }

  // We need to calculate the new bounding box, taking into account
  // transformations that are already present.  To do this, begin by
  // transforming the four corners of the box.
  auto newStartRow = transformedRow(startRow, startCol);
  auto newStartCol = transformedCol(startRow, startCol);
  auto newEndRow = transformedRow(endRow, endCol);
  auto newEndCol = transformedCol(endRow, endCol);

  // Now, the corners may be out of order; i.e., we may have start >
  // end.  So swap them if necessary.  Note that the "end" is always
  // one beyond the range, whereas the "start" is always in the range,
  // so add ones to compensate.
  if ( (mFlipV && !mTransposed) || (mFlipH && mTransposed) ) {
    std::swap(newStartRow, newEndRow);
    newStartRow++;
    newEndRow++;
  }

  if ( (mFlipH && !mTransposed) || (mFlipV && mTransposed) ) {
    std::swap(newStartCol, newEndCol);
    newStartCol++;
    newEndCol++;
  }

  MatrixView<Scalar> newView(*this);
  newView.mStartRow = newStartRow;
  newView.mStartCol = newStartCol;
  newView.mEndRow   = newEndRow;
  newView.mEndCol   = newEndCol;

  return newView;
}

template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::rotCCW() const {
  return (*this).transpose().reflectH();
}

template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::rotCW() const {
  return (*this).transpose().reflectV();
}

template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::reflectH() const {
  MatrixView<Scalar> newView(*this);
  if ( !newView.mTransposed ) {
    newView.mFlipH = !newView.mFlipH;
  } else {
    newView.mFlipV = !newView.mFlipV;
  }
  return newView;
}

template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::reflectV() const {
  MatrixView<Scalar> newView(*this);
  if ( !newView.mTransposed ) {
    newView.mFlipV = !newView.mFlipV;
  } else {
    newView.mFlipH = !newView.mFlipH;
  }
  return newView;
}

template<typename Scalar>
MatrixView<Scalar> MatrixView<Scalar>::transpose() const {
  MatrixView<Scalar> newView(*this);
  newView.mTransposed = !newView.mTransposed;
  return newView;
}

// Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
template<typename Scalar>
SubscriptWrapper<Scalar> MatrixView<Scalar>::operator[](size_type row) const {
  // This closure almost identical to that in the other operator[].
  // Not sure if it's worth the additional complexity to consolidate code.
  auto closure = [=] (size_type col) {
    auto accessRow = this->transformedRow(row, col);
    auto accessCol = this->transformedCol(row, col);
    return (*baseMatrix)[accessRow][accessCol];
  };

  return SubscriptWrapper<Scalar>(closure);
}

// Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
// depend on constraints.
template<typename Scalar>
SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> MatrixView<Scalar>::operator[](Scalar row) const {
  // This closure almost identical to that in the other operator[].
  // Not sure if it's worth the additional complexity to consolidate code.
  auto closure = [=] (Scalar col) {
    auto accessRow = this->transformedRow(row, col);
    auto accessCol = this->transformedCol(row, col);
    return (*baseMatrix)[accessRow][accessCol];
  };
  
  return SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar>(closure);
}

template<typename Scalar>
typename MatrixView<Scalar>::size_type MatrixView<Scalar>::height() const {
  if ( !mTransposed ) {
    return mEndRow - mStartRow;
  } else {
    return mEndCol - mStartCol;
  }
}

template<typename Scalar>
typename MatrixView<Scalar>::size_type MatrixView<Scalar>::width() const {
  if ( !mTransposed ) {
    return mEndCol - mStartCol;
  } else {
    return mEndRow - mStartRow;
  }
}

#endif // MATRIXVIEW_H
