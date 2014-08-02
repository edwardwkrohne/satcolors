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
#include "ordinal.h"
#include "matrix.h"
#include "pairindexedscalar.h"
#include "solvermanager.h"
#include "markerregions.h"
#include "manipulators.h"
#include "almostlines.h"
#include "ordinaladdexpr.h"
#include "ordinalminexpr.h"
using namespace std;

using Minisat::Var;
using Minisat::Lit;
using Minisat::mkLit;

/////////////////////////////////////////////////////////////////////
//
//  main
//
int main (int argc, char** argv) {
  // Get arguments
  if ( argc < 3 ) {
    cerr << "Insufficient arguments." << endl;
    cerr << argv[0] << " <inputfile.mtx> <outputfile.sltn>" << endl;
    exit(1);
  }
  ifstream fin(argv[1]);
  ofstream fout(argv[2]);

  SolverManager manager;

  const int topPeriod = 3;
  const int bottomPeriod = 7;
  const int width = 65;
  const int height = 20;
  const int highColor = 1; // used when the code tries to "simplify" the image

  // Read the incidence matrix
  cout << timestamp << " Reading incidence matrix" << endl;
  int order;
  fin >> order;
  cout << "Order is " << order << endl;
  bool incidences[order][order];
  for ( int row = 0; row < order; row++ ) {
    for ( int col = 0; col < order; col++ ) {
      if ( !fin ) {
	throw std::runtime_error("Error reading incidence matrix from stream. "
				 "Corrupted file? "
				 "Incorrect dimension specification? "
				 "Non-integer values?");
      }
      fin >> incidences[row][col];
    }
  }

  // Establish the constraints
  cout << timestamp << " Establishing basic morphism constraints." << endl;
  Matrix<Cardinal> morphism(&manager, height, width, 0, order);

  cout << timestamp << " Basic morphism constraints established." << endl;
  cout << timestamp << " Establishing graph coloring constraints." << endl;

  // Establish horizontally oriented constraints
  for ( int row = 0; row < height; row++ ) {
    for ( int col = 0; col < width-1; col++ ) {
      for ( int thisColor = 0; thisColor < order; thisColor++ ) {
	Clause rightClause;
	Clause leftClause;
	for ( int otherColor = 0; otherColor < order; otherColor++ ) {
	  if ( incidences[thisColor][otherColor] ) {
	    rightClause |= morphism[row][col+1] == otherColor;
	    leftClause  |= morphism[row][col]   == otherColor;
	  }
	}
	manager.require(implication(morphism[row][col]   == thisColor, rightClause));
	manager.require(implication(morphism[row][col+1] == thisColor, leftClause));
      }
    }
  }

  // Establish vertically oriented constraints
  for ( int row = 0; row < height-1; row++ ) {
    for ( int col = 0; col < width; col++ ) {
      for ( int thisColor = 0; thisColor < order; thisColor++ ) {
	Clause downClause;
	Clause upClause;
	for ( int otherColor = 0; otherColor < order; otherColor++ ) {
	  if ( incidences[thisColor][otherColor] ) {
	    downClause |= morphism[row+1][col] == otherColor;
	    upClause   |= morphism[row]  [col] == otherColor;
	  }
	}
	manager.require(implication(morphism[row]  [col] == thisColor, downClause));
	manager.require(implication(morphism[row+1][col] == thisColor, upClause));
      }
    }
  }

  // // Make the grid periodic horizontally.
  // for ( int row = 0; row < height; row++ ) {
  //   for ( int col = 0; col < width; col++ ) {
  //     manager.require(morphism[row][col] == morphism[row][col+width]);
  //   }
  // }

  // Make the top and bottom of the grids periodic with a much tighter period
  for ( int col = 0; col < width-topPeriod; col++ ) {
    manager.require(morphism[0][col] == morphism[0][col+topPeriod]);
  }

  // Make the top and bottom of the grids periodic with a much tighter period
  for ( int col = 0; col < width-bottomPeriod; col++ ) {
    manager.require(morphism[height-1][col] == morphism[height-1][col+bottomPeriod]);
  }

  for ( int col = 0; col < width; col++ ) {
    for ( int row = 3; row < height-3; row++ ) {
      manager.require(morphism[row][col] <= highColor);
    }
  }

  for ( int col = 0; col < width; col++ ) {
    manager.require(morphism[0][col] <= 2);
    manager.require(morphism[height-1][col] <= 2);
  }

  cout << timestamp << " constraints established.  Solving." << endl;

  // See if the problem is solvable at all
  if ( !manager.solve() ) {
    cout << timestamp << " UNSATISFIABLE" << endl;
    return 0;
  }

  fout << height << " " << width << endl;
  fout << morphism << endl << endl;

  cout << timestamp << " initial solution found.  Optimizing." << endl;

  return 0;

  // Location of a  cell that must be at most highColor.
  Cardinal reqRow(&manager, 1, height-1);
  Cardinal reqCol(&manager, 0, width);
  manager.require(morphism[reqRow][reqCol] <= highColor);

  cout << timestamp << " Optimization constraints established.  Beginning solve loop." << endl;
  // Now try and force specific cells to be at most highColor
  while ( true ) {
    for ( int row = 1; row < height-1; row++ ) {
      for ( int col = 0; col < width; col++ ) {
  	auto val = morphism[row][col].modelValue();
  	if ( val <= highColor ) {
	  manager.require(morphism[row][col] <= highColor);

  	  // Also make sure that some new cell must be at most highcolor.
  	  manager.require(reqRow != row | reqCol != col);
  	}
      }
    }

    if ( !manager.solve() ) {
      break;
    }

    cout << timestamp << " Solution found" << endl;
    fout << height << " " << width << endl;
    fout << morphism << endl << endl;
  }

  cout << timestamp << " All solutions found." << endl;

  return 0;
}
