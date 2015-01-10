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
#include "../src/grid.h"

using namespace std;

class GridTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(GridTest);
  CPPUNIT_TEST(testConstruction2d);
  CPPUNIT_TEST(testConstructionFailure);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction2d(void);
  void testConstructionFailure(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( GridTest );

namespace {

function<Cardinal(int, int)> getBuilder(Solver* solver) {

  auto builder2d = [=](int row, int col) {
    return Cardinal(solver, row, row+2*col+1);
  };

  return builder2d;
}

} // namespace

void GridTest::testConstruction2d(void) {
  MockSolver solver;
  auto builder2d = getBuilder(&solver);

  auto list = makeList(3, 4, builder2d);

  // Just hit it with a battery of checks.
  CPPUNIT_ASSERT_EQUAL(3u, list.height());
  CPPUNIT_ASSERT_EQUAL(4u, list.width());

  CPPUNIT_ASSERT_EQUAL(0, list[0][0].min());
  CPPUNIT_ASSERT_EQUAL(1, list[0][0].max());
  CPPUNIT_ASSERT_EQUAL(2, list[2][0].min());
  CPPUNIT_ASSERT_EQUAL(3, list[2][0].max());
  CPPUNIT_ASSERT_EQUAL(1, list[1][3].min());
  CPPUNIT_ASSERT_EQUAL(8, list[1][3].max());

  CPPUNIT_ASSERT_EQUAL(list.data().numLiterals(), list.numLiterals());

  CPPUNIT_ASSERT_THROW(list[-1], out_of_range);
  CPPUNIT_ASSERT_THROW(list[3], out_of_range);
  CPPUNIT_ASSERT_THROW(list[1][-1], out_of_range);
  CPPUNIT_ASSERT_THROW(list[1][4], out_of_range);

}


void GridTest::testConstructionFailure(void) {
  MockSolver solver;
  auto builder2d = getBuilder(&solver);

  CPPUNIT_ASSERT_THROW(auto list = makeList(4, -1, builder2d), invalid_argument);
  CPPUNIT_ASSERT_THROW(auto list = makeList(-1, 4, builder2d), invalid_argument);
  CPPUNIT_ASSERT_THROW(auto list = makeList(-1, -1, builder2d), invalid_argument);
}
