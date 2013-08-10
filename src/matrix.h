// -*- Mode:C++ -*-
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
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "cardinal.h"
#include "grid.h"
#include "solvermanager.h"

template<class Scalar>
class MatrixView;

template<class Scalar>
class PairIndexedScalar;

template<typename Scalar = Cardinal>
class Matrix : public Grid<Scalar> {
public:
  typedef typename Scalar::value_type value_type;
  typedef value_type size_type;

  Matrix(SolverManager* manager, 
	 size_type height, 
	 size_type width, 
	 value_type min, 
	 value_type max, 
	 Minisat::Var& startingVar = SolverManager::allocateNew);

  Matrix() = delete;
  Matrix(const Matrix& copy) = default; // deep copy inherited from vector
  Matrix(Matrix&& move) = default;
  Matrix& operator=(const Matrix& copy) = default;
  Matrix& operator=(Matrix&& move) = default;

  MatrixView<Scalar> view(size_type startRow, size_type startCol, size_type endRow, size_type endCol) const;

  using Grid<Scalar>::operator[];
  SubscriptWrapper<PairIndexedScalar<Scalar>, Scalar> operator[](Scalar row) const;

};

template<typename Scalar>
Matrix<Scalar>::Matrix(SolverManager* _manager, 
		       size_type _height, 
		       size_type _width, 
		       value_type _min, 
		       value_type _max, 
		       Minisat::Var& _startingVar) :
  Grid<Scalar>(_height, 
	       _width, 
	       [&] (value_type row, value_type col) { 
		 return Scalar(_manager, _min, _max, _startingVar);
	       })

{

}

template<typename Scalar>
MatrixView<Scalar> Matrix<Scalar>::view(size_type startRow, 
					size_type startCol, 
					size_type endRow, 
					size_type endCol) const {
  return MatrixView<Scalar>(this, startRow, startCol, endRow, endCol);
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
  typedef typename Matrix<Scalar>::size_type size_type;
  for ( size_type i = 0; i < matrix.height(); i++) {
    out << "    ";
    for ( size_type j = 0; j < matrix.width(); j++ ) {
      // FIXME: why do I have to cast this?
      out << int(matrix[i][j]) << " ";
    }
    out << std::endl;
  }
  return out;
}

#endif // MATRIX_H
