// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/ordinal.h"
#include "../src/ordinaladdexpr.h"

using namespace std;

class OrdinalTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalTest);
  CPPUNIT_TEST(testCopy);
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
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCopy(void);
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
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalTest );

void OrdinalTest::testCopy(void) {
  unsigned int var = 0;
  Ordinal ord(nullptr, 0, 4, var);
  unsigned int expectedVar = var;
  Ordinal cpy(ord);
  Ordinal mv(std::move(ord));

  cpy = ord;
  mv = std::move(ord);

  CPPUNIT_ASSERT_EQUAL(expectedVar, var);
}

void OrdinalTest::testIsOrdinal(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ordinal(nullptr, 0, 4, var);
  CPPUNIT_ASSERT_EQUAL(3u, ordinal.numLiterals());
  CPPUNIT_ASSERT_EQUAL(3u, var);

  Requirement result = ordinal.typeRequirement();
  
  Requirement expected;
  expected &= implication(ordinal <= 0, ordinal <= 1);
  expected &= implication(ordinal <= 1, ordinal <= 2);

  CPPUNIT_ASSERT_EQUAL(expected, result);

}

void OrdinalTest::testEmptyConstructorRange(void) {
  // Object under test.
  unsigned int var = 0;
  CPPUNIT_ASSERT_THROW(Ordinal(nullptr, 2, 2, var), domain_error);
  CPPUNIT_ASSERT_THROW(Ordinal(nullptr, 5, 3, var), domain_error);
}


void OrdinalTest::testEquality(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ordinal(nullptr, 1, 5, var);

  typedef decltype(ordinal == 1) eqType;

  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal <= 1), ordinal == 1);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 2 & ordinal >= 2, ordinal == 2);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 3 & ordinal >= 3, ordinal == 3);
  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal >= 4), ordinal == 4);

  Ordinal smallOrd(nullptr, 1, 2, var);
  CPPUNIT_ASSERT_EQUAL(eqType(), smallOrd == 1);
}

void OrdinalTest::testDomainError(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ordinal(nullptr, 1, 4, var);

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
  // Object under test.
  unsigned int var = 0;
  Ordinal ord1(nullptr, -3, 5, var);
  Ordinal ord2(nullptr, -2, 2, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= implication(ord2 < -1, ord1 < -1);
  expected &= implication(ord2 <  0, ord1 <  0);
  expected &= implication(ord2 <  1, ord1 <  1);
  expected &= ord1 < 2;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEq2(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord1(nullptr, 3, 7, var);
  Ordinal ord2(nullptr, 1, 10, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= ord2 >= 3;
  expected &= implication(ord1 >= 4, ord2 >= 4);
  expected &= implication(ord1 >= 5, ord2 >= 5);
  expected &= implication(ord1 >= 6, ord2 >= 6);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqImpossible(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord1(nullptr, 3, 7, var);
  Ordinal ord2(nullptr, 0, 3, var);

  Requirement result = ord1 <= ord2;
  
  // Note most vexing parse
  Requirement expected((Clause())); // "FALSE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqTrivial(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord1(nullptr, 0, 3, var);
  Ordinal ord2(nullptr, 3, 7, var);

  Requirement result = ord1 <= ord2;
  
  Requirement expected; // "TRUE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testNotEq(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord1(nullptr, -1, 4, var);
  Ordinal ord2(nullptr, 1, 7, var);

  Requirement result = ord1 != ord2;
  
  Requirement expected;
  expected &= (ord1 != 1 | ord2 != 1);
  expected &= (ord1 != 2 | ord2 != 2);
  expected &= (ord1 != 3 | ord2 != 3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
  
}

void OrdinalTest::testNegation(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord(nullptr,  0,  5, var);
  Ordinal negOrd = -ord;

  CPPUNIT_ASSERT_EQUAL(-4, negOrd.min());
  CPPUNIT_ASSERT_EQUAL( 1, negOrd.max());
  CPPUNIT_ASSERT_EQUAL(negOrd.numLiterals(), ord.numLiterals());
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrd >  -1); // Literal 0 true
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrd >  -2); // Literal 1 true
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrd >  -3); // Literal 2 true
  CPPUNIT_ASSERT_EQUAL(ord <  4, negOrd >  -4); // Literal 3 true

  Ordinal negNegOrd = -negOrd;
  CPPUNIT_ASSERT_EQUAL(negNegOrd >= 3, ord >= 3);
}

void OrdinalTest::testNegation2(void) {
  // Object under test.
  unsigned int var = 0;
  Ordinal ord(nullptr,  0,  5, var);
  Ordinal negOrdPlusOne = -ord + 1;
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrdPlusOne >  -1+1);
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrdPlusOne >  -2+1);
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrdPlusOne >  -3+1);
}

void OrdinalTest::testSolveBug(void) {
  SolverManager manager;

  Ordinal ord(&manager,  0,  5);
  
  CPPUNIT_ASSERT(manager.solve(ord == 2));
}

void OrdinalTest::testModelValue(void) {
  SolverManager manager;

  Ordinal ord(&manager,  0,  5);
  Ordinal negOrd = -ord;

  Literal lit = Literal(manager.newVars(1));
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
