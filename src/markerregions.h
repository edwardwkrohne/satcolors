// -*- Mode:C++ -*-
//
// Class to house the constraints for marker regions
//
// DEPRECATED 5/15/2013
//

#ifndef MARKERREGIONS_H
#define MARKERREGIONS_H

#include <iostream>
#include <functional>
#include "requirement.h"
#include "solvermanager.h"
#include "compoundobject.h"
#include "cardinal.h"
#include "matrix.h"
#include "matrixview.h"

class MarkerRegionsImpl {
public:
  typedef int size_type;

  // Creates an object representing the marker regions structure.
  MarkerRegionsImpl(SolverManager* manager, 
		    size_type height, 
		    size_type width, 
		    int regionSize, 
		    unsigned int& startingVar = SolverManager::allocateNew);

  // The corresponding requirement of being an object of this type.
  Requirement typeRequirement() const;

  // Require a different/same set of marker regions as the current.
  Clause diffSolnReq() const;

  // Returns a stream manipulator that plots what must, must not, or can be in a boundary.
  std::function<std::ostream& (std::ostream&)> yesNoMaybe(const DualClause& clause) const;
  std::function<std::ostream& (std::ostream&)> yesNoMaybe() const;
  template <class T>
  std::function<std::ostream& (std::ostream&)> yesNoMaybe(const T& item) const {
    return yesNoMaybe(DualClause(item));
  };

  // Returns a stream manipulator that plots what must, must not, can be, or must be with some assumptions,
  // in the boundary.
  std::function<std::ostream& (std::ostream&)> yesNoMaybeRedBlue(const DualClause& clause, Literal redLit, Literal blueLit) const;

  const size_type height;
  const size_type width;
  const int regionSize;
  // Note: regions vary between regionSize and regionSize+1 in size

  SolverManager* manager;

  Matrix<>     boundaryUnderlay;
  MatrixView<> boundary;
  Matrix<>     teeTeeNeighbor;
  Matrix<>     teeBlankNeighbor;
  Matrix<>     hSpacing;
  Matrix<>     vSpacing;
  Matrix<>     actionRowUnderlay;
  MatrixView<> actionRow;
  Matrix<>     actionColUnderlay;
  MatrixView<> actionCol;
  std::tuple<Matrix<>&, Matrix<>&, Matrix<>&, Matrix<>&, Matrix<>&, Matrix<>&, Matrix<>&> objTuple;

  // Each element can be a boundary or not.  If not, there are several types of boundary it can take.
  enum Boundary {
    BLANK,
    VERTICAL,
    HORIZONTAL,
    TEE,
    CROSS,
    NUM_BOUNDARY
  };

  // Convenient way to refer to specific neighbors
  enum Neighbor {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NUM_NEIGHBOR
  };
};

typedef CompoundObject<MarkerRegionsImpl> MarkerRegions;

// Output operator
std::ostream& operator<<(std::ostream& out, const MarkerRegionsImpl& markerRegions);

// Math helper function
int floorDiv(int n, int m);

#endif // MARKERREGIONS_H
