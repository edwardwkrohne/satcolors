// -*- Mode:C++ -*-
//
// Class to house the constraints for marker regions
//

#ifndef ALMOSTLINES_H
#define ALMOSTLINES_H

#include <iostream>
#include <functional>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"
#include "compoundobject.h"
#include "ordinal.h"
#include "list2d.h"

class AlmostLinesImpl {
public:
  typedef Index::value_type value_type;
  typedef Index::value_type size_type;

  // Creates an object representing the marker regions structure.
  AlmostLinesImpl(SolverManager& manager, 
		  size_type height, 
		  size_type width, 
		  value_type minThickness, 
		  value_type maxThickness, 
		  Minisat::Var& startingVar = SolverManager::allocateNew);

  // The corresponding requirement of being an object of this type.
  Requirement typeRequirement() const;

  // Require a different/same set of marker regions as the current.
  Clause diffSolnReq() const;

  // Returns a stream manipulator that plots what must, must not, can be, or must be with some assumptions,
  // in the boundary.
  std::function<std::ostream& (std::ostream&)> yesNoMaybeRedBlue(const DualClause& clause, Minisat::Lit redLit, Minisat::Lit blueLit) const;

  SolverManager& manager;

  const size_type height;
  const size_type width;
  const value_type minThickness;
  const value_type maxThickness;

  List2d<Ordinal>   majorOffsets;
  List2d<Ordinal>   minorOffsets;

  std::tuple<List2d<Ordinal>&, List2d<Ordinal>&> objTuple;
};

typedef CompoundObject<AlmostLinesImpl> AlmostLines;

// Output operator
std::ostream& operator<<(std::ostream& out, const AlmostLinesImpl& almostLines);

#endif // ALMOSTLINES_H
