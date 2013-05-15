// -*- Mode:C++ -*-
//
// Implementation of the MarkerRegionsImpl class

#include <iterator>
#include "markerregions.h"
#include "pairindexedscalar.h"
#include "requirementinserter.h"
#include "manipulators.h"

using Minisat::Var;
using namespace std;
using Minisat::Lit;

// Returns the unique integer b such that
//
//    b <= n/m < b+1
//
// regardless of the sign of n or m.
int floorDiv(int n, int m) {
  // Will rounding down naturally occur?
  if ( (n >= 0 && m >= 0) || (n < 0 && m < 0) ) {
    return n/m;
  }

  else {
    // Is there no rounding?
    if ( n % m == 0 ) {
      return n/m;
    }

    // There is rounding, and its up, not down.  So correct it.
    else {
      return -((-n)/m)-1;
    }
  }
};

namespace {
// Consider a MarkerRegions object of height h, width, w, and region size r.
// Each region has associated with it a row relative to the other regions.
// This function returns the maximum possible such row (taking into account
// the extra cells at the edge of the grid).
int maxRelRow(int h, int w, int r) {
  // This formula derived by hand.
  int rowCorrection = floorDiv(w, r);
  return floorDiv(h + 1 + rowCorrection, r) + 2;
}

// Consider a MarkerRegions object of width, w, and region size r.
// Each region has associated with it a row relative to the other regions.
// This function returns the minimum possible such row.
int minRelRow(int w, int r) {
  // This formula derived by hand.
  return floorDiv(-w-1, r*r);
}

}

// Creates an object representing the marker regions structure.  Registers requirements if
// var is SolverManager::allocateNew
MarkerRegionsImpl::MarkerRegionsImpl(
    SolverManager* _manager,
    size_type _height,
    size_type _width,
    value_type _regionSize,
    Var& var)
:
    manager   (_manager),
    height    (_height),
    width     (_width),
    regionSize(_regionSize),

    boundaryUnderlay (_manager, _height+2, _width+2, 0,        NUM_BOUNDARY,     var),
    teeTeeNeighbor   (_manager, _height+2, _width+2, 0,        NUM_NEIGHBOR,     var),
    teeBlankNeighbor (_manager, _height+2, _width+2, 0,        NUM_NEIGHBOR,     var),
    hSpacing         (_manager, _height+2, _width+2, 0,        _regionSize+1,    var),
    vSpacing         (_manager, _height+2, _width+2, 0,        _regionSize+1,    var),
    actionRowUnderlay(_manager, _height+2, _width+2, minRelRow(_width,  _regionSize), maxRelRow(_height, _width, _regionSize), var),
    actionColUnderlay(_manager, _height+2, _width+2, minRelRow(_height, _regionSize), maxRelRow(_width, _height, _regionSize), var),

    boundary (boundaryUnderlay.view   (1, 1, _height+1, _width+1)),
    actionRow(actionRowUnderlay.view(1, 1, _height+1, _width+1)),
    actionCol(actionColUnderlay.view(1, 1, _height+1, _width+1)),

    objTuple(
        boundaryUnderlay,
        teeTeeNeighbor,
        teeBlankNeighbor,
        hSpacing,
        vSpacing,
        actionRowUnderlay,
        actionColUnderlay)
{
}

// Require a different set of marker regions from the current.
Clause MarkerRegionsImpl::diffSolnReq() const {
  Clause diffSoln;
  for ( size_type i = 0; i < height; i++ ) {
    for ( size_type j = 0; j < width; j++ ) {
      diffSoln |= (boundary[i][j].modelValue() == BLANK ? boundary[i][j] != BLANK : boundary[i][j] == BLANK);
    }
  }
  return diffSoln;
}

