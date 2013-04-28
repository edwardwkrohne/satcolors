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
using namespace std;

using Minisat::Var;
using Minisat::Lit;
using Minisat::mkLit;

int main (int argc, char** argv) {
  SolverManager manager;

  cout << timestamp << " Establishing requirements." << endl;

  AlmostLines lines(manager, 70, 70, 5, 8);

  cout << timestamp << " Requirements established." << endl;

  if ( manager.solve() ) {
    cout << timestamp << " Satisfiable" << endl;
    cout << lines;
  } else {
    cout << timestamp << " Unsatisfiable" << endl;
  }

  return 0;
}
