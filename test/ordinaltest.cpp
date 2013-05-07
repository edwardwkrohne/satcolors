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

class OrdinalTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalTest);
  CPPUNIT_TEST(testIsOrdinal);
  CPPUNIT_TEST(testEmptyConstructorRange);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST(testEquality);
  CPPUNIT_TEST(testLessEq1);
  CPPUNIT_TEST(testLessEq2);
  CPPUNIT_TEST(testLessEqImpossible);
  CPPUNIT_TEST(testLessEqTrivial);
  CPPUNIT_TEST(testNotEq);
  CPPUNIT_TEST(testNegation);
  CPPUNIT_TEST(testNegation2);
  CPPUNIT_TEST(testSolveBug);
  CPPUNIT_TEST(testModelValue);
  CPPUNIT_TEST(testSummation);
  CPPUNIT_TEST(testSummationOptimization);
  CPPUNIT_TEST(testSummationImpossibleInequality);
  CPPUNIT_TEST(testSummationTrivialInequality);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsOrdinal(void);
  void testEmptyConstructorRange(void);
  void testDomainError(void);
  void testEquality(void);
  void testLessEq1(void);
  void testLessEq2(void);
  void testLessEqImpossible(void);
  void testLessEqTrivial(void);
  void testNotEq(void);
  void testNegation(void);
  void testNegation2(void);
  void testSolveBug(void);
  void testModelValue(void);
  void testSummation(void);
  void testSummationOptimization(void);
  void testSummationImpossibleInequality(void);
  void testSummationTrivialInequality(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalTest );

void OrdinalTest::testIsOrdinal(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ordinal(dummyManager, 0, 4, var);
  CPPUNIT_ASSERT_EQUAL(3u, ordinal.getNumLiterals());
  CPPUNIT_ASSERT_EQUAL(3, var);

  Requirement result = ordinal.typeRequirement();
  
  Requirement expected;
  expected &= implication(ordinal <= 0, ordinal <= 1);
  expected &= implication(ordinal <= 1, ordinal <= 2);

  CPPUNIT_ASSERT_EQUAL(expected, result);

}

void OrdinalTest::testEmptyConstructorRange(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  CPPUNIT_ASSERT_THROW(Ordinal(dummyManager, 2, 2, var), domain_error);
  CPPUNIT_ASSERT_THROW(Ordinal(dummyManager, 5, 3, var), domain_error);
}


void OrdinalTest::testEquality(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ordinal(dummyManager, 1, 5, var);

  using eqType = decltype(ordinal == 1);

  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal <= 1), ordinal == 1);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 2 & ordinal >= 2, ordinal == 2);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 3 & ordinal >= 3, ordinal == 3);
  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal >= 4), ordinal == 4);

  Ordinal smallOrd(dummyManager, 1, 2, var);
  CPPUNIT_ASSERT_EQUAL(eqType(), smallOrd == 1);
}

void OrdinalTest::testDomainError(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ordinal(dummyManager, 1, 4, var);

  CPPUNIT_ASSERT_THROW(ordinal == 4, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal == 0, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal != 4, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal != 0, domain_error);

  CPPUNIT_ASSERT_NO_THROW(ordinal == 3);
  CPPUNIT_ASSERT_NO_THROW(ordinal != 3);
  CPPUNIT_ASSERT_NO_THROW(ordinal == 1);
  CPPUNIT_ASSERT_NO_THROW(ordinal != 1);

  CPPUNIT_ASSERT_THROW   (ordinal >= 1, domain_error);
  CPPUNIT_ASSERT_NO_THROW(ordinal >= 2);
  CPPUNIT_ASSERT_NO_THROW(ordinal <= 1);

  CPPUNIT_ASSERT_THROW   (ordinal <= 3, domain_error);
  CPPUNIT_ASSERT_NO_THROW(ordinal <= 2);
  CPPUNIT_ASSERT_NO_THROW(ordinal >= 2);
}

