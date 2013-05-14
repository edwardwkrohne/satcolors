// -*- Mode:C++ -*-
//
// Implementation of the MatrixViewView class

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "matrixview.h"

using Minisat::Lit;
using Minisat::mkLit;
using Minisat::Var;
using namespace::std;
typedef MatrixView::size_type size_type;
typedef MatrixView::value_type value_type;

// Constructor
MatrixView::MatrixView(SolverManager& _manager, size_type _height, size_type _width, size_type _pitch, value_type _min, value_type _max, Var _startingVar) :
  height(_height),
  width(_width),
  pitch(_pitch),
  min(_min),
  max(_max),
  manager(_manager),
  startingVar(_startingVar)
{

}

// Copy constructor.  Does not register requirements.
MatrixView::MatrixView(const MatrixView& copy) :
    manager(copy.manager),
    startingVar(copy.startingVar),
    height(copy.height),
    width(copy.width),
    pitch(copy.pitch),
    min(copy.min),
    max(copy.max)
{

}

MatrixView MatrixView::view(size_type startRow, size_type startCol, size_type endRow, size_type endCol) const {
  // Check that the index is in range.
  if ( startRow < 0 || startCol < 0 || endRow > height || endCol > width ) {
    ostringstream sout;
    sout << "Out of bounds view requested.  This matrix height=" << height << " and width=" << width
        << ", but [" << startRow << ", " << endRow << ")x[" << startCol << ", " << endCol << ") requested.";
    throw out_of_range(sout.str());
  }

  if ( endRow < startRow || endCol < startCol ) {
    ostringstream sout;
    sout << "Degenerate view [" << startRow << ", " << endRow << ")x[" << startCol << ", " << endCol << ") requested.";
    throw invalid_argument(sout.str());
  }

  Var var = 0;
  Cardinal dummyCardinal(&manager, min, max, var);
  return MatrixView(
      manager,
      endRow-startRow,
      endCol-startCol,
      pitch,
      min,
      max,
      startingVar + (startRow*pitch + startCol)*dummyCardinal.numLiterals());
}

// Index into the matrix, producing a row vector
Vector MatrixView::operator[](size_type index) const {
  // Check that the index is in range.
  if ( index < 0 || index >= height ) {
    ostringstream sout;
    sout << "Incorrect index comparison for Matrix.  This matrix height=" << height << " but " << index << " requested.";
    throw out_of_range(sout.str());
  }


  Var var = 0;
  Cardinal dummyCardinal(&manager, min, max, var);
  var = startingVar + index*pitch*dummyCardinal.numLiterals();
  return Vector(manager, width, min, max, var);
}

// The following four functions access a PairIndexedCardinal
MatrixView::CardinalIndexedVector MatrixView::operator[](Cardinal row) const {
  return CardinalIndexedVector(*this, row);
}

MatrixView::CardinalIndexedVector::CardinalIndexedVector(const MatrixView& _matrix, Cardinal _row) :
    matrix(_matrix),
    row(_row)
{
  ;
}

MatrixView::PairIndexedCardinal MatrixView::CardinalIndexedVector::operator[](Cardinal col) const {
  return PairIndexedCardinal(matrix,row,col);
}

MatrixView::PairIndexedCardinal::PairIndexedCardinal(const MatrixView& _matrix, Cardinal _row, Cardinal _col) :
    matrix(_matrix),
    row(_row),
    col(_col)
{
  ;
}

// Now that we have a PairIndexedCardinal, define its basic equals operation
Requirement MatrixView::PairIndexedCardinal::operator ==(value_type rhs) const {
  Requirement result;

  size_type height = matrix.height;
  size_type width = matrix.width;

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
          matrix[i][j] == rhs);
    }
  }

  return result;
}
Requirement operator==(value_type lhs, const MatrixView::PairIndexedCardinal& rhs) {
  return rhs == lhs;
}

// Define its basic not equals operation
Requirement MatrixView::PairIndexedCardinal::operator !=(value_type rhs) const {
  Requirement result;

  size_type height = matrix.height;
  size_type width = matrix.width;

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
          matrix[i][j] != rhs);
    }
  }

  return result;
}
Requirement operator!=(value_type lhs, const MatrixView::PairIndexedCardinal& rhs) {
  return rhs != lhs;
}

ostream& operator <<(ostream& out, const MatrixView& matrix) {
  for ( size_type i=0; i < matrix.height; i++) {
    out << "    " << matrix[i] << endl;
  }

  return out;
}
