// -*- Mode: C++ -*-
////////////////////////////////////////////////////////////////////////////
//
//  satcolors -- for writably and readably building problems for SAT-solving
//  Copyright (C) 2014 Edward W. Krohne III
//
//  The research that led to this product was partially supported by the
//  U.S. NSF grants DMS-0943870 and DMS-1201290.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "mocksolver.h"
#include "testglue.h"
#include "../src/minisatsolver.h"
#include "../src/matrix.h"
#include "../src/matrixview.h"
#include "../src/pairindexedscalar.h"
#include "../src/twelvetiles.h"

using namespace std;

class TwelveTilesTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(TwelveTilesTest);
  CPPUNIT_TEST(testTilesLinked);
  CPPUNIT_TEST(testIterateViews);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testTilesLinked(void);
  void testIterateViews(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( TwelveTilesTest );
void TwelveTilesTest::testTilesLinked(void) {
  // Object under test.
  MinisatSolver solver;
  TwelveTiles<> tt(&solver, 3, 5, 2, 10);
  
  ASSERT_SAT(solver);


  // Check that Tacac is linked as a torus
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tacac[1][1] == 0) & (tt.Tacac[4][4] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tacac[1][4] == 0) & (tt.Tacac[4][1] == 1), tt);

  // Check that Tacac and Tbcbc are linked
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tacac[0][2] == 0) & (tt.Tbcbc[0][2] == 1), tt);
  ASSERT_SAT_ASSUMP(solver, (tt.Tacac[2][0] == 0) & (tt.Tbcbc[2][0] == 1));

  // Check that Tbcbc's sides are linked
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tbcbc[2][0] == 0) & (tt.Tbcbc[2][3] == 1), tt);

  // Check that Tacac and Tadad are linked
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tacac[2][0] == 0) & (tt.Tadad[2][0] == 1), tt);
  ASSERT_SAT_ASSUMP(solver, (tt.Tacac[0][2] == 0) & (tt.Tadad[0][2] == 1));

  // Check that Tadad's top and bottom are linked
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tadad[0][2] == 0) & (tt.Tadad[0][2] == 1), tt);

  // Check that Tacac and Tbdbd are linked, and Tbdbd is linked in a torus.
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tacac[0][0] == 0) & (tt.Tbdbd[0][0] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tbdbd[0][0] == 0) & (tt.Tbdbd[5][5] == 1), tt);

  // Check Tcabcab
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcabcab[2][0] == 0) & (tt.Tcabcab[5][3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcabcab[0][2] == 0) & (tt.Tcabcab[8][2] == 1), tt);

  // Check Tcbacba
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbacba[2][0] == 0) & (tt.Tcbacba[7][3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbacba[0][2] == 0) & (tt.Tcbacba[8][2] == 1), tt);

  // Check Tcdacda
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcdacda[0][2] == 0) & (tt.Tcdacda[3][5] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcdacda[2][0] == 0) & (tt.Tcdacda[2][8] == 1), tt);

  // Check Tdcadca
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdcadca[0][2] == 0) & (tt.Tdcadca[3][7] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdcadca[2][0] == 0) & (tt.Tdcadca[2][8] == 1), tt);

  // Check Tcqadpa
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcqadpa[0][2] == 0) & (tt.Tcqadpa[0][5]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcqadpa[0][5] == 0) & (tt.Tcqadpa[0][8]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcqadpa[3][2] == 0) & (tt.Tcqadpa[3][7]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcqadpa[3][7] == 0) & (tt.Tcqadpa[3][12] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcqadpa[2][0] == 0) & (tt.Tcqadpa[2][15] == 1), tt);

  // Check Tdpacqa
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[0][2] == 0) & (tt.Tdpacqa[0][7]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[0][7] == 0) & (tt.Tdpacqa[0][12] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[3][2] == 0) & (tt.Tdpacqa[3][5]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[3][5] == 0) & (tt.Tdpacqa[3][8]  == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[2][0] == 0) & (tt.Tdpacqa[2][15] == 1), tt);

  // Check Tcbpcaq
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbpcaq[2][0] == 0) & (tt.Tcbpcaq[5] [0] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbpcaq[5][0] == 0) & (tt.Tcbpcaq[8] [0] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbpcaq[2][3] == 0) & (tt.Tcbpcaq[7] [3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbpcaq[7][3] == 0) & (tt.Tcbpcaq[12][3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcbpcaq[0][2] == 0) & (tt.Tcbpcaq[15][2] == 1), tt);

  // Check Tcaqcbp
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcaqcbp[2][0] == 0) & (tt.Tcaqcbp[7] [0] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcaqcbp[7][0] == 0) & (tt.Tcaqcbp[12][0] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcaqcbp[2][3] == 0) & (tt.Tcaqcbp[5] [3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcaqcbp[5][3] == 0) & (tt.Tcaqcbp[8] [3] == 1), tt);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tcaqcbp[0][2] == 0) & (tt.Tcaqcbp[15][2] == 1), tt);  
}


void TwelveTilesTest::testIterateViews(void) {
  // Object under test.
  MinisatSolver solver;
  TwelveTiles<> tt(&solver, 3, 5, 2, 10);
  
  ASSERT_SAT(solver);
  ASSERT_SAT_ASSUMP(solver, (tt.Tdpacqa[2][3] == 1));
  
  // Require that each node in the entire graph be 0.
  for ( auto view : tt ) {
    solver.require(view[0][0] == 0);
  }

  // Should be possible, not not if a node is required to be 1.
  ASSERT_SAT(solver);
  ASSERT_UNSAT_ASSUMP(solver, (tt.Tdpacqa[2][3] == 1), tt);
  
}
