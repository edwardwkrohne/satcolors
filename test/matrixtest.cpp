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

using namespace std;

class MatrixTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(MatrixTest);
  CPPUNIT_TEST(testIsMatrix);
  CPPUNIT_TEST(testPairIndexedCardinalEquals);
  CPPUNIT_TEST(testPairIndexedCardinalNotEquals);
  CPPUNIT_TEST(testPairIndexedCardinalEqualsWithRange);
  CPPUNIT_TEST(test3x3);
  CPPUNIT_TEST(testRestrict);
  CPPUNIT_TEST(testOutOfRange);
  CPPUNIT_TEST(testRestrictErrors);
  CPPUNIT_TEST(testRotate);
  CPPUNIT_TEST(testReflect);
  CPPUNIT_TEST(testTranspose);
  CPPUNIT_TEST(testRotatePairIndex);
  CPPUNIT_TEST(testRestrictedTransposedView);
  CPPUNIT_TEST(testRotatedRestrictedView);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsMatrix(void);
  void testPairIndexedCardinalEquals(void);
  void testPairIndexedCardinalNotEquals(void);
  void testPairIndexedCardinalEqualsWithRange(void);
  void test3x3(void);
  void testRestrict(void);
  void testOutOfRange(void);
  void testRestrictErrors(void);
  void testRotate(void);
  void testReflect(void);
  void testTranspose(void);
  void testRotatePairIndex(void);
  void testRestrictedTransposedView(void);
  void testRotatedRestrictedView(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( MatrixTest );


void MatrixTest::testIsMatrix(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 2, 2, 0, 3);

  Requirement expected;
  expected &= matrix[0][0].typeRequirement();
  expected &= matrix[1][0].typeRequirement();
  expected &= matrix[0][1].typeRequirement();
  expected &= matrix[1][1].typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, matrix.typeRequirement());
}

void MatrixTest::testPairIndexedCardinalEquals(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 2, 2, 0, 3);
  Cardinal row(&solver, 0, 2);
  Cardinal col(&solver, 0, 2);

  // Require (row == 0 & col == 0) implies matrix[0][0] == 1, etc.
  Requirement expectedReq;
  expectedReq &= row >= 0 & row < 2;
  expectedReq &= col >= 0 & col < 2;
  expectedReq &= row != 0 | col != 0 | matrix[0][0] == 1;
  expectedReq &= row != 1 | col != 0 | matrix[1][0] == 1;
  expectedReq &= row != 0 | col != 1 | matrix[0][1] == 1;
  expectedReq &= row != 1 | col != 1 | matrix[1][1] == 1;

  Requirement result = matrix[row][col] == 1;

  CPPUNIT_ASSERT_EQUAL(expectedReq, result);
}

void MatrixTest::testPairIndexedCardinalNotEquals(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 2, 2, 0, 3); // values are in [0,3)
  Cardinal row(&solver, 0, 2);
  Cardinal col(&solver, 0, 2);

  // Require (row == 0 & col == 0) implies matrix[0][0] == 1, etc.
  Requirement expectedReq;
  expectedReq &= row >= 0 & row < 2;
  expectedReq &= col >= 0 & col < 2;
  expectedReq &= implication(row == 0 & col == 0, matrix[0][0] != 1);
  expectedReq &= implication(row == 1 & col == 0, matrix[1][0] != 1);
  expectedReq &= implication(row == 0 & col == 1, matrix[0][1] != 1);
  expectedReq &= implication(row == 1 & col == 1, matrix[1][1] != 1);

  Requirement result = matrix[row][col] != 1;

  CPPUNIT_ASSERT_EQUAL(expectedReq, result);
}

void MatrixTest::testPairIndexedCardinalEqualsWithRange(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 3, 3, 0, 3);
  Cardinal row(&solver,  1, 4);
  Cardinal col(&solver, -1, 2);

  Requirement expectedReq;

  // Impose some range restrictions
  expectedReq &= row >= 0 & row < 3;
  expectedReq &= col >= 0 & col < 3;

  // Values of row and col imply values of the matrix.
  expectedReq &= row != 1 | col != 0 | matrix[1][0] == 1;
  expectedReq &= row != 2 | col != 0 | matrix[2][0] == 1;
  expectedReq &= row != 1 | col != 1 | matrix[1][1] == 1;
  expectedReq &= row != 2 | col != 1 | matrix[2][1] == 1;

  Requirement result = matrix[row][col] == 1;

  CPPUNIT_ASSERT_EQUAL(expectedReq, result);
}

void MatrixTest::test3x3(void) {
  // Object under test.
  MinisatSolver solver;
  Matrix<> matrix(&solver, 3, 3, 0, 2);

  ASSERT_SAT(solver);
}

