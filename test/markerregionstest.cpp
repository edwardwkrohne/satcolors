// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/markerregions.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

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
//  CPPUNIT_TEST(testActionRow);
//  CPPUNIT_TEST(testActionRowMin);
//  CPPUNIT_TEST(testActionRowMax3);
//  CPPUNIT_TEST(testActionRowMax5);
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
  void testActionRow(void);
  void testActionRowMin(void);
  void testActionRowMax3(void);
  void testActionRowMax5(void);
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
  MarkerRegions mr(manager, 5, 5, 3);

  Cardinal row(manager, 1, 4);
  Cardinal col(manager, 1, 4);

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
  MarkerRegions mr(manager, 5, 5, 3);

  Cardinal row(manager, 1, 4);
  Cardinal col(manager, 1, 5);

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
  MarkerRegions mr(manager, 5, 5, 3);

  Cardinal row(manager, 1, 5);
  Cardinal col(manager, 1, 4);

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
  MarkerRegions mr(manager, 5, 5, 4);

  Cardinal row(manager, 1, 5);
  Cardinal col(manager, 1, 6);

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
  MarkerRegions mr(manager, 5, 5, 4);

  Cardinal row(manager, 1, 6);
  Cardinal col(manager, 1, 5);

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
  MarkerRegions mr(manager, 5, 5, 3);

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
  MarkerRegions mr(manager, 5, 5, 3);

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
  MarkerRegions mr(manager, 6, 5, 3);

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

void MarkerRegionsTest::testActionRow(void) {
}

void MarkerRegionsTest::testActionRowMin(void) {
  SolverManager& manager = *(SolverManager*)0;

  Var var = 0;

  /////////////////////////////////////////////////////////////////////////////////////
  // What is the minimum marker region row that a rectangle with width n can touch?
  MarkerRegions mr01(manager, 1, 01, 4, var);
  MarkerRegions mr15(manager, 1, 15, 4, var);
  MarkerRegions mr16(manager, 1, 16, 4, var);
  MarkerRegions mr31(manager, 1, 31, 4, var);
  MarkerRegions mr32(manager, 1, 32, 4, var);
  MarkerRegions mr47(manager, 1, 47, 4, var);
  MarkerRegions mr48(manager, 1, 48, 4, var);

  CPPUNIT_ASSERT_EQUAL(-1, mr01.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-1, mr15.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-2, mr16.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-2, mr31.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-3, mr32.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-3, mr47.actionRow.min);
  CPPUNIT_ASSERT_EQUAL(-4, mr48.actionRow.min);
}

