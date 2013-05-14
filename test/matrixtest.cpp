// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/matrix.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class MatrixTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(MatrixTest);
  CPPUNIT_TEST(testIsMatrix);
  CPPUNIT_TEST(testPairIndexedCardinalEquals);
  CPPUNIT_TEST(testPairIndexedCardinalNotEquals);
  CPPUNIT_TEST(testPairIndexedCardinalEqualsWithRange);
  CPPUNIT_TEST(test3x3);
  CPPUNIT_TEST(testIncrementVar);
  CPPUNIT_TEST(testView);
  CPPUNIT_TEST(testOutOfRange);
  CPPUNIT_TEST(testViewErrors);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsMatrix(void);
  void testPairIndexedCardinalEquals(void);
  void testPairIndexedCardinalNotEquals(void);
  void testPairIndexedCardinalEqualsWithRange(void);
  void test3x3(void);
  void testIncrementVar(void);
  void testView(void);
  void testOutOfRange(void);
  void testViewErrors(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( MatrixTest );


void MatrixTest::testIsMatrix(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 2, 2, 0, 3, var);

  Requirement expected;
  expected &= matrix[0][0].typeRequirement();
  expected &= matrix[1][0].typeRequirement();
  expected &= matrix[0][1].typeRequirement();
  expected &= matrix[1][1].typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, matrix.typeRequirement());
}

void MatrixTest::testPairIndexedCardinalEquals(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 2, 2, 0, 3, var);
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
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 2, 2, 0, 3, var); // values are in [0,3)
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
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 3, 3, 0, 3, var);
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

  CPPUNIT_ASSERT_EQUAL(expectedReq, matrix[row][col] == 1);
}

void MatrixTest::testIncrementVar(void) {
  SolverManager manager;

  // Object under test.
  Matrix matrix1(manager, 3, 3, 0, 2);
  Matrix matrix2(manager, 3, 3, 0, 2);

  // These matrices should be separate, not point to the same values.
  CPPUNIT_ASSERT_EQUAL(0U, (unsigned int)matrix1.startingVar);
  CPPUNIT_ASSERT_EQUAL(matrix1.startingVar + matrix1.getNumLiterals(), (unsigned int)matrix2.startingVar);
}


void MatrixTest::test3x3(void) {
  SolverManager manager;

  // Object under test.
  Var var = 0;
  Matrix matrix(manager, 3, 3, 0, 2);

  ASSERT_SAT(manager);
}

void MatrixTest::testView(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 5;
  Matrix matrix(dummyManager, 4, 6, 0, 3, var);  // values are in [0,3)
  MatrixView view = matrix.view(1, 2, 3, 4); // Get a 2x2 view corresponding to [1,3)x[2,4).

  CPPUNIT_ASSERT_EQUAL(2, view.width);
  CPPUNIT_ASSERT_EQUAL(2, view.height);
  CPPUNIT_ASSERT_EQUAL(matrix[1][2] == 0, view[0][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][2] == 0, view[1][0] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[1][3] == 0, view[0][1] == 0);
  CPPUNIT_ASSERT_EQUAL(matrix[2][3] == 0, view[1][1] == 0);
}

void MatrixTest::testOutOfRange(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 4, 4, 0, 3, var);

  CPPUNIT_ASSERT_THROW(matrix[-1][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][-1], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[4][0], out_of_range);
  CPPUNIT_ASSERT_THROW(matrix[0][4], out_of_range);
}

void MatrixTest::testViewErrors(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Matrix matrix(dummyManager, 4, 4, 0, 3, var);

  CPPUNIT_ASSERT_THROW(matrix.view(-1, 0, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.view(0, -1, 2, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.view(0, 0, 5, 2), out_of_range);
  CPPUNIT_ASSERT_THROW(matrix.view(0, 0, 2, 5), out_of_range);

  CPPUNIT_ASSERT_THROW(matrix.view(2, 0, 1, 4), invalid_argument);
  CPPUNIT_ASSERT_THROW(matrix.view(0, 2, 4, 1), invalid_argument);
}