void MatrixTest::testRestrict(void) {
  // Object under test.
  MockSolver solver;
  unsigned int var = 5;
  Matrix<> matrix(&solver, 4, 6, 0, 3);  // values are in [0,3)
  auto view = matrix.restrict(1, 2, 3, 4); // Get a 2x2 view corresponding to [1,3)x[2,4).

  CPPUNIT_ASSERT_EQUAL(2u, view.width());
  CPPUNIT_ASSERT_EQUAL(2u, view.height());
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, view[0][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][2] == 0, view[1][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][3] == 0, view[0][1] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][3] == 0, view[1][1] == 0);
}

void MatrixTest::testOutOfRange(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 4, 4, 0, 3);

  CPPUNIT_ASSERT_THROW(matrix[-1][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][-1], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[4][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][4], out_of_range);
}

void MatrixTest::testRestrictErrors(void) {
  // Object under test.
  MockSolver solver;
  Matrix<> matrix(&solver, 4, 4, 0, 3);

  CPPUNIT_ASSERT_THROW(matrix.restrict(-1, 0, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, -1, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 0, 5, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 0, 2, 5), out_of_range);

  CPPUNIT_ASSERT_THROW(matrix.restrict(2, 0, 1, 4), invalid_argument);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 2, 4, 1), invalid_argument);
}

void MatrixTest::testRotate(void) {
  MockSolver solver;
  Matrix<> matrix(&solver, 4, 3, 0, 3);
  
  auto rot90  = matrix.rotCW();
  auto rot180 = rot90.rotCW();
  auto rot270 = rot180.rotCW();
  auto rot360 = rot270.rotCW();

  // Check dimensions
  CPPUNIT_ASSERT_EQUAL(4u, matrix.height()); 
  CPPUNIT_ASSERT_EQUAL(3u, matrix.width()); 
  CPPUNIT_ASSERT_EQUAL(3u, rot90.height());
  CPPUNIT_ASSERT_EQUAL(4u, rot90.width());
  CPPUNIT_ASSERT_EQUAL(4u, rot180.height());
  CPPUNIT_ASSERT_EQUAL(3u, rot180.width());

  // Easy rotations
  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, rot90 [0][3] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, rot180[3][2] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, rot270[2][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, rot360[0][0] == 0);

  // Harder rotations
  // I wish there was a better way to explain why these should be as
  // they are than to simply direct the reader to draw rectangles on
  // paper and verify for themself.
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, rot90 [2][2] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, rot180[2][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, rot270[0][1] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, rot360[1][2] == 0);

  auto rotNeg90 = matrix.rotCCW();
  CPPUNIT_ASSERT_EQUAL(rot270[1][2] == 0, rotNeg90[1][2] == 0);
}

void MatrixTest::testReflect(void) {
  MockSolver solver;
  Matrix<> matrix(&solver, 10, 10, 0, 3);
  
  auto reflH  = matrix.reflectH();
  auto reflV  = matrix.reflectV();
  auto rot180 = matrix.rotCCW().rotCCW();
  auto reflVH = reflV.reflectH();

  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, reflH [1][7] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, reflV [8][2] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, reflVH[8][7] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, rot180[8][7] == 0);
}

void MatrixTest::testTranspose(void) {
  MockSolver solver;
  Matrix<> matrix(&solver, 4, 3, 0, 3);
  
  auto transp  = matrix.transpose();
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, transp[2][1] == 0);
}

void MatrixTest::testRotatePairIndex(void) {
  MockSolver solver;
  Matrix<> matrix(&solver, 10, 10, 0, 3);
  Cardinal row(&solver, 10, 10);
  Cardinal col(&solver, 10, 10);

  auto rot90  = matrix.rotCW();

  CPPUNIT_ASSERT_EQUAL(matrix[row][col] == 0, rot90[9-col][row] == 0);
}

void MatrixTest::testRestrictedTransposedView() {
  MockSolver solver;
  Matrix<> matrix(&solver, 10, 10, 0, 3);
  auto view = matrix.restrict(1,3,5,5);
  auto transp = view.transpose();

  CPPUNIT_ASSERT_EQUAL(2u, transp.height());
  CPPUNIT_ASSERT_EQUAL(4u, transp.width());
  CPPUNIT_ASSERT_EQUAL(matrix[1][3] == 0, transp[0][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][3] == 0, transp[0][1] == 0);
}

void MatrixTest::testRotatedRestrictedView() {
  MockSolver solver;
  Matrix<> matrix(&solver, 10, 10, 0, 3);
  auto rot = matrix.rotCW();
  auto restrict1 = rot.restrict(0,0,10,10);

  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, restrict1[0][9] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[0][9] == 0, restrict1[9][9] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[9][9] == 0, restrict1[9][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[9][0] == 0, restrict1[0][0] == 0);

  auto restrict2 = rot.restrict(1,3,5,5);

  CPPUNIT_ASSERT_EQUAL(4u, restrict2.height());
  CPPUNIT_ASSERT_EQUAL(2u, restrict2.width());
  CPPUNIT_ASSERT_EQUAL(matrix[6][1] == 0, restrict2[0][0] == 0); 
  CPPUNIT_ASSERT_EQUAL(matrix[5][4] == 0, restrict2[3][1] == 0); 

}
