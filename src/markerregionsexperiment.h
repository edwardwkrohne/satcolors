// -*- Mode:C++ -*-
//
// Class to house the constraints for marker regions
//

#ifndef MARKERREGIONSEXPERIMENT_H
#define MARKERREGIONSEXPERIMENT_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include <functional>
#include "requirement.h"
#include "solvermanager.h"
#include "markerregions.h"
#include "isolatedpattern.h"
#include "list2d.h"

class MarkerRegionsExperiment {
public:
  typedef Matrix::value_type value_type;
  // Usual constructors
  MarkerRegionsExperiment(SolverManager& manager, value_type gridSize, Array2d<std::string> patternStrings, Minisat::Var& var = SolverManager::allocateNew);
  MarkerRegionsExperiment(const MarkerRegionsExperiment& copy);

  // Usual access methods.  Behavior similar to that of Cardinal, Matrix, etc.
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;
  unsigned int getNumLiterals() const;
  Requirement typeRequirement() const;

  std::function<std::ostream&(std::ostream&)> yesNoMaybe(const DualClause& clause) const;

  void propagationExperiment(std::function<bool(void)> continueCondition, std::function<void(const DualClause&)> action) const;

  // The absolute row and column of the center of the pattern given by (row, col).
  value_type patternCenterRow(value_type row, value_type col);
  value_type patternCenterCol(value_type row, value_type col);

  Array2d<std::string> patternStrings;
  const value_type gridSize;
  const value_type height;
  const value_type width;

  SolverManager& manager;
  MarkerRegions markerRegions;
  Matrix brackets;
  List2d<IsolatedPattern> grids;

private:
  std::function<IsolatedPattern(value_type, value_type)> 
  createGridBuilder(SolverManager* manager, Minisat::Var& var) const;
};

// Output operator
std::ostream& operator<<(std::ostream& out, const MarkerRegionsExperiment& markerRegionsExperiment);

#endif // MARKERREGIONSEXPERIMENT_H
