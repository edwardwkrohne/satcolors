// -*- Mode:C++ -*-
//
// Class to house the constraints for marker regions
//
// This class encapsulates some combinatorial work I was doing last
// year; it probably has little relevance to anyone but myself except
// as an example of the use of CompoundObject. (EK Sep 2014)
//

#ifndef ALMOSTLINES_H
#define ALMOSTLINES_H

#include <iostream>
#include <functional>
#include "requirement.h"
#include "solvermanager.h"
#include "compoundobject.h"
#include "ordinal.h"
#include "grid.h"

class AlmostLinesImpl {
public:
  typedef int size_type;

  // Creates an object representing the marker regions structure.
  AlmostLinesImpl(SolverManager* manager, 
		  size_type height, 
		  size_type width, 
		  int minThickness, 
		  int maxThickness, 
		  unsigned int& startingVar = SolverManager::allocateNew);

  // The corresponding requirement of being an object of this type.
  Requirement typeRequirement() const;

  // Require a different/same set of marker regions as the current.
  Clause diffSolnReq() const;

  // Returns a stream manipulator that plots what must, must not, can be, or must be with some assumptions,
  // in the boundary.
  std::function<std::ostream& (std::ostream&)> yesNoMaybeRedBlue(const DualClause& clause, Literal redLit, Literal blueLit) const;

  SolverManager* manager;

  const size_type height;
  const size_type width;
  const int minThickness;
  const int maxThickness;

  Grid<Ordinal>   majorOffsets;
  Grid<Ordinal>   minorOffsets;

  std::tuple<Grid<Ordinal>&, Grid<Ordinal>&> objTuple;
};

typedef CompoundObject<AlmostLinesImpl> AlmostLines;

// Output operator
std::ostream& operator<<(std::ostream& out, const AlmostLinesImpl& almostLines);

#endif // ALMOSTLINES_H
