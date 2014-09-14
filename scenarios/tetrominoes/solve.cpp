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
#include "../../src/matrixview.h"
#include "../../src/pairindexedscalar.h"
#include "../../src/solvermanager.h"
#include "../../src/manipulators.h"
using namespace std;

using Minisat::Var;

const string pieces[5][2] = {
  {
    "XX  ", // Box
    "XX  ",
  }, 
  {
    "XXX ", // Tee
    " X  ",
  }, 
  {
    "XXXX", // Straight
    "    ",
  }, 
  {
    "XXX ", // Ell
    "X   ",
  }, 
  {
    "XX  ", // Skew
    " XX ",
  },
};
/////////////////////////////////////////////////////////////////////
//
//  main
//
int main (int argc, char** argv) {
  SolverManager manager;

  cout << timestamp << " Establishing constraints" << endl;

  // Start with a 5x8 grid of numbers 0 through 9 (i.e., numbers in
  // [0,10) ) Each color will represent a different tetromino; we'll
  // have two each of the five free tetrominoes.
  Matrix<Cardinal> puzzle(&manager, 
			  5, 8, 
			  0, 10);

  // Setup requirements for each piece
  for ( int piece = 0; piece < 10; piece++ ) {
    // "row" and "col" refer to the upper-left corner of the piece
    // we're placing.  The bounds, [0,9) large enough to express all
    // positions in a 5x8 (or 8x5 as below) grid.
    Cardinal row(&manager,0,9);
    Cardinal col(&manager,0,9);

    // "transform" selects which transform of the piece is being used.
    Cardinal transform(&manager,0,8);

    // Loop through all the possible transforms and set up the requisite condition for each.
    for ( int transCount = 0; transCount < 8; transCount++ ) {
      //  Transforming the board is easier than transforming the
      //  pieces, so we transform the board.
      auto view = (transCount & 0x1) ? puzzle.reflectV() : puzzle;
      view      = (transCount & 0x2) ? view.reflectH()   : view;
      view      = (transCount & 0x4) ? view.transpose()  : view;

      // Build the requirement for the given piece to be in the given position
      Requirement req;

      // i and j index individual squares of the piece; a piece is an
      // array of two strings of length 4, hence the bounds on i and j
      for ( int i = 0; i < 2; i++ ) {
	for ( int j = 0; j < 4; j++ ) {
	  if ( pieces[piece/2][i][j] == 'X' ) {
	    req &= view[row+i][col+j] == piece;
	  }
	}
      }

      manager.require(implication(transform == transCount, req));
    }
  }

  cout << timestamp << " Constraints established" << endl;

  int numSolns = 5;

  // See if the problem is solvable
  for ( int i = 0; i < numSolns; i++ ) {
    if ( manager.solve() ) {
      cout << timestamp << " Solution " << i << " found: " << endl;
      cout << puzzle << endl;
      
      // Repeatedly require new solutions
      manager.require(puzzle.diffSolnReq());
    }
    else {
      cout << timestamp << " UNSATISFIABLE searching for solution " << i << "." << endl;
      return 1;
    }
  }

  cout << timestamp << " Done.  All " << numSolns << " solutions found." << endl;
}
