// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/isolatedpattern.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;

class IsolatedPatternTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(IsolatedPatternTest);
  CPPUNIT_TEST(testConstruction);
  CPPUNIT_TEST(testConstruction2);
  CPPUNIT_TEST(testBlankSolution);
  CPPUNIT_TEST(testFullExtent);
  CPPUNIT_TEST(testWeirdPattern);
  CPPUNIT_TEST(testTooWide);
  CPPUNIT_TEST(testTooTall);
  CPPUNIT_TEST(testOnView);
  CPPUNIT_TEST(testSeparateSolutions);
  CPPUNIT_TEST(testEmptyPattern);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction(void);
  void testConstruction2(void);
  void testBlankSolution(void);
  void testFullExtent(void);
  void testWeirdPattern(void);
  void testTooWide(void);
  void testTooTall(void);
  void testOnView(void);
  void testSeparateSolutions(void);
  void testEmptyPattern(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( IsolatedPatternTest );

void IsolatedPatternTest::testConstruction(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 7, 10, 0, 2);
  // This pattern will exist somewhere in the matrix.  Also,
  // the entire REST of the matrix will consist of zeroes.
  IsolatedPattern isoPat(&manager,
      matrix,
      " .  x  .    ; "
      "(x) x  x    ; "
      " .  x  .    ; ");

  // The parentheses in the pattern string indicate where the "center" (row, col) is.  The semicolons serve as line breaks.
  CPPUNIT_ASSERT_EQUAL(1, isoPat.patternCenterRow);
  CPPUNIT_ASSERT_EQUAL(0, isoPat.patternCenterCol);
  CPPUNIT_ASSERT_EQUAL(3, isoPat.patternHeight);
  CPPUNIT_ASSERT_EQUAL(3, isoPat.patternWidth);
  CPPUNIT_ASSERT_EQUAL(1, isoPat.row.min());
  CPPUNIT_ASSERT_EQUAL(6, isoPat.row.max());
  CPPUNIT_ASSERT_EQUAL(0, isoPat.col.min());
  CPPUNIT_ASSERT_EQUAL(8, isoPat.col.max());
  CPPUNIT_ASSERT_EQUAL(7, isoPat.matrix.height());
  CPPUNIT_ASSERT_EQUAL(10,isoPat.matrix.width());

  // Possible to have a horizontal row of three, because those fit in the pattern.
  ASSERT_SAT_ASSUMP(manager,
      matrix[2][1] == 1 &
      matrix[2][2] == 1 &
      matrix[2][3] == 1);

  // But if so, there is only once place row can be
  ASSERT_UNSAT_ASSUMP(manager,
      matrix[2][1] == 1 &
      matrix[2][2] == 1 &
      matrix[2][3] == 1 &
      isoPat.row != 2,
      matrix);
  // And one place col can be
  ASSERT_UNSAT_ASSUMP(manager,
      matrix[2][1] == 1 &
      matrix[2][2] == 1 &
      matrix[2][3] == 1 &
      isoPat.col != 1,
      matrix);

  // Not possible to have to adjacent cells on row 0 filled in
  ASSERT_UNSAT_ASSUMP(manager,
      matrix[0][1] == 1 &
      matrix[0][2] == 1,
      matrix);

  // Not possible to have to cells filled in with a gap in between.
  ASSERT_UNSAT_ASSUMP(manager,
      matrix[2][1] == 1 &
      matrix[2][2] == 0 &
      matrix[2][3] == 1,
      matrix);
}

void IsolatedPatternTest::testConstruction2(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 7, 7, 0, 2);
  IsolatedPattern isoPat(&manager,
      matrix,
      " .  x        ; "
      " .  x x .    ; "
      " . (.)       ; ");

  // The parentheses in the pattern string indicate where the "center" (row, col) is.  The semicolons serve as line breaks.
  CPPUNIT_ASSERT_EQUAL(2,  isoPat.patternCenterRow);
  CPPUNIT_ASSERT_EQUAL(1,  isoPat.patternCenterCol);
  CPPUNIT_ASSERT_EQUAL(3,  isoPat.patternHeight);
  CPPUNIT_ASSERT_EQUAL(4,  isoPat.patternWidth);
  CPPUNIT_ASSERT_EQUAL(2,  isoPat.row.min());
  CPPUNIT_ASSERT_EQUAL(7,  isoPat.row.max());
  CPPUNIT_ASSERT_EQUAL(1,  isoPat.col.min());
  CPPUNIT_ASSERT_EQUAL(5,  isoPat.col.max());
}

