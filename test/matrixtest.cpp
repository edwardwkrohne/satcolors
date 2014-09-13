// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/matrix.h"
#include "../src/matrixview.h"
#include "../src/pairindexedscalar.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::Var;

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
  Var var = 0;
  Matrix<> matrix(nullptr, 2, 2, 0, 3, var);

  Requirement expected;
  expected &= matrix[0][0].typeRequirement();
  expected &= matrix[1][0].typeRequirement();
  expected &= matrix[0][1].typeRequirement();
  expected &= matrix[1][1].typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, matrix.typeRequirement());
}

void MatrixTest::testPairIndexedCardinalEquals(void) {
  // Object under test.
  Var var = 0;
  Matrix<> matrix(nullptr, 2, 2, 0, 3, var);
  Cardinal row(nullptr, 0, 2, var);
  Cardinal col(nullptr, 0, 2, var);

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
  Var var = 0;
  Matrix<> matrix(nullptr, 2, 2, 0, 3, var); // values are in [0,3)
  Cardinal row(nullptr, 0, 2, var);
  Cardinal col(nullptr, 0, 2, var);

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
  Var var = 0;
  Matrix<> matrix(nullptr, 3, 3, 0, 3, var);
  Cardinal row(nullptr,  1, 4, var);
  Cardinal col(nullptr, -1, 2, var);

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
  SolverManager manager;

  // Object under test.
  Var var = 0;
  Matrix<> matrix(&manager, 3, 3, 0, 2);

  ASSERT_SAT(manager);
}

void MatrixTest::testRestrict(void) {
  // Object under test.
  Var var = 5;
  Matrix<> matrix(nullptr, 4, 6, 0, 3, var);  // values are in [0,3)
  auto view = matrix.restrict(1, 2, 3, 4); // Get a 2x2 view corresponding to [1,3)x[2,4).

  CPPUNIT_ASSERT_EQUAL(2, view.width());
  CPPUNIT_ASSERT_EQUAL(2, view.height());
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, view[0][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][2] == 0, view[1][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][3] == 0, view[0][1] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][3] == 0, view[1][1] == 0);
}

void MatrixTest::testOutOfRange(void) {
  // Object under test.
  Var var = 0;
  Matrix<> matrix(nullptr, 4, 4, 0, 3, var);

  CPPUNIT_ASSERT_THROW(matrix[-1][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][-1], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[4][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][4], out_of_range);
}

void MatrixTest::testRestrictErrors(void) {
  // Object under test.
  Var var = 0;
  Matrix<> matrix(nullptr, 4, 4, 0, 3, var);

  CPPUNIT_ASSERT_THROW(matrix.restrict(-1, 0, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, -1, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 0, 5, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 0, 2, 5), out_of_range);

  CPPUNIT_ASSERT_THROW(matrix.restrict(2, 0, 1, 4), invalid_argument);
  CPPUNIT_ASSERT_THROW(matrix.restrict(0, 2, 4, 1), invalid_argument);
}

void MatrixTest::testRotate(void) {
  Var var = 0;
  Matrix<> matrix(nullptr, 4, 3, 0, 3, var);
  
  auto rot90  = matrix.rotCW();
  auto rot180 = rot90.rotCW();
  auto rot270 = rot180.rotCW();
  auto rot360 = rot270.rotCW();

  // Check dimensions
  // TODO: Figure out return types, get rid of kludges like the
  // explicit cast here
  CPPUNIT_ASSERT_EQUAL(4, int(matrix.height())); 
  CPPUNIT_ASSERT_EQUAL(3, int(matrix.width())); 
  CPPUNIT_ASSERT_EQUAL(3, rot90.height());
  CPPUNIT_ASSERT_EQUAL(4, rot90.width());
  CPPUNIT_ASSERT_EQUAL(4, rot180.height());
  CPPUNIT_ASSERT_EQUAL(3, rot180.width());

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
  Var var = 0;
  Matrix<> matrix(nullptr, 10, 10, 0, 3, var);
  
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
  Var var = 0;
  Matrix<> matrix(nullptr, 4, 3, 0, 3, var);
  
  auto transp  = matrix.transpose();
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, transp[2][1] == 0);
}

void MatrixTest::testRotatePairIndex(void) {
  Var var = 0;
  Matrix<> matrix(nullptr, 10, 10, 0, 3, var);
  Cardinal row(nullptr, 10, 10, var);
  Cardinal col(nullptr, 10, 10, var);

  auto rot90  = matrix.rotCW();

  CPPUNIT_ASSERT_EQUAL(matrix[row][col] == 0, rot90[9-col][row] == 0);
}

void MatrixTest::testRestrictedTransposedView() {
  Var var = 0;
  Matrix<> matrix(nullptr, 10, 10, 0, 3, var);
  auto view = matrix.restrict(1,3,5,5);
  auto transp = view.transpose();

  CPPUNIT_ASSERT_EQUAL(2, transp.height());
  CPPUNIT_ASSERT_EQUAL(4, transp.width());
  CPPUNIT_ASSERT_EQUAL(matrix[1][3] == 0, transp[0][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][3] == 0, transp[0][1] == 0);
}

void MatrixTest::testRotatedRestrictedView() {
  Var var = 0;
  Matrix<> matrix(nullptr, 10, 10, 0, 3, var);
  auto rot = matrix.rotCW();
  auto restrict1 = rot.restrict(0,0,10,10);

  CPPUNIT_ASSERT_EQUAL(matrix[0][0] == 0, restrict1[0][9] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[0][9] == 0, restrict1[9][9] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[9][9] == 0, restrict1[9][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[9][0] == 0, restrict1[0][0] == 0);

  auto restrict2 = rot.restrict(1,3,5,5);

  CPPUNIT_ASSERT_EQUAL(4, restrict2.height());
  CPPUNIT_ASSERT_EQUAL(2, restrict2.width());
  CPPUNIT_ASSERT_EQUAL(matrix[6][1] == 0, restrict2[0][0] == 0); 
  CPPUNIT_ASSERT_EQUAL(matrix[5][4] == 0, restrict2[3][1] == 0); 

}
