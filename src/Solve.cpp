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
#include "ordinal.h"
#include "matrix.h"
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

// Implementation of Floyd-Warshall all pairs shortest paths in SAT
Matrix<Ordinal> floydWarshall(SolverManager* manager, 
			      const Matrix<Ordinal>& mat, 
			      Minisat::Var& var = SolverManager::allocateNew) {
  if ( mat.width() != mat.height() ) {
    throw exception();
  }
  auto dim = mat.width();
  auto elemMin = mat[0][0].min();
  auto elemMax = mat[0][0].max();

  Matrix<Ordinal> oldMat = mat;
  for ( int k = 0; k < dim; k++ ) {
    Matrix<Ordinal> newMat(manager,dim,dim,elemMin,elemMax, var); 
    for ( int i = 0; i < dim; i++ ) {
      for ( int j = 0; j < dim; j++ ) {
	// The following implements M_ij = min(M_ij, M_ik + M_kj) in SAT
	Ordinal sum(manager, elemMin, elemMax, var);
	manager->require( sum == oldMat[i][k] + oldMat[k][j] );
	manager->require( newMat[i][j] == min(sum, oldMat[i][j]) );
      }
    }
    
    oldMat = move(newMat);
  }

  return move(oldMat); // the last newMat
}

Requirement isShortestPathsMatrix(const Matrix<Ordinal>& mat) {
  if ( mat.width() != mat.height() ) {
    throw exception();
  }
  auto dim = mat.width();

  Requirement result;
  for ( int i = 0; i < dim; i++ ) {
    for ( int j = 0; j < dim; j++ ) {
      for ( int k = 0; k < dim; k++ ) {
	result &= mat[i][k] <= mat[i][j] + mat[j][k];
      }
    }
  }

  for ( int i = 0; i < dim; i++ ) {
    result &= mat[i][i] == 0;
  }

  return result;
}

int main (int argc, char** argv) {
  SolverManager manager;

  Matrix<Ordinal>  mat  (&manager, 4, 4, -1, 2);
  Matrix<Ordinal>  mat2 (&manager, 4, 4, -20, 21);
  Ordinal          gamma(&manager, -3, 4); // The updated value

  // mat should be an all-pairs shortest paths matrix already.  This
  // is easier to require directly than to use Floyd-Warshall
  manager.require(isShortestPathsMatrix(mat));

  // Gamma must strictly refine both mat_01 and mat_23.
  manager.require(gamma < mat[0][1] & gamma >= -mat[1][0]);
  manager.require(gamma < mat[2][3] & gamma >= -mat[3][2]);

  // The two matrices must be equal, except for gamma
  for ( int i = 0; i < 4; i++ ) {
    for ( int j = 0; j < 4; j++ ) {
      if      ( i == 0 && j == 1 )
	manager.require(mat2[i][j] == gamma);
      else if ( i == 2 && j == 3 ) {
	manager.require(mat2[i][j] == gamma);
      } else {
	manager.require(mat2[i][j] == mat[i][j]);
      }      
    }
  }

  // Compute a new all pairs shortest paths on mat2, and call that fw.
  auto fw = floydWarshall(&manager, mat2);

  // Require further that the addition of gamma in both places
  // introduces a negative cycle; do this by detecting nonzero
  // diagonal entries on the shortest-path matrix.
  manager.require(fw[0][0] < 0 |
		  fw[1][1] < 0 |
		  fw[2][2] < 0 |
		  fw[3][3] < 0);

  int count = 0;
  while ( manager.solve()) {
    cout << timestamp << " Satisfiable" << endl;
    cout << "Solution number " << count++ << endl;
    cout << "Gamma: " << gamma.modelValue() << endl;
    cout << endl;
    cout << mat << endl;
    cout << fw << endl;
    cout << "-----------------------------------------------------" << endl;
    manager.require(mat.diffSolnReq());
  }

  return 0;
}
