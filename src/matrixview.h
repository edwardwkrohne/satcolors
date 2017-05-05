// -*- Mode: C++ -*-
////////////////////////////////////////////////////////////////////////////
//
//  satcolors -- for writably and readably building problems for SAT-solving
//  Copyright (C) 2014 Edward W. Krohne III
//
//  The research that led to this product was partially supported by the
//  U.S. NSF grants DMS-0943870 and DMS-1201290.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////
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
#include "requirement.h"
#include "solver.h"
#include "cardinal.h"
#include "matrix.h"

template<typename Scalar>
class Matrix;

template<typename Scalar = Cardinal>
class MatrixView {
public:
  MatrixView(const Matrix<Scalar>& other); 
  MatrixView(const MatrixView& copy) = default;
  //  MatrixView(MatrxView&& move) = default;
  MatrixView& operator=(const MatrixView& copy) = default;
  MatrixView& operator=(MatrixView&& copy) = default;

  // Return various related views
  MatrixView restrict(int startRow, int startCol, int endRow, int endCol) const;
  MatrixView rotCCW() const;
  MatrixView rotCW() const;
  MatrixView reflectH() const;
  MatrixView reflectV() const;
  MatrixView transpose() const;

  template<typename LhsMatrixType>
  Requirement operator==(const LhsMatrixType& lhs);
  
  // Return a vector representing a row of the matrix.  Can be used to double-index the matrix.
  SubscriptWrapper<Scalar> operator[](int index) const;

  // Allows indexing of Scalars by Scalars.  Thus, the location of the Scalar indexed can
  // depend on constraints.
  SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> operator[](Scalar row) const;

  unsigned int height() const;
  unsigned int width() const;

private:
  template<typename T> 
  T transformedRow(T row, T col) const;
  template<typename T>
  T transformedCol(T row, T col) const;

  const Matrix<Scalar>* baseMatrix;
  int mStartRow;
  int mStartCol;
  int mEndRow;
  int mEndCol;

  bool mFlipV;
  bool mFlipH;
  bool mTransposed;
  
};

// Output operator
template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const MatrixView<Scalar>& matrix) {
  for ( int i = 0; i < matrix.height(); i++) {
    out << "    ";
    for ( int j = 0; j < matrix.width(); j++ ) {
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
MatrixView<Scalar> MatrixView<Scalar>::restrict(int startRow, 
						int startCol, 
						int endRow, 
						int endCol) const {
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
SubscriptWrapper<Scalar> MatrixView<Scalar>::operator[](int row) const {
  // This closure almost identical to that in the other operator[].
  // Not sure if it's worth the additional complexity to consolidate code.
  auto closure = [=] (int col) {
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
unsigned int MatrixView<Scalar>::height() const {
  if ( !mTransposed ) {
    return mEndRow - mStartRow;
  } else {
    return mEndCol - mStartCol;
  }
}

template<typename Scalar>
unsigned int MatrixView<Scalar>::width() const {
  if ( !mTransposed ) {
    return mEndCol - mStartCol;
  } else {
    return mEndRow - mStartRow;
  }
}

template<typename Scalar>
template<typename LhsMatrixType>
Requirement MatrixView<Scalar>::operator==(const LhsMatrixType& lhs) {
  if ( height() != lhs.height() || width() != lhs.width() ) {
      return Atom::falsity;
  }

  Requirement req;
  
  for (int row = 0; row < height(); row++ ) {
    for (int col = 0; col < width(); col++ ) {
      req &= (*this)[row][col] == lhs[row][col];
    }
  }

  return req;

}

#endif // MATRIXVIEW_H
