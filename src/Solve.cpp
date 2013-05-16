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

  const unsigned dim = 14;

  Matrix<Ordinal>  mat  (&manager, dim, dim, -1, 2);
  Matrix<Ordinal>  mat2 (&manager, dim, dim, -20, 21);
  Ordinal          gamma(&manager, -20, 21); // The updated value

  // mat should be an all-pairs shortest paths matrix already.  This
  // is easier to require directly than to use Floyd-Warshall
  manager.require(isShortestPathsMatrix(mat));

  // Gamma must strictly refine mat_01, mat_23, etc.
  for ( int i = 0; i < dim/2; i++ ) {
    manager.require(gamma < mat[i][i+1] & gamma >= -mat[i+1][i]);
  }

  // The two matrices must be equal, except for the gamma entries
  for ( int i = 0; i < dim; i++ ) {
    for ( int j = 0; j < dim; j++ ) {
      // Every pair like (0,1), (2,3), (4,5) should be set to gamma.
      if ( i%2 == 0 && j == i+1 ) {
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
  Clause hasNegativeCycles;
  for ( int i = 0; i < dim; i++ ) {
    hasNegativeCycles |= fw[i][i] < 0;
  }
  manager.require(hasNegativeCycles);

  cout << timestamp << " Constraints established." << endl;

  int count = 0;
  while ( manager.solve() && count < 1000) {
    cout << timestamp << " Satisfiable" << endl;
    cout << "Solution number " << count++ << endl;
    cout << "Gamma: " << gamma.modelValue() << endl;
    cout << endl;
    cout << mat << endl;
    cout << "-----------------------------------------------------" << endl;
    manager.require(mat.diffSolnReq());
  }

  return 0;
}