// Because this function template is private, it is appropriate
// to place its definition in this source file.
Requirement MarkerRegionsImpl::typeRequirement() const {
  Requirement result;
  // Upper-left-hand corner cell is always part of the marker region with coordinates 0,0.
  result &= actionRow[0][0] == 0;
  result &= actionCol[0][0] == 0;

  for ( size_type i = 1; i < height+1; i++ ) {
    for ( size_type j = 1; j < width+1; j++ ) {
      // If a cell is a horizontal boundary, its two horizontal neighbors are boundaries,
      // and its two vertical neighbors are blank
      Requirement horizontalReq;
      horizontalReq &= boundaryUnderlay[i+1][j  ] == BLANK;
      horizontalReq &= boundaryUnderlay[i-1][j  ] == BLANK;
      horizontalReq &= boundaryUnderlay[i  ][j+1] != BLANK & boundaryUnderlay[i  ][j+1] != VERTICAL;
      horizontalReq &= boundaryUnderlay[i  ][j-1] != BLANK & boundaryUnderlay[i  ][j-1] != VERTICAL;
      result &= implication(boundaryUnderlay[i][j] == HORIZONTAL, move(horizontalReq));

      // If a cell is a vertical boundary, its two vertical neighbors are boundaries,
      // and its two horizontal neighbors are blank
      Requirement verticalReq;
      verticalReq &= boundaryUnderlay[i+1][j  ] != BLANK & boundaryUnderlay[i+1][j  ] != HORIZONTAL;
      verticalReq &= boundaryUnderlay[i-1][j  ] != BLANK & boundaryUnderlay[i-1][j  ] != HORIZONTAL;
      verticalReq &= boundaryUnderlay[i  ][j+1] == BLANK;
      verticalReq &= boundaryUnderlay[i  ][j-1] == BLANK;
      result &= implication(boundaryUnderlay[i][j] == VERTICAL, move(verticalReq));

      // If a cell is a cross, all of its neighbors are boundaries
      Requirement crossReq;
      crossReq &= boundaryUnderlay[i+1][j  ] == VERTICAL;
      crossReq &= boundaryUnderlay[i-1][j  ] == VERTICAL;
      crossReq &= boundaryUnderlay[i  ][j+1] == HORIZONTAL;
      crossReq &= boundaryUnderlay[i  ][j-1] == HORIZONTAL;
      result &= implication(boundaryUnderlay[i][j] == CROSS, move(crossReq));

      // If a cell is a tee, it has one tee neighbor, one blank neighbor, and two boundary neighbors
      Requirement teeReq;
      teeReq &= teeBlankNeighbor[i][j] != teeTeeNeighbor[i][j];
      teeReq &= equivalence(teeBlankNeighbor[i][j] == UP,    boundaryUnderlay[i+1][j  ] == BLANK);
      teeReq &= equivalence(teeBlankNeighbor[i][j] == DOWN,  boundaryUnderlay[i-1][j  ] == BLANK);
      teeReq &= equivalence(teeBlankNeighbor[i][j] == RIGHT, boundaryUnderlay[i  ][j+1] == BLANK);
      teeReq &= equivalence(teeBlankNeighbor[i][j] == LEFT,  boundaryUnderlay[i  ][j-1] == BLANK);
      teeReq &= equivalence(boundaryUnderlay[i+1][j  ] == TEE, teeTeeNeighbor[i][j] == UP);
      teeReq &= equivalence(boundaryUnderlay[i-1][j  ] == TEE, teeTeeNeighbor[i][j] == DOWN);
      teeReq &= equivalence(boundaryUnderlay[i  ][j+1] == TEE, teeTeeNeighbor[i][j] == RIGHT);
      teeReq &= equivalence(boundaryUnderlay[i  ][j-1] == TEE, teeTeeNeighbor[i][j] == LEFT);
      result &= implication(boundaryUnderlay[i][j] == TEE, move(teeReq));

      // hSpacing or and vSpacing are zero iff the corresponding boundary type is "non-blank"
      result &= equivalence(hSpacing[i][j] == 0, boundaryUnderlay[i][j] != BLANK);
      result &= equivalence(vSpacing[i][j] == 0, boundaryUnderlay[i][j] != BLANK);

      // The following rules only inside each region (no requirement is placed on vertical boundaries)
      // In most cases, hSpacing increases by one as you go right.
      // However, if hSpacing is regionSize-1 or regionSize, then it can either increase (up to a maximum of regionSize) or
      //  reset to 0 as you go right.
      Requirement hSpacingReq;
      hSpacingReq &= implication(hSpacing[i][j] != 0, hSpacing[i][j]-1 == hSpacing[i][j-1]);
      hSpacingReq &= implication(hSpacing[i][j] == 0, hSpacing[i][j-1] == regionSize-1 | hSpacing[i][j-1] == regionSize);
      result &= implication(boundaryUnderlay[i][j] == BLANK | boundaryUnderlay[i][j-1] == BLANK, move(hSpacingReq));

      // Similar to hSpacing
      Requirement vSpacingReq;
      vSpacingReq &= implication(vSpacing[i][j] != 0, vSpacing[i][j]-1 == vSpacing[i-1][j]);
      vSpacingReq &= implication(vSpacing[i][j] == 0, vSpacing[i-1][j] == regionSize-1 | vSpacing[i-1][j] == regionSize);
      result &= implication(boundaryUnderlay[i][j] == BLANK | boundaryUnderlay[i-1][j] == BLANK, move(vSpacingReq));

      // Effect of the boundaries on the rowAction.
      // FIXME this implementation leaves inaccuracies at the edge of actionRow
      result &= implication(boundaryUnderlay[i][j] == BLANK | boundaryUnderlay[i][j] == VERTICAL,
          actionRowUnderlay[i-1][j  ] == actionRowUnderlay[i][j] &
          actionRowUnderlay[i  ][j-1] == actionRowUnderlay[i][j]);
      result &= implication(boundaryUnderlay[i][j] == HORIZONTAL,
          actionRowUnderlay[i-1][j  ] == actionRowUnderlay[i][j] - 1 &
          actionRowUnderlay[i  ][j-1] == actionRowUnderlay[i][j]);

      // Effect of the boundaries on the colAction.
      // FIXME this implementation leaves inaccuracies at the edge of actionCol
      result &= implication(boundaryUnderlay[i][j] == BLANK | boundaryUnderlay[i][j] == HORIZONTAL,
          actionColUnderlay[i-1][j  ] == actionColUnderlay[i][j] &
          actionColUnderlay[i  ][j-1] == actionColUnderlay[i][j]);
      result &= implication(boundaryUnderlay[i][j] == VERTICAL,
          actionColUnderlay[i-1][j  ] == actionColUnderlay[i][j] &
          actionColUnderlay[i  ][j-1] == actionColUnderlay[i][j] - 1);
    }
  }

  return result;
}

