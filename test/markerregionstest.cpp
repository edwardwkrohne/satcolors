// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/markerregions.h"
#include "../src/pairindexedscalar.h"

using namespace std;

class MarkerRegionsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(MarkerRegionsTest);
  CPPUNIT_TEST(testFloorDiv);
  CPPUNIT_TEST(testEdgeBug);
  CPPUNIT_TEST(testEdgeBug2);
  CPPUNIT_TEST(testEdgeBug3);
  CPPUNIT_TEST(testEdgeBug4);
  CPPUNIT_TEST(testEdgeBug5);
  CPPUNIT_TEST(testShortBoxes); // FIXME
  CPPUNIT_TEST(testTallBoxes);
  CPPUNIT_TEST(testTooTallBoxes);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testFloorDiv(void);
  void testEdgeBug(void);
  void testEdgeBug2(void);
  void testEdgeBug3(void);
  void testEdgeBug4(void);
  void testEdgeBug5(void);
  void testShortBoxes(void);
  void testTallBoxes(void);
  void testTooTallBoxes(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( MarkerRegionsTest );

void MarkerRegionsTest::testFloorDiv(void) {
  CPPUNIT_ASSERT_EQUAL( 0, floorDiv(  0,  5 ));
  CPPUNIT_ASSERT_EQUAL( 0, floorDiv(  4,  5 ));
  CPPUNIT_ASSERT_EQUAL( 1, floorDiv(  5,  5 ));
  CPPUNIT_ASSERT_EQUAL( 1, floorDiv(  9,  5 ));

  CPPUNIT_ASSERT_EQUAL(-1, floorDiv( -4,  5 ));
  CPPUNIT_ASSERT_EQUAL(-1, floorDiv( -5,  5 ));
  CPPUNIT_ASSERT_EQUAL(-2, floorDiv( -9,  5 ));

  CPPUNIT_ASSERT_EQUAL( 0, floorDiv(  0, -5 ));
  CPPUNIT_ASSERT_EQUAL(-1, floorDiv(  4, -5 ));
  CPPUNIT_ASSERT_EQUAL(-1, floorDiv(  5, -5 ));
  CPPUNIT_ASSERT_EQUAL(-2, floorDiv(  9, -5 ));

  CPPUNIT_ASSERT_EQUAL( 0, floorDiv( -4, -5 ));
  CPPUNIT_ASSERT_EQUAL( 1, floorDiv( -5, -5 ));
  CPPUNIT_ASSERT_EQUAL( 1, floorDiv( -9, -5 ));
}

void MarkerRegionsTest::testEdgeBug(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 3);

  Cardinal row(&manager, 1, 4);
  Cardinal col(&manager, 1, 4);

  // Require something that should be impossible.
  manager.require(mr.boundaryUnderlay[row  ][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+2][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row  ][col+2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+2][col+2] != MarkerRegions::BLANK);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "(row, col) = (" << row.modelValue() << ", " << col.modelValue() << ")" << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}

void MarkerRegionsTest::testEdgeBug2(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 3);

  Cardinal row(&manager, 1, 4);
  Cardinal col(&manager, 1, 5);

  // Require something that should be impossible.
  manager.require(mr.boundaryUnderlay[row  ][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+2][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row  ][col+1] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+2][col+1] != MarkerRegions::BLANK);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "(row, col) = (" << row.modelValue() << ", " << col.modelValue() << ")" << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}

void MarkerRegionsTest::testEdgeBug3(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 3);

  Cardinal row(&manager, 1, 5);
  Cardinal col(&manager, 1, 4);

  // Require something that should be impossible.
  manager.require(mr.boundaryUnderlay[row  ][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+1][col  ] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row  ][col+2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[row+1][col+2] != MarkerRegions::BLANK);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "(row, col) = (" << row.modelValue() << ", " << col.modelValue() << ")" << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}

void MarkerRegionsTest::testEdgeBug4(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 4);

  Cardinal row(&manager, 1, 5);
  Cardinal col(&manager, 1, 6);

  // Require something that should be impossible.
  manager.require(mr.vSpacing[row  ][col  ] == 2);
  manager.require(mr.boundaryUnderlay[row+1][col  ] == MarkerRegions::TEE);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "(row, col) = (" << row.modelValue() << ", " << col.modelValue() << ")" << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}

void MarkerRegionsTest::testEdgeBug5(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 4);

  Cardinal row(&manager, 1, 6);
  Cardinal col(&manager, 1, 5);

  // Require something that should be impossible.
  manager.require(mr.hSpacing[row  ][col  ] == 2);
  manager.require(mr.boundaryUnderlay[row  ][col+1] == MarkerRegions::TEE);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "(row, col) = (" << row.modelValue() << ", " << col.modelValue() << ")" << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}

void MarkerRegionsTest::testShortBoxes(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 3);

  // Require something that should be possible.
  manager.require(mr.boundaryUnderlay[1][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][4] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[4][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[4][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[4][4] != MarkerRegions::BLANK);

  // Is it?
  CPPUNIT_ASSERT( manager.solve() );
}


void MarkerRegionsTest::testTallBoxes(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 5, 5, 3);

  // Require something that should be possible.
  manager.require(mr.boundaryUnderlay[1][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][4] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[5][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[5][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[5][4] != MarkerRegions::BLANK);

  // Is it?
  CPPUNIT_ASSERT( manager.solve() );
}

void MarkerRegionsTest::testTooTallBoxes(void) {
  SolverManager manager;

  // Object under test.
  // BoundaryUnderlay is 7x7.
  MarkerRegions mr(&manager, 6, 5, 3);

  // Require something that should be possible.
  manager.require(mr.boundaryUnderlay[1][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[1][4] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[6][2] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[6][3] != MarkerRegions::BLANK);
  manager.require(mr.boundaryUnderlay[6][4] != MarkerRegions::BLANK);

  // Is it?
  if ( manager.solve () ) {
    ostringstream sout;
    sout << endl
         << "Boundary " << endl << mr.boundaryUnderlay << endl
         << "Blank Neighbor " << endl << mr.teeBlankNeighbor << endl
         << "Tee Neighbor " << endl << mr.teeTeeNeighbor << endl
         << "Hor. Spacing " << endl << mr.hSpacing << endl
         << "Vert. Spacing " << endl << mr.vSpacing << endl;
    CPPUNIT_FAIL(sout.str());
  }
}
