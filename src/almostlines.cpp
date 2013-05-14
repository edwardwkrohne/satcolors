// -*- Mode:C++ -*-
//
// Implementation of the AlmostLinesImpl class

#include <iterator>
#include "almostlines.h"
#include "requirementinserter.h"
#include "manipulators.h"
#include <iomanip>

using Minisat::Var;
using namespace std;
using Minisat::Lit;

// Creates an object representing the marker regions structure.  Registers requirements if
// var is SolverManager::allocateNew
AlmostLinesImpl::AlmostLinesImpl(
    SolverManager& _manager,
    size_type _height,
    size_type _width,
    value_type _minThickness,
    value_type _maxThickness,
    Var& var)
:
    manager     (_manager),
    height      (_height),
    width       (_width),
    minThickness(_minThickness),
    maxThickness(_maxThickness),

    majorOffsets (&_manager, 
		  _height+2, 
		  _width+2, 
		  [&](int row, int col) {
		    return Ordinal(&manager, -height-2, width+height+4, var);
		  },
		  var),

    minorOffsets (&_manager, 
		  _height+2, 
		  _width+2, 
		  [&](int row, int col) {
		    return Ordinal(&manager, 0, maxThickness, var);
		  },
		  var),

    objTuple(majorOffsets, minorOffsets)
{
}

// Require a different set of marker regions from the current.
Clause AlmostLinesImpl::diffSolnReq() const {
  return minorOffsets.diffSolnReq();
}

// Because this function template is private, it is appropriate
// to place its definition in this source file.
Requirement AlmostLinesImpl::typeRequirement() const {
  Requirement result;

  for ( int row = 0; row < height-1; row++ ) {
    for ( int col = 0; col < width-1; col++ ) {
      auto mnrOff = minorOffsets[row][col];
      auto rMnrOff = minorOffsets[row][col+1];
      auto drMnrOff = minorOffsets[row+1][col+1];

      // Enforce minimum thickness: no change of majOffset allowed
      result &= implication(mnrOff < minThickness, rMnrOff == mnrOff+1);

      // If greater than minimum thickness, change of majOffset optional
      result &= implication(mnrOff < maxThickness-1, rMnrOff == mnrOff+1 | rMnrOff == 0);
      
      // If at maximum thickness, change of majOffset required
      result &= implication(mnrOff == maxThickness-1, rMnrOff == 0);

      // If a change of majOffset occurs, a change occurs nearby on the next level.
      result &= implication(mnrOff == 0, drMnrOff <= 2);

      auto majOff  = majorOffsets[row][col];
      auto rMajOff = majorOffsets[row][col+1];
      auto lMajOff = majorOffsets[row+1][col];

      // only increments by one are allowed
      result &= majOff <= rMajOff & rMajOff <= majOff+1;

      // Ensure the next row maintains the same major offset as the current row.
      result &= implication(mnrOff > 0 & mnrOff < minThickness, lMajOff == majOff);

      // If an increment actually happens, minor offset must reset.
      result &= implication(rMnrOff == 0, majOff <  rMajOff);
      result &= implication(rMnrOff != 0, majOff >= rMajOff);
    }
  }
  return result;
}

std::ostream& operator<<(std::ostream& out, const AlmostLinesImpl& almostLines) {
  for ( int row = 0; row < almostLines.height; row++) {
    for ( int col = 0; col < almostLines.width; col++ ) {
      out << " " << setw(2) << almostLines.minorOffsets[row][col].modelValue();
    }
    out << endl;
  }
  return out;
}
