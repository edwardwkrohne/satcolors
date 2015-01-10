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
// An matrix of Cardinals
//
// A 2-dimensional matrix of Cardinals. Overloads the subscript operator
// to return a corresponding element in a particular location, upon which requirements
// can be established.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "requirement.h"
#include "cardinal.h"
#include "grid.h"
#include "solver.h"

template<class Scalar>
class MatrixView;

template<class Scalar> // Are these needed now?
class PairIndexedScalar;

template<typename Scalar = Cardinal>
class Matrix : public Grid<Scalar> {
public:
  Matrix(Solver* solver, 
	 int height, 
	 int width, 
	 int min, 
	 int max);

  Matrix() = delete;
  Matrix(const Matrix& copy) = default; // deep copy inherited from vector
  Matrix(Matrix&& move) = default;
  Matrix& operator=(const Matrix& copy) = default;
  Matrix& operator=(Matrix&& move) = default;

  // Return various views on the matrix
  MatrixView<Scalar> restrict(int startRow, int startCol, int endRow, int endCol) const;
  MatrixView<Scalar> rotCCW() const;
  MatrixView<Scalar> rotCW() const;
  MatrixView<Scalar> reflectH() const;
  MatrixView<Scalar> reflectV() const;
  MatrixView<Scalar> transpose() const;

  using Grid<Scalar>::operator[];
  SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> operator[](Scalar row) const;

};

template<typename Scalar>
Matrix<Scalar>::Matrix(Solver* _solver, 
		       int _height, 
		       int _width, 
		       int _min, 
		       int _max) :
  Grid<Scalar>(_height, 
	       _width, 
	       [&] (int row, int col) { 
		 return Scalar(_solver, _min, _max);
	       })

{

}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::restrict(int startRow, 
					    int startCol, 
					    int endRow, 
					    int endCol) const {
  return MatrixView<Scalar>(*this).restrict(startRow, startCol, endRow, endCol);
}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::rotCCW() const {
  return MatrixView<Scalar>(*this).rotCCW();
}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::rotCW() const {
  return MatrixView<Scalar>(*this).rotCW();
}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::reflectH() const {
  return MatrixView<Scalar>(*this).reflectH();
}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::reflectV() const {
  return MatrixView<Scalar>(*this).reflectV();
}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::transpose() const {
  return MatrixView<Scalar>(*this).transpose();
}


template<typename Scalar>
SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> Matrix<Scalar>::operator[](Scalar row) const {
  auto closure = [=] (Scalar col) {
    return PairIndexedScalar<Scalar>(this, row, col);
  };
  
  return SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar>(closure);
}

// Output operator
template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Matrix<Scalar>& matrix) {
  for ( int i = 0; i < matrix.height(); i++) {
    out << "    ";
    for ( int j = 0; j < matrix.width(); j++ ) {
      out << matrix[i][j] << " ";
    }
    out << std::endl;
  }
  return out;
}

#endif // MATRIX_H
