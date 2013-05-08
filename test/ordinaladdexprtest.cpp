// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/ordinal.h"
#include "../src/ordinaladdexpr.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class OrdinalAddExprTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalAddExprTest);
  CPPUNIT_TEST(testSummation);
  CPPUNIT_TEST(testSummationOptimization);
  CPPUNIT_TEST(testSummationImpossibleInequality);
  CPPUNIT_TEST(testSummationTrivialInequality);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testSummation(void);
  void testSummationOptimization(void);
  void testSummationImpossibleInequality(void);
  void testSummationTrivialInequality(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalAddExprTest );

void OrdinalAddExprTest::testSummation(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager,  -2,  2, var);
  Ordinal ord2(dummyManager, -10, 10, var);
  Ordinal ord3(dummyManager, -10, 10, var);

  Requirement result = (ord1 + ord2 <= ord3);
  
  // Claim:
  // Consider the condition "for every x, x <= ord1 implies x + ord2 <= ord3"
  // This condition is equivalent to ord1 + ord2 <= ord3.
  //
  // Proof:
  // Suppose that the condition holds. 
  // let x = ord1.  Then, ord1 >= x, so x + ord2 <= ord3.
  //
  // For the converse, suppose ord1+ord2 <= ord3.  Let x <= ord1.
  // We have x + ord2 <= ord1 + ord2 <= ord3.  (QED)
  //
  // Now, naturally we can't do all x, so we must do some of them.
  // First of all, for every x >= ord1.max, we have x > ord1,
  // so we need not require x + ord2 <= ord3.
  //
  // Claim:
  // Consider the condition "for all x <= ord1.min, x + ord2 <= ord3"
  // This condition is equivalent to ord1.min + ord2 <= ord3.
  //
  // Proof:
  // Suppose the condition holds.  Let x=ord1.min.  Then, ord1.min + ord2 <= ord3.
  //
  // Conversely, suppose ord1.min + ord2 <= ord3.  Fix x <= ord1.min. Then,
  // we have x + ord2 <= ord1.min + ord2 <= ord3.  (QED)
  Requirement expected;
  expected &=                 (-2 + ord2 <= ord3);
  expected &= ~(-1 <= ord1) | (-1 + ord2 <= ord3);
  expected &= ~( 0 <= ord1) | ( 0 + ord2 <= ord3);
  expected &= ~( 1 <= ord1) | ( 1 + ord2 <= ord3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalAddExprTest::testSummationOptimization(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, -10, 10, var);
  Ordinal ord2(dummyManager, -1, 3, var);
  Ordinal ord3(dummyManager, -3, 1, var);

  Requirement result = (ord1 + ord2 <= ord3);

  // We could leave it as simple as in the last test, or we could
  // consider the possibility that x + ord2 <= ord3 is necessary or
  // impossible.
  //
  // x + ord2 <= ord3 is necessary iff x < ord3.min - ord2.max.
  // Indeed, the following are equivalent:
  //    x + ord2  <= ord3                for all ord2, ord3
  //    x         <= ord3 - ord2         for all ord2, ord3
  //    x         <= ord3.min - ord2     for all ord2
  //    x         <  ord3.min - ord2.max 
  //
  // Similarly, x + ord2 <= ord3 is impossible iff x >= ord3.max - ord2.min
  // Indeed, the following are equivalent:
  //    x + ord2  >  ord3                for all ord2, ord3
  //    x         >  ord3 - ord2         for all ord2, ord3
  //    x         >= ord3.max - ord2     for all ord2
  //    x         >= ord3.max - ord2.min
  //
  // Thus, for x < ord3.min - ord2.max, we have necessarily x + ord2 <= ord3,
  // so there is no need for a requirement.
  //
  // For x >= ord3.max - ord2.min, it is impossible that x + ord2 <=
  // ord3, so the requirement boils down to ~(x <= ord1).

  Requirement expected;
  expected &= ~(-6 <= ord1) | (-5 + ord2 <= ord3);
  expected &= ~(-4 <= ord1) | (-4 + ord2 <= ord3);
  expected &= ~(-4 <= ord1) | (-4 + ord2 <= ord3);
  expected &= ~(-3 <= ord1) | (-3 + ord2 <= ord3);
  expected &= ~(-2 <= ord1) | (-2 + ord2 <= ord3);
  expected &= ~(-1 <= ord1) | (-1 + ord2 <= ord3);
  expected &= ~( 0 <= ord1) | ( 0 + ord2 <= ord3);
  expected &= ~( 1 <= ord1) | ( 1 + ord2 <= ord3);
  expected &= ~( 2 <= ord1);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalAddExprTest::testSummationImpossibleInequality(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager,   5, 10, var);
  Ordinal ord2(dummyManager,   5, 10, var);
  Ordinal ord3(dummyManager, -10, -5, var);

  Requirement result = (ord1 + ord2 <= ord3);
  
  Requirement expected = Clause(); // FALSE

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalAddExprTest::testSummationTrivialInequality(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, -10, -5, var);
  Ordinal ord2(dummyManager, -10, -5, var);
  Ordinal ord3(dummyManager,   5, 10, var);

  Requirement result = (ord1 + ord2 <= ord3);
  
  Requirement expected; // TRUE

  CPPUNIT_ASSERT_EQUAL(expected, result);
}
