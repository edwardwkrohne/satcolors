// -*- Mode:C++ -*-
//
// Makes two requirements.  First, that a pattern
//
// DEPRECATED 5/15/2013
//

#ifndef ISOLATEDPATTERN_H
#define ISOLATEDPATTERN_H

#include <iostream>
#include "requirement.h"
#include "solvermanager.h"
#include "markerregions.h"
#include "array2d.h"
#include "pattern.h"

class IsolatedPattern {
public:
  typedef Matrix<>::size_type size_type;

  // Usual constructors
  IsolatedPattern() = delete;
  IsolatedPattern(const IsolatedPattern& copy) = default;
  IsolatedPattern(IsolatedPattern&& move) = default;
  
  IsolatedPattern& operator=(const IsolatedPattern& copy) = default;
  IsolatedPattern& operator=(IsolatedPattern&& move) = default;

  IsolatedPattern(SolverManager* manager,
		  const MatrixView<>& matrix,
		  const std::string& pattern,
		  unsigned int& var = SolverManager::allocateNew);
  IsolatedPattern(SolverManager* manager,
		  const MatrixView<>& matrix,
		  const Pattern& pattern,
		  unsigned int& var = SolverManager::allocateNew);

  // Usual access methods.  Behavior similar to that of Cardinal, MatrixView, etc.
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;
  unsigned int getNumLiterals() const;
  Requirement typeRequirement() const;

  // Note: the order of declaration is relevant here.
  // See the constructor.
  SolverManager* manager;
  Pattern pattern;

  size_type patternHeight;
  size_type patternWidth;
  size_type patternCenterRow;
  size_type patternCenterCol;

  MatrixView<> matrix;
  Cardinal row;
  Cardinal col;
  Matrix<> centerIndicator;

private:
  static size_type calcPatternHeight(const std::string& pattern);
  static size_type calcPatternWidth(const std::string& pattern);
  static size_type calcPatternCenterRow(const std::string& pattern);
  static size_type calcPatternCenterCol(const std::string& pattern);

  static void calcPatternParams(const std::string& pattern,
      size_type& patternHeight,
      size_type& patternWidth,
      size_type& patternCenterRow,
      size_type& patternCenterCol);

  void calcPatternShape(const std::string& strPattern, Array2d<bool>& boolPattern);
};

#endif // ISOLATEDPATTERN_H
