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
#include <map>
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

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

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

void printProgress(ostream& out, const map<pair<int,int>, char>& progress, int maxSize) {
  const int maxQ = 80;
  out << "    ";
  for ( int q = 2; q < maxQ; q++ ) {
    cout << (q/10)%10 << ' ';
  }
  cout << endl;

  out << "    ";
  for ( int q = 2; q < maxQ; q++ ) {
    cout << q%10 << ' ';
  }
  cout << endl;

  
  for ( int p = 2; p*p*2 <= maxSize; p++ ) {
    cout << setw(3) << setfill('0') << p << ' ';
    for ( int q = 2; q < maxQ; q++ ) {
      auto key = make_pair(p,q);
      if ( progress.find(key) != progress.end() ) {
	cout << progress.at(key) << ' ';
      } else {
	cout << "  ";
      }
    }
    cout << endl;
  }
}

/////////////////////////////////////////////////////////////////////
//
//  main
//
int main (int argc, char** argv) {
  map<pair<int,int>, char> progress;
  for ( int size = 8; size < 10000; size++ ) {
    for ( int p = 2; p*p < size; p++ ) {
      // Want (p*p*q == size). That's how we order this sanely. 
      if ( size % (p*p) != 0 ) {
	continue;
      }
      int q = size/(p*p);

      // Want p,q to be coprime
      if ( gcd(p,q) != 1 ) {
	continue;
      }
 
	
      MinisatSolver solver;

      cout << timestamp << " Establishing constraints for p=" << p << ", q=" << q << endl;
      cout << timestamp << " p*p*q = " << size << endl;
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

      if ( solver.solve(std::chrono::seconds(5)) ) {
	cout << timestamp << " " << p << " " << q << " SATISFIABLE" << endl;
	cout << "Solution: " << endl;
	cout << puzzle << endl;
	progress[make_pair(p,q)] = 'S';
      } else if ( solver.interrupted() ) {
	cout << timestamp << " " << p << " " << q << " TIMEOUT" << endl;
	progress[make_pair(p,q)] = '?';
      } else {
	cout << timestamp << " " << p << " " << q << " UNSATISFIABLE" << endl;
	progress[make_pair(p,q)] = '.';
      }
      printProgress(cout, progress, size);
      cout << endl;
    }

  }
}

