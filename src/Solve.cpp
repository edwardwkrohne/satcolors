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

  const int width = 30; // 30
  const int height = 71; // 71

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

  // Get some coprime periodicities on the sides to make it more interesting.
  for ( int row = 0; row < height-5; row++ ) {
    manager.require(morphism[row][0] == morphism[row+5][0]);
  }
  for ( int row = 0; row < height-7; row++ ) {
    manager.require(morphism[row][width-1] == morphism[row+7][width-1]);
  }

  for ( int col = 0; col < width; col++ ) {
      manager.require(morphism[0][col] == morphism[height-1][col]);
  }

  cout << timestamp << " Graph-coloring constraints established.  Solving." << endl;

  // See if the problem is solvable at all
  if ( !manager.solve() ) {
    cout << timestamp << " UNSATISFIABLE" << endl;
  }

  cout << timestamp << " initial solution found.  Optimizing." << endl;
  
  // Location of a  cell that must be at most 10.
  Cardinal reqRow(&manager, 0, height);
  Cardinal reqCol(&manager, 0, width);
  manager.require(morphism[reqRow][reqCol] <= 10);

  cout << timestamp << " Optimization constraints established.  Beginning solve loop." << endl;

  // Now try and force specific cells to be at most 10
  do {
    cout << timestamp << " Solution found" << endl;
    fout << height << " " << width << endl;
    fout << morphism << endl << endl;

    // Also make sure that some new cell must be at most ten.
    for ( int row = 0; row < height; row++ ) {
      for ( int col = 0; col < width; col++ ) {
	auto val = morphism[row][col].modelValue();
	if ( val <= 10 ) {
	  // Any cell that's already at most ten, lock it in.
	  manager.require(morphism[row][col] == val);

	  // Also make sure that some new cell must be at most ten.
	  manager.require(reqRow != row | reqCol != col);
	}
      }
    }
  } while ( manager.solve() );

  return 0;
}