void OrdinalTest::testLessEq1(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, -3, 5, var);
  Ordinal ord2(dummyManager, -2, 2, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= implication(ord2 < -1, ord1 < -1);
  expected &= implication(ord2 <  0, ord1 <  0);
  expected &= implication(ord2 <  1, ord1 <  1);
  expected &= ord1 < 2;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEq2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, 3, 7, var);
  Ordinal ord2(dummyManager, 1, 10, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= ord2 >= 3;
  expected &= implication(ord1 >= 4, ord2 >= 4);
  expected &= implication(ord1 >= 5, ord2 >= 5);
  expected &= implication(ord1 >= 6, ord2 >= 6);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqImpossible(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, 3, 7, var);
  Ordinal ord2(dummyManager, 0, 3, var);

  Requirement result = ord1 <= ord2;
  
  // Note most vexing parse
  Requirement expected((Clause())); // "FALSE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqTrivial(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, 0, 3, var);
  Ordinal ord2(dummyManager, 3, 7, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected; // "TRUE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testNotEq(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord1(dummyManager, -1, 4, var);
  Ordinal ord2(dummyManager, 1, 7, var);

  Requirement result = ord1 != ord2;
  
  Requirement expected;
  expected &= (ord1 != 1 | ord2 != 1);
  expected &= (ord1 != 2 | ord2 != 2);
  expected &= (ord1 != 3 | ord2 != 3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
  
}

void OrdinalTest::testNegation(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord(dummyManager,  0,  5, var);
  Ordinal negOrd = -ord;

  CPPUNIT_ASSERT_EQUAL(-4, negOrd.min);
  CPPUNIT_ASSERT_EQUAL( 1, negOrd.max);
  CPPUNIT_ASSERT_EQUAL(negOrd.getNumLiterals(), ord.getNumLiterals());
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrd >  -1); // Literal 0 true
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrd >  -2); // Literal 1 true
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrd >  -3); // Literal 2 true
  CPPUNIT_ASSERT_EQUAL(ord <  4, negOrd >  -4); // Literal 3 true

  Ordinal negNegOrd = -negOrd;
  CPPUNIT_ASSERT_EQUAL(negNegOrd >= 3, ord >= 3);
}

void OrdinalTest::testNegation2(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Ordinal ord(dummyManager,  0,  5, var);
  Ordinal negOrdPlusOne = -ord + 1;
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrdPlusOne >  -1+1);
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrdPlusOne >  -2+1);
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrdPlusOne >  -3+1);
}

void OrdinalTest::testSolveBug(void) {
  SolverManager manager;

  Ordinal ord(manager,  0,  5);
  
  CPPUNIT_ASSERT(manager.solve(ord == 2));
}

void OrdinalTest::testModelValue(void) {
  SolverManager manager;

  Ordinal ord(manager,  0,  5);
  Ordinal negOrd = -ord;

  Lit lit = mkLit(manager.newVars(1));
  manager.require(implication(lit, ord == 2));

  CPPUNIT_ASSERT(manager.solve(ord == 0));
  CPPUNIT_ASSERT_EQUAL( 0, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL( 0, negOrd.modelValue());

  CPPUNIT_ASSERT(manager.solve(ord == 1));
  CPPUNIT_ASSERT_EQUAL( 1, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-1, negOrd.modelValue());

  CPPUNIT_ASSERT(manager.solve(ord == 2));
  CPPUNIT_ASSERT_EQUAL( 2, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-2, negOrd.modelValue());

  CPPUNIT_ASSERT(manager.solve(ord == 3));
  CPPUNIT_ASSERT_EQUAL( 3, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-3, negOrd.modelValue());

  CPPUNIT_ASSERT(manager.solve(ord == 4));
  CPPUNIT_ASSERT_EQUAL( 4, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-4, negOrd.modelValue());
}

void OrdinalTest::testSummation(void) {
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

void OrdinalTest::testSummationOptimization(void) {
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

void OrdinalTest::testSummationImpossibleInequality(void) {
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

void OrdinalTest::testSummationTrivialInequality(void) {
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
