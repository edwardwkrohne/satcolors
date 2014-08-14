// -*- Mode:C++ -*-
//
// A class (intended as a temporary) which can establish restrictions on
// cardinals in situations where the exact location of the index depends
// on other constraints
//

#ifndef PAIRINDEXEDSCALAR_H
#define PAIRINDEXEDSCALAR_H

#include "matrix.h"
#include <algorithm>

template<class Scalar>
class Matrix;

template<class Scalar>
class MatrixView;

template<class Scalar>
class PairIndexedScalar {
public:
  typedef typename Scalar::value_type value_type;
  typedef value_type size_type;

  // Value requirements on PairIndexedScalars
  Requirement operator ==(value_type rhs) const;
  Requirement operator !=(value_type rhs) const;

  Requirement operator <=(value_type rhs) const;
  Requirement operator <(value_type rhs)  const;  // Not Yet Implemented
  Requirement operator >=(value_type rhs) const;  // Not Yet Implemented
  Requirement operator >(value_type rhs)  const;  // Not Yet Implemented

  Requirement operator == (Scalar rhs) const;
  Requirement operator != (Scalar rhs) const;

  friend class Matrix<Scalar>;
  friend class MatrixView<Scalar>;
private:
  PairIndexedScalar(const Matrix<Scalar>* matrix, Scalar row, Scalar col);

  const Matrix<Scalar>* matrix;
  Scalar row;
  Scalar col;
};

template<class Scalar>
PairIndexedScalar<Scalar>::PairIndexedScalar(const Matrix<Scalar>* matrix, 
					     Scalar _row, 
					     Scalar _col) :
  matrix(matrix),
  row(_row),
  col(_col)
{
  
}

template<class Scalar>
Requirement PairIndexedScalar<Scalar>::operator <= (value_type rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (value_type)height;
  result &= col >= 0 & col < (value_type)width;

  size_type rowStart = std::max(row.min(), 0);	
  size_type rowEnd   = std::min(row.max(), height);

  size_type colStart = std::max(col.min(), 0);	
  size_type colEnd   = std::min(col.max(), width);

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      Scalar element = (*matrix)[i][j];
      result &= implication(row == (value_type)i & col == (value_type)j,
			    element <= rhs);
    }
  }

  return result;
}

template<class Scalar>
Requirement PairIndexedScalar<Scalar>::operator == (value_type rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (value_type)height;
  result &= col >= 0 & col < (value_type)width;

  size_type rowStart = std::max(row.min(), 0);	
  size_type rowEnd   = std::min(row.max(), height);

  size_type colStart = std::max(col.min(), 0);	
  size_type colEnd   = std::min(col.max(), width);

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      Scalar element = (*matrix)[i][j];
      result &= implication(row == (value_type)i & col == (value_type)j,
			    element == rhs);
    }
  }

  return result;
}

template<class Scalar>
Requirement PairIndexedScalar<Scalar>::operator != (value_type rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (value_type)height;
  result &= col >= 0 & col < (value_type)width;

  size_type rowStart = row.min() <= 0      ? 0      : row.min();
  size_type rowEnd   = row.max() >= height ? height : row.max();

  size_type colStart = col.min() <= 0     ? 0     : col.min();
  size_type colEnd   = col.max() >= width ? width : col.max();

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      result &= implication(row == (value_type)i & col == (value_type)j,
			    (*matrix)[i][j] != rhs);
    }
  }

  return result;
}

// TODO: FIXME: I don't know why these are commented out; EK April 2014
// Not currently implemented?
// template<class Scalar>
// Requirement PairIndexedScalar<Scalar>::operator == (const Scalar& rhs) const {
// }

// template<class Scalar>
// Requirement PairIndexedScalar<Scalar>::operator != (const Scalar& rhs) const {
// }

template<class Scalar = Cardinal>
Requirement operator==(Scalar lhs, const PairIndexedScalar<Scalar>& rhs) {
  return rhs == lhs;
}

template<class Scalar = Cardinal>
Requirement operator!=(Scalar lhs, const PairIndexedScalar<Scalar>& rhs) {
  return rhs != lhs;
}

// TODO: FIXME: I don't know why these are commented out; EK April 2014
// Not currently implemented?
// template<class Scalar>
// Requirement operator==(typename Matrix<Scalar>::value_type lhs, const PairIndexedScalar<Scalar>& rhs) {
//   return rhs == lhs;
// }

// template<class Scalar = Cardinal>
// Requirement operator!=(typename Matrix<Scalar>::value_type lhs, const PairIndexedScalar<Scalar>& rhs) {
//   return rhs != lhs;
// }

#endif // PAIRINDEXEDSCALAR_H
