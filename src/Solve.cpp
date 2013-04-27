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
#include "matrix.h"
#include "solvermanager.h"
#include "markerregions.h"
#include "manipulators.h"

using namespace std;

using Minisat::Var;
using Minisat::Lit;
using Minisat::mkLit;

void getOffsets(int argc, char** argv, int& rowOffset, int& colOffset ) {
  if ( argc != 3 ) {
    cerr << "Two integer arguments required, " << argc-1 << " provided" << endl;
    exit(EXIT_FAILURE);
  }

  istringstream strin(string(argv[1]) + " " + string(argv[2]));
  strin >> rowOffset >> colOffset;

  if ( !strin ) {
    cerr << "Error processing arguments" << endl;
    exit(EXIT_FAILURE);
  }
}

string getGnuplotName(int rowOffset, int colOffset) {
  string str;
  ostringstream strout(str);
  strout << "plot"
      << setw(2) << setfill('0') << rowOffset << "-"
      << setw(2) << setfill('0') << colOffset << ".gnu" << endl;
  return str;
}

int main (int argc, char** argv) {
  int rowOffset;
  int colOffset;
  getOffsets(argc, argv, rowOffset, colOffset);

  SolverManager manager;

  MarkerRegions mr(manager, 60, 60, 5);
  Lit pinch = mkLit(manager.newVars(1));

  manager.require(implication(pinch, mr.actionRow[10][10] + 2 == mr.actionRow[18][20]));
  manager.require(implication(pinch, mr.actionCol[10][10] + 2 == mr.actionCol[18][20]));

  for ( int i = 1; i < mr.height-1; i++ ) {
    for ( int j = 1; j < mr.width-1; j++ ) {
      if ( !manager.solve(mr.actionRow[i][j] == rowOffset & mr.actionCol[i][j] == colOffset) ) {
        cout << " 0 ";
      } else if ( !manager.solve(mr.actionRow[i][j] == rowOffset & mr.actionCol[i][j] == colOffset & pinch) ) {
        cout << "-1 ";
      } else {
        cout << " 1 ";
      }
      cout << flush;
    }
    cout << endl;
  }

  return 0;
}