void IsolatedPatternTest::testBlankSolution(void) {
  SolverManager manager;

  unsigned int height = 7;
  unsigned int width = 7;
  Matrix<> matrix(&manager, height, width, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      "(x) x   ;"
      " x  .   ;");

  DualClause assumptions;
  for ( int i = 0; i < height; i++ ) {
    for ( int j = 0; j < width; j++ ) {
      assumptions &= matrix[i][j] == 0;
    }
  }

  // A blank matrix should not be possible.
  ASSERT_UNSAT_ASSUMP(manager, assumptions, matrix);
}

void IsolatedPatternTest::testFullExtent(void) {
  SolverManager manager;

  unsigned int height = 7;
  unsigned int width = 7;
  Matrix<> matrix(&manager, height, width, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      "(x) x   ;"
      " x  .   ;");

  // A pattern in the corner should be solvable
  ASSERT_SAT_ASSUMP(manager, matrix[5][5] == 1 & matrix[5][6] == 1 & matrix[6][5] == 1);
}

void IsolatedPatternTest::testWeirdPattern(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 5, 5, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      " . x . x  x  ;"
      " . . . x (.) ;");

  // A pattern in the corner should be solvable
  ASSERT_SAT_ASSUMP(manager, matrix[0][3] == 1);
  ASSERT_SAT_ASSUMP(manager, matrix[0][4] == 1);
}

void IsolatedPatternTest::testTooWide(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 5, 5, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      " . x . x  x      ;"
      " . . . x (.) x   ;");

  // A pattern in the corner should be solvable
  ASSERT_UNSAT_ASSUMP(manager, DualClause(), matrix);
}

void IsolatedPatternTest::testTooTall(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 5, 5, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      " x   .  ;"
      " .   x  ;"
      " x   .  ;"
      " .   x  ;"
      " x  (.) ;"
      " .   x  ;");

  // A pattern in the corner should be solvable
  ASSERT_UNSAT(manager, matrix);
}

void IsolatedPatternTest::testOnView(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 18, 27, 0, 2);
  IsolatedPattern pattern(&manager, matrix.view(0, 0, 9, 9),
      " .  x  .    ;"
      " x (x) x    ;");

  // A pattern in the corner should be solvable
  ASSERT_SAT(manager);
}

void IsolatedPatternTest::testSeparateSolutions(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 7, 10, 0, 2);
  // This pattern will exist somewhere in the matrix.  Also,
  // the entire REST of the matrix will consist of zeroes.
  IsolatedPattern pattern(&manager,
      matrix,
      " .  x  .    ; "
      "(x) x  x    ; "
      " .  x  .    ; ");


  // Solvable
  ASSERT_SAT(manager);

  manager.require(pattern.diffSolnReq());

  // And solvable in a different configuration
  ASSERT_SAT(manager);
}
void IsolatedPatternTest::testEmptyPattern(void) {
  SolverManager manager;

  Matrix<> matrix(&manager, 5, 7, 0, 2);
  IsolatedPattern pattern(&manager, matrix,
      " (.) . . . . ;"
      "  .  . . . . ;"
      "  .  . . . . ;"
      "  .  . . . . ;"
      );

  // Should not be solvable with non-blanks
  ASSERT_UNSAT_ASSUMP(manager, matrix[1][2] == 1, matrix);

  // Should always be solvable
  ASSERT_SAT(manager);

  manager.require(pattern.diffSolnReq());

  // But only one solution should be possible.
  ASSERT_UNSAT(manager, matrix);
}
