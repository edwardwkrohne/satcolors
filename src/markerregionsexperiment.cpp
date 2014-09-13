// -*- Mode:C++ -*-
//
// Implementation of the MarkerRegions class

#include <iterator>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <minisat/core/Solver.h>

#include "markerregionsexperiment.h"
#include "manipulators.h"
#include "solvermanager.h"

typedef typename Matrix<>::size_type size_type;
typedef typename Matrix<>::value_type value_type;

using Minisat::Var;
using namespace std;

std::function<IsolatedPattern(value_type, value_type)> 
MarkerRegionsExperiment::createGridBuilder(SolverManager* manager, Var& var) const {
  auto builder = [this, manager, &var] (value_type row, value_type col) {
    return IsolatedPattern
    (
        manager,
        brackets.restrict(row*gridSize, col*gridSize, (row+1)*gridSize, (col+1)*gridSize),
        patternStrings[row][col],
        var);
  };

  return builder;
}

MarkerRegionsExperiment::MarkerRegionsExperiment(SolverManager* _manager, size_type _gridSize, Array2d<string> _patternStrings, Var& var) :
    patternStrings(_patternStrings),
    gridSize(_gridSize),
    height(_patternStrings.height()),
    width(_patternStrings.width()),
    manager(_manager),
    markerRegions(manager, height*gridSize, width*gridSize, gridSize, var),
    brackets(manager, height*gridSize, width*gridSize, 0, 2, var),
    grids(height, width, createGridBuilder(manager, var))
{
  if ( var == SolverManager::allocateNew ) {
    for ( size_type i = 0; i < brackets.height(); i++ ) {
      for ( size_type j = 0; j < brackets.width(); j++ ) {
        manager->require(implication(brackets[i][j] == 1, markerRegions.boundary[i][j] != MarkerRegions::BLANK));
      }
    }
  }
}

Clause MarkerRegionsExperiment::diffSolnReq() const {
  return grids.diffSolnReq();
}

DualClause MarkerRegionsExperiment::currSolnReq() const {
  return grids.currSolnReq();
}

ostream& operator << (ostream& out, const MarkerRegionsExperiment& rhs) {
  return out << rhs.brackets;
}

// Plots entailment in the markerRegions.  That is: what must be in the boundary,
// what must not be in the boundary, and what could be or not.
function<ostream& (ostream&)> MarkerRegionsExperiment::yesNoMaybe(const DualClause& clause) const {
  return markerRegions.yesNoMaybe(clause);
}

void MarkerRegionsExperiment::propagationExperiment(std::function<bool(void)> continueCondition, std::function<void(const DualClause&)> action) const {
  while ( continueCondition() && manager->solve() ) {
    auto boundaryScenario = currSolnReq();
    action(boundaryScenario);

    manager->require(~boundaryScenario);
  }
}

// The absolute row and column of the center of the pattern given by (row, col).
value_type MarkerRegionsExperiment::patternCenterRow(value_type row, value_type col) {
  return grids[row][col].row + row*gridSize;
};

value_type MarkerRegionsExperiment::patternCenterCol(value_type row, value_type col) {
  return grids[row][col].col + col*gridSize;
};
