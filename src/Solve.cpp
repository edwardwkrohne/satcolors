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

using namespace std;

using Minisat::Var;
using Minisat::Lit;
using Minisat::mkLit;

int main (int argc, char** argv) {
  SolverManager manager;
  vector<Index> testVec;

  testVec.emplace_back(manager, 0, 5, SolverManager::allocateNew);

  return 0;
}
