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
  typedef int size_type;

  // Value requirements on PairIndexedScalars
  Requirement operator ==(int rhs) const;
  Requirement operator !=(int rhs) const;

  Requirement operator <=(int rhs) const;
  Requirement operator <(int rhs)  const;  // Not Yet Implemented
  Requirement operator >=(int rhs) const;  // Not Yet Implemented
  Requirement operator >(int rhs)  const;  // Not Yet Implemented

  Requirement operator == (Scalar rhs) const; // Not Yet Implemented
  Requirement operator != (Scalar rhs) const; // Not Yet Implemented

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
Requirement PairIndexedScalar<Scalar>::operator <= (int rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (int)height;
  result &= col >= 0 & col < (int)width;

  size_type rowStart = std::max(row.min(), 0);	
  size_type rowEnd   = std::min(row.max(), height);

  size_type colStart = std::max(col.min(), 0);	
  size_type colEnd   = std::min(col.max(), width);

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      Scalar element = (*matrix)[i][j];
      result &= implication(row == (int)i & col == (int)j,
			    element <= rhs);
    }
  }

  return result;
}

template<class Scalar>
Requirement PairIndexedScalar<Scalar>::operator == (int rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (int)height;
  result &= col >= 0 & col < (int)width;

  size_type rowStart = std::max(row.min(), 0);	
  size_type rowEnd   = std::min(row.max(), height);

  size_type colStart = std::max(col.min(), 0);	
  size_type colEnd   = std::min(col.max(), width);

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      Scalar element = (*matrix)[i][j];
      result &= implication(row == (int)i & col == (int)j,
			    element == rhs);
    }
  }

  return result;
}

template<class Scalar>
Requirement PairIndexedScalar<Scalar>::operator != (int rhs) const {
  Requirement result;

  size_type height = matrix->height();
  size_type width = matrix->width();

  // Range requirements
  result &= row >= 0 & row < (int)height;
  result &= col >= 0 & col < (int)width;

  size_type rowStart = row.min() <= 0      ? 0      : row.min();
  size_type rowEnd   = row.max() >= height ? height : row.max();

  size_type colStart = col.min() <= 0     ? 0     : col.min();
  size_type colEnd   = col.max() >= width ? width : col.max();

  for (size_type i = rowStart; i < rowEnd; i++ ) {
    for (size_type j = colStart; j < colEnd; j++ ) {
      result &= implication(row == (int)i & col == (int)j,
			    (*matrix)[i][j] != rhs);
    }
  }

  return result;
}

#endif // PAIRINDEXEDSCALAR_H