// Plots entailment in the markerRegions.  That is: what must be in the boundary,
// what must not be in the boundary, and what could be or not.
function<ostream& (ostream&)> MarkerRegionsImpl::yesNoMaybe(const DualClause& clause) const {
  return [this,clause] (ostream& out) -> ostream& {

    for ( size_type i = 0; i < height; i++ ) { // Rows
      out << "    ";
      for ( size_type j = 0; j < width; j++ ) { // Columns
        if        ( !manager->solve(clause & boundary[i][j] != BLANK ) ) {
          out << 0 << " ";
        } else if ( !manager->solve(clause & boundary[i][j] == BLANK ) ) {
          out << 1 << " ";
        } else {
          out << -1 << " ";
        }

      } // Columns
      out << endl;
    } // Rows

    return out;

  }; // Lambda function
}

// Plots entailment in the markerRegions.  That is: what must be in the boundary,
// what must not be in the boundary, and what could be or not.
function<ostream& (ostream&)> MarkerRegionsImpl::yesNoMaybeRedBlue(const DualClause& clause, Lit redLit, Lit blueLit) const {
  const auto GRAY = -1;
  const auto WHITE = 0;
  const auto BLACK = 1;
  const auto BLUE  = 2;
  const auto RED   = 3;
  return [this,clause,redLit,blueLit] (ostream& out) -> ostream& {

    for ( size_type i = 0; i < height; i++ ) { // Rows
      out << "    ";
      for ( size_type j = 0; j < width; j++ ) { // Columns
        if        ( !manager->solve(clause & boundary[i][j] != BLANK ) ) {
          out << WHITE << " ";
        } else if ( !manager->solve(clause & boundary[i][j] == BLANK ) ) {
          out << BLACK << " ";
        } else if ( !manager->solve(clause & redLit  & boundary[i][j] == BLANK ) ) {
          out << RED << " ";
        } else if ( !manager->solve(clause & blueLit & boundary[i][j] == BLANK ) ) {
          out << BLUE << " ";
        } else {
          out << GRAY << " ";
        }
      } // Columns
      out << endl;
    } // Rows

    return out;

  }; // Lambda function
}


function<ostream& (ostream&)> MarkerRegionsImpl::yesNoMaybe() const {
  return yesNoMaybe(DualClause());
}

// Output operator
ostream& operator<<(ostream& out, const MarkerRegionsImpl& mr) {
  for ( MarkerRegionsImpl::size_type i = 0; i < mr.boundary.height(); i++) {
    out << "    ";
    for ( MarkerRegionsImpl::size_type j = 0; j < mr.boundary.width(); j++) {
      out << (mr.boundary[i][j].modelValue() == MarkerRegionsImpl::BLANK ? "0 " : "1 ");
    }
    out << newl;
  }
  return out << flush;
}
