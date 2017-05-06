// Solve.cpp
//
// Sets up a MiniSat solver instance and a simple problem, and solves it.

#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <array>
#include <tuple>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <set>
#include "../../src/cardinal.h"
#include "../../src/matrix.h"
#include "../../src/matrixview.h"
#include "../../src/pairindexedscalar.h"
#include "../../src/minisatsolver.h"
#include "../../src/manipulators.h"
#include "../../src/twelvetiles.h"
using namespace std;

enum CellType {
  OTHER,
  BIG_CORNER,
  SMALL_CORNER,
  TOP_EDGE,
  LEFT_EDGE,
  NUM_CELL_TYPES
};

template<typename ViewType>
DualClause getBoxNonoverlapRequirement(ViewType& view, size_t boxSize) {
  DualClause req;
  for (int row = 1; row < boxSize; row++) {
    req &= view[row][0] == LEFT_EDGE;
  }

  for (int col = 1; col < boxSize; col++) {
    req &= view[0][col] == TOP_EDGE;
  }
  
  for (int row = 1; row < boxSize; row++) {
    for (int col = 1; col < boxSize; col++) {
      req &= view[row][col] == OTHER;
    }
  }
  return req;
}

template<typename ViewType>
Clause getBoxCoverageRequirement(ViewType& view, size_t boxSize, CellType reqCellType) {
  Clause req;
  for (int row = view.height()-boxSize; row < view.height(); row++) {
    for (int col = view.width()-boxSize; col < view.width(); col++) {
      req |= view[row][col] == reqCellType;
    }
  }
  return req;
}

/////////////////////////////////////////////////////////////////////
//
//  main
//
int main (int argc, char** argv) {
  set<pair<int,int>> finishedPairs;
  for ( int sizeLimit = 6; sizeLimit < 10000; sizeLimit++ ) {
    for ( int p = 2; p < sizeLimit; p++ ) {
      for ( int q = 2; q < sizeLimit; q++ ) {
	if( p*p*q > sizeLimit ) {
	  break;
	}
	if (finishedPairs.find(make_pair(p,q)) != finishedPairs.end()) {
	  continue;
	}

	int skipPq = false;
	for ( int factor = 2; factor <= q || factor <= p; factor++ ) {
	  if ( p %factor == 0 && q % factor == 0 ) {
	    skipPq = true;
	  }
	}
	if ( skipPq ) {
	  continue;
	}

	finishedPairs.insert(make_pair(p,q));
	
	MinisatSolver solver;

	cout << timestamp << " Establishing constraints for p=" << p << ", q=" << q << endl;
	cout << timestamp << " Size limit is " << sizeLimit << endl;
	TwelveTiles<Cardinal> puzzle(&solver,
				     p,q, // p,q
				     3,     // n
				     NUM_CELL_TYPES);   // depth

  
	for ( auto view : puzzle ) {
	  solver.require(implication(view[0][0] == BIG_CORNER,   getBoxNonoverlapRequirement(view, 3)));
	  solver.require(implication(view[0][0] == SMALL_CORNER, getBoxNonoverlapRequirement(view, 2)));

	  solver.require(getBoxCoverageRequirement(view, 2, SMALL_CORNER) |
			 getBoxCoverageRequirement(view, 3, BIG_CORNER));
	}

	cout << timestamp << " Constraints established" << endl;

	int numSolns = 5;

	if ( solver.solve() ) {
	  cout << timestamp << " Solution found: " << endl;
	  cout << puzzle << endl;
	} else {
	  cout << timestamp << " UNSATISFIABLE searching for solution." << endl;
	}
	cout << endl;
      }

    }
  }
}
