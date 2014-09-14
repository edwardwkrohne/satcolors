// -*- Mode:C++ -*-
//
// Class to house the constraints for marker regions
//
// DEPRECATED 5/15/2013
//

#ifndef MARKERREGIONSEXPERIMENT_H
#define MARKERREGIONSEXPERIMENT_H

#include <iostream>
#include <functional>
#include "requirement.h"
#include "solvermanager.h"
#include "markerregions.h"
#include "isolatedpattern.h"
#include "grid.h"

class MarkerRegionsExperiment {
public:
  // Usual constructors
  MarkerRegionsExperiment(SolverManager* manager, 
			  int gridSize, 
			  Array2d<std::string> patternStrings, 
			  unsigned int& var = SolverManager::allocateNew);
  MarkerRegionsExperiment(const MarkerRegionsExperiment& copy);

  // Usual access methods.  Behavior similar to that of Cardinal, Matrix, etc.
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;
  unsigned int getNumLiterals() const;
  Requirement typeRequirement() const;

  std::function<std::ostream&(std::ostream&)> yesNoMaybe(const DualClause& clause) const;

  void propagationExperiment(std::function<bool(void)> continueCondition, std::function<void(const DualClause&)> action) const;

  // The absolute row and column of the center of the pattern given by (row, col).
  int patternCenterRow(int row, int col);
  int patternCenterCol(int row, int col);

  Array2d<std::string> patternStrings;
  const int gridSize;
  const int height;
  const int width;

  SolverManager* manager;
  MarkerRegions markerRegions;
  Matrix<> brackets;
  Grid<IsolatedPattern> grids;

private:
  std::function<IsolatedPattern(int, int)> 
  createGridBuilder(SolverManager* manager, unsigned int& var) const;
};

// Output operator
std::ostream& operator<<(std::ostream& out, const MarkerRegionsExperiment& markerRegionsExperiment);

#endif // MARKERREGIONSEXPERIMENT_H
