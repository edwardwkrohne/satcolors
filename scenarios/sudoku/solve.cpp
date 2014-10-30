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
#include "../../src/cardinal.h"
#include "../../src/matrix.h"
#include "../../src/minisatsolver.h"
#include "../../src/manipulators.h"
using namespace std;


/////////////////////////////////////////////////////////////////////
//
//  main
//
int main (int argc, char** argv) {
  MinisatSolver solver;

  if ( argc < 2 ) {
    cerr << "Please specify sudoku data file" << endl;
    return 1;
  }

  ifstream fin(argv[1]);
  if ( !fin ) {
    cerr << "Error opening file " << argv[1] << endl;
    return 1;
  }

  cout << timestamp << " Establishing puzzle constraints." << endl;
  
  // Start with a 9x9 grid of numbers 1 through 9 (i.e., numbers in [1,10) )
  Matrix<Cardinal> puzzle(&solver, 
			  9, 9, 
			  1, 10);

  // Read in the puzzle
  for ( int row = 0; row < 9; row++ ) {
    for ( int col = 0; col < 9; col++ ) {
      int elem;
      fin >> elem;
      if ( !fin ) {
	cerr << "Error reading file." << endl;
	return 1;
      }
      if ( elem == 0 ) {
	continue;
      } else if ( elem <= 9 ) {
	solver.require(puzzle[row][col] == elem);
      } else {
	cerr << "Unexpected input '" << elem << "' in sudoku puzzle" << endl;
	return 1;
      }
    }
  }

  // In any row, no two elements may be equal
  for ( int row = 0; row < 9; row++ ) {
    for ( int elem1 = 0; elem1 < 9; elem1++ ) {
      for ( int elem2 = elem1+1; elem2 < 9; elem2++ ) {
	solver.require(puzzle[row][elem1] != puzzle[row][elem2]);
      }
    }
  }

  // In any column, no two elements may be equal
  for ( int col = 0; col < 9; col++ ) {
    for ( int elem1 = 0; elem1 < 9; elem1++ ) {
      for ( int elem2 = elem1+1; elem2 < 9; elem2++ ) {
	solver.require(puzzle[elem1][col] != puzzle[elem2][col]);
      }
    }
  }
  
  // In any 3x3 square, no two elements may be equal
  for ( int boxRow = 0; boxRow < 9; boxRow += 3 ) {
    for ( int boxCol = 0; boxCol < 9; boxCol += 3 ) {
      for ( int boxIdx1 = 0; boxIdx1 < 9; boxIdx1++ ) {
	for ( int boxIdx2 = boxIdx1+1; boxIdx2 < 9; boxIdx2++ ) {
	  int row1 = boxRow + boxIdx1/3;
	  int col1 = boxCol + boxIdx1%3;
	  int row2 = boxRow + boxIdx2/3;
	  int col2 = boxCol + boxIdx2%3;
	  solver.require(puzzle[row1][col1] != puzzle[row2][col2]);
	}
      }
    }
  }

  cout << timestamp << " Puzzle constraints established.  Solving." << endl;

  // See if the problem is solvable
  if ( !solver.solve() ) {
    cout << timestamp << " UNSATISFIABLE" << endl;
    return 1;
  } else {  
    cout << timestamp << " Solution found: " << endl;
    cout << puzzle << endl;
    return 0;
  }
}
