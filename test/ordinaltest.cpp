// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/ordinal.h"

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
  expected &= ordinal <= 0 | ordinal > 0;
  expected &= ordinal <= 1 | ordinal > 1;
  expected &= ordinal <= 2 | ordinal > 2;

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
