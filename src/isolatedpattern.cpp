// -*- Mode:C++ -*-
//
// Implementation of the IsolatedPattern class

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "isolatedpattern.h"

using Minisat::Var;
using namespace std;

typedef IsolatedPattern::size_type size_type;

namespace {
Requirement typeSpecificTypeRequirement(const IsolatedPattern& pat) {
  Requirement result;

  // Much simpler requirements are available if the pattern is blank.
  if ( pat.pattern.isBlank ) {
    for ( size_type i = 0; i < pat.matrix.height(); i++ ) {
      for ( size_type j = 0; j < pat.matrix.width(); j++ ) {
        result &= pat.matrix[i][j] == 0;
      }
    }
    return result;
  }

  // centerIndicator[row][col] is the only cell in centerIndicator that is 1.
  // centerIndicator is the indicator function of the center point for the pattern.
  for ( size_type i = 0; i < pat.centerIndicator.height(); i++ ) {
    for ( size_type j = 0; j < pat.centerIndicator.width(); j++ ) {
      result &= equivalence(
          pat.row == i+pat.patternCenterRow & pat.col == j+pat.patternCenterCol,
          pat.centerIndicator[i][j] == 1);
    }
  }

  // Make a clause for each cell in the matrix
  Array2d<Clause> clauses(pat.matrix.height(), pat.matrix.width());

  // Loop through the possible values that row and col can take
  for ( size_type i = pat.row.min(); i < pat.row.max(); i++ ) {
    for ( size_type j = pat.col.min(); j < pat.col.max(); j++ ) {
      // Loop through all the cells in the pattern
      for ( size_type i2 = 0; i2 < pat.patternHeight; i2++ ) {
        for ( size_type j2 = 0; j2 < pat.patternWidth; j2++ ) {
          // If pattern[i2][j2] has been filled in, then centerIndicator[i][j] == 1 is a possible way for
          // the cell associated with clauses[i-patternCenterRow+i2][j-patternCenterCol+j2] to itself be nonzero.
          if ( pat.pattern[i2][j2] ) {
            clauses[i-pat.patternCenterRow+i2][j-pat.patternCenterCol+j2] |=
                (pat.centerIndicator[i-pat.row.min()][j-pat.col.min()] == 1);
          }
        }
      }
    }
  }

  // Now, we have clauses specifying how each cell in the matrix can be one.  Make this an equivalence
  // in each case.
  for ( size_type i = 0; i < pat.matrix.height(); i++ ) {
    for ( size_type j = 0; j < pat.matrix.width(); j++ ) {
      result &= equivalence(move(clauses[i][j]), pat.matrix[i][j] == 1);
    }
  }

  return result;
}
}

IsolatedPattern::IsolatedPattern(
  SolverManager* _manager,
  const MatrixView<>& _matrix,
  const std::string& strPattern,
  Var& var)
:
    manager(_manager),
    pattern(strPattern),
    patternHeight   (pattern.height),
    patternWidth    (pattern.width),
    patternCenterRow(pattern.centerRow),
    patternCenterCol(pattern.centerCol),
    matrix(_matrix),
    row(_manager, patternCenterRow, _matrix.height() - (patternHeight - patternCenterRow)+1, var),
    col(_manager, patternCenterCol, _matrix.width()  - (patternWidth  - patternCenterCol)+1, var),
    centerIndicator(_manager, _matrix.height()-patternHeight+1, _matrix.width()-patternWidth+1, 0, 2, var)
{
  if ( var == SolverManager::allocateNew ) {
    manager->require(typeSpecificTypeRequirement(*this));
  }

}

IsolatedPattern::IsolatedPattern(
  SolverManager* _manager,
  const MatrixView<>& _matrix,
  const Pattern& _pattern,
  Var& var)
:
    manager(_manager),
    pattern(_pattern),
    patternHeight   (pattern.height),
    patternWidth    (pattern.width),
    patternCenterRow(pattern.centerRow),
    patternCenterCol(pattern.centerCol),
    matrix(_matrix),
    row(_manager, patternCenterRow, _matrix.height() - (patternHeight - patternCenterRow)+1, var),
    col(_manager, patternCenterCol, _matrix.width()  - (patternWidth  - patternCenterCol)+1, var),
    centerIndicator(_manager, _matrix.height()-patternHeight+1, _matrix.width()-patternWidth+1, 0, 2, var)
{
  if ( var == SolverManager::allocateNew ) {
    manager->require(typeSpecificTypeRequirement(*this));
  }
}

Clause IsolatedPattern::diffSolnReq() const {
  // If the pattern is blank, there ARE no different solutions.  So return a failing clause.
  if ( pattern.isBlank ) {
    return Clause();
  }

  // Otherwise require that either the row or column be different.
  else {
    return row.diffSolnReq() | col.diffSolnReq();
  }
}

DualClause IsolatedPattern::currSolnReq() const {
  return ~diffSolnReq();
}

size_type IsolatedPattern::calcPatternHeight(const std::string& pattern) {
  size_type patternHeight;
  size_type patternWidth;
  size_type patternCenterRow;
  size_type patternCenterCol;
  calcPatternParams(pattern, patternHeight, patternWidth, patternCenterRow, patternCenterCol);
  return patternHeight;
}

size_type IsolatedPattern::calcPatternWidth(const std::string& pattern) {
  size_type patternHeight;
  size_type patternWidth;
  size_type patternCenterRow;
  size_type patternCenterCol;
  calcPatternParams(pattern, patternHeight, patternWidth, patternCenterRow, patternCenterCol);
  return patternWidth;
}

size_type IsolatedPattern::calcPatternCenterRow(const std::string& pattern) {
  size_type patternHeight;
  size_type patternWidth;
  size_type patternCenterRow;
  size_type patternCenterCol;
  calcPatternParams(pattern, patternHeight, patternWidth, patternCenterRow, patternCenterCol);
  return patternCenterRow;
}

size_type IsolatedPattern::calcPatternCenterCol(const std::string& pattern) {
  size_type patternHeight;
  size_type patternWidth;
  size_type patternCenterRow;
  size_type patternCenterCol;
  calcPatternParams(pattern, patternHeight, patternWidth, patternCenterRow, patternCenterCol);
  return patternCenterCol;
}

void IsolatedPattern::calcPatternParams(const std::string& pattern,
    size_type& patternHeight,
    size_type& patternWidth,
    size_type& patternCenterRow,
    size_type& patternCenterCol) {

  patternHeight = 0;
  patternWidth = 0;
  patternCenterRow = 0;
  patternCenterCol = 0;

  istringstream sin(pattern);

  size_type row = 0;
  size_type col = 0;

  for ( string marking; sin >> marking; ) {

    // Look for end of row
    if ( marking == ";" ) {
      col = 0;
      patternHeight = max(++row, patternHeight);
      continue;
    }

    // Look for center
    if ( marking == "(.)" || marking == "(x)" ) {
      patternCenterRow = row;
      patternCenterCol = col;
    }

    // Increment column
    patternWidth  = max(++col, patternWidth);
  }
}

void IsolatedPattern::calcPatternShape(const string& strPattern, Array2d<bool>& boolPattern) {
  istringstream sin(strPattern);

  size_type row = 0;
  size_type col = 0;

  for ( string marking; sin >> marking; ) {
    // Look for end of row
    if ( marking == ";" ) {
      col = 0;
      row++;
      continue;
    }

    // Fill in any spaces
    boolPattern[row][col] = ( marking == "(x)" || marking == "x" );

    col++;
  }
}