void MarkerRegionsTest::testActionRowMax3(void) {
  SolverManager& manager = *(SolverManager*)0;

  Var var = 0;

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x3 rectangle touch?
  MarkerRegions mr11(manager, 1, 1, 3, var);
  MarkerRegions mr21(manager, 2, 1, 3, var);
  MarkerRegions mr31(manager, 3, 1, 3, var);
  MarkerRegions mr41(manager, 4, 1, 3, var);
  MarkerRegions mr51(manager, 5, 1, 3, var);
  MarkerRegions mr61(manager, 6, 1, 3, var);
  MarkerRegions mr71(manager, 7, 1, 3, var);
  MarkerRegions mr81(manager, 8, 1, 3, var);
  MarkerRegions mr91(manager, 9, 1, 3, var);

  CPPUNIT_ASSERT_EQUAL(2, mr11.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr21.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr31.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr41.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr51.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr61.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr71.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr81.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr91.actionRow.max);

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x4 rectangle touch?
  MarkerRegions mr12(manager, 1, 2, 3, var);
  MarkerRegions mr22(manager, 2, 2, 3, var);
  MarkerRegions mr32(manager, 3, 2, 3, var);
  MarkerRegions mr42(manager, 4, 2, 3, var);
  MarkerRegions mr52(manager, 5, 2, 3, var);
  MarkerRegions mr62(manager, 6, 2, 3, var);
  MarkerRegions mr72(manager, 7, 2, 3, var);
  MarkerRegions mr82(manager, 8, 2, 3, var);
  MarkerRegions mr92(manager, 9, 2, 3, var);

  CPPUNIT_ASSERT_EQUAL(2, mr12.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr22.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr32.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr42.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr52.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr62.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr72.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr82.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr92.actionRow.max);

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x5 rectangle touch?
  MarkerRegions mr13(manager, 1, 3, 3, var);
  MarkerRegions mr23(manager, 2, 3, 3, var);
  MarkerRegions mr33(manager, 3, 3, 3, var);
  MarkerRegions mr43(manager, 4, 3, 3, var);
  MarkerRegions mr53(manager, 5, 3, 3, var);
  MarkerRegions mr63(manager, 6, 3, 3, var);
  MarkerRegions mr73(manager, 7, 3, 3, var);
  MarkerRegions mr83(manager, 8, 3, 3, var);
  MarkerRegions mr93(manager, 9, 3, 3, var);

  CPPUNIT_ASSERT_EQUAL(3, mr13.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr23.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr33.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr43.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr53.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr63.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr73.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr83.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(5, mr93.actionRow.max);
}

void MarkerRegionsTest::testActionRowMax5(void) {
  SolverManager& manager = *(SolverManager*)0;

  Var var = 0;

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x3 rectangle touch?
  MarkerRegions mr11(manager, 1, 1, 5, var);
  MarkerRegions mr21(manager, 2, 1, 5, var);
  MarkerRegions mr31(manager, 3, 1, 5, var);
  MarkerRegions mr41(manager, 4, 1, 5, var);
  MarkerRegions mr51(manager, 5, 1, 5, var);
  MarkerRegions mr61(manager, 6, 1, 5, var);
  MarkerRegions mr71(manager, 7, 1, 5, var);
  MarkerRegions mr81(manager, 8, 1, 5, var);
  MarkerRegions mr91(manager, 9, 1, 5, var);

  CPPUNIT_ASSERT_EQUAL(2, mr11.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(2, mr21.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(2, mr31.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr41.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr51.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr61.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr71.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr81.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr91.actionRow.max);

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x6 rectangle touch?
  MarkerRegions mr14(manager, 1, 4, 5, var);
  MarkerRegions mr24(manager, 2, 4, 5, var);
  MarkerRegions mr34(manager, 3, 4, 5, var);
  MarkerRegions mr44(manager, 4, 4, 5, var);
  MarkerRegions mr54(manager, 5, 4, 5, var);
  MarkerRegions mr64(manager, 6, 4, 5, var);
  MarkerRegions mr74(manager, 7, 4, 5, var);
  MarkerRegions mr84(manager, 8, 4, 5, var);
  MarkerRegions mr94(manager, 9, 4, 5, var);

  CPPUNIT_ASSERT_EQUAL(2, mr14.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(2, mr24.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(2, mr34.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr44.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr54.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr64.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr74.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr84.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr94.actionRow.max);

  /////////////////////////////////////////////////////////////////////////////////////
  // How many different marker region rows can an (n+2)x7 rectangle touch?
  MarkerRegions mr15(manager, 1, 5, 5, var);
  MarkerRegions mr25(manager, 2, 5, 5, var);
  MarkerRegions mr35(manager, 3, 5, 5, var);
  MarkerRegions mr45(manager, 4, 5, 5, var);
  MarkerRegions mr55(manager, 5, 5, 5, var);
  MarkerRegions mr65(manager, 6, 5, 5, var);
  MarkerRegions mr75(manager, 7, 5, 5, var);
  MarkerRegions mr85(manager, 8, 5, 5, var);
  MarkerRegions mr95(manager, 9, 5, 5, var);

  CPPUNIT_ASSERT_EQUAL(2, mr15.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(2, mr25.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr35.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr45.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr55.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr65.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(3, mr75.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr85.actionRow.max);
  CPPUNIT_ASSERT_EQUAL(4, mr95.actionRow.max);
}
