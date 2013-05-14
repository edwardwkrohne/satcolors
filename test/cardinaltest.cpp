// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/cardinal.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class CardinalTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CardinalTest);
  CPPUNIT_TEST(testIsCardinal);
  CPPUNIT_TEST(testNonzeroMin);
  CPPUNIT_TEST(testEqualsValue);
  CPPUNIT_TEST(testEqualsCardinal);
  CPPUNIT_TEST(testEqualsCardinal2);
  CPPUNIT_TEST(testEqualsCardinal3);
  CPPUNIT_TEST(testNotEqualCardinal);
  CPPUNIT_TEST(testNotEqualCardinal2);
  CPPUNIT_TEST(testAddition);
  CPPUNIT_TEST(testGreaterThan);
  CPPUNIT_TEST(testRedundantGreaterThan);
  CPPUNIT_TEST(testLessThan);
  CPPUNIT_TEST(testRedundantLessThan);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsCardinal(void);
  void testNonzeroMin(void);
  void testEqualsValue(void);
  void testEqualsCardinal(void);
  void testEqualsCardinal2(void);
  void testEqualsCardinal3(void);
  void testNotEqualCardinal(void);
  void testNotEqualCardinal2(void);
  void testAddition(void);
  void testGreaterThan(void);
  void testRedundantGreaterThan(void);
  void testLessThan(void);
  void testRedundantLessThan(void);
  void testDomainError(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( CardinalTest );


void CardinalTest::testIsCardinal(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 3, var);
  Requirement result = cardinal.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void CardinalTest::testNonzeroMin(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 2, 5, var);
  Requirement result = cardinal.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsValue(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(mkLit(0), cardinal == 0);
  CPPUNIT_ASSERT_EQUAL(mkLit(1), cardinal == 1);
  CPPUNIT_ASSERT_EQUAL(mkLit(2), cardinal == 2);

  CPPUNIT_ASSERT_EQUAL(~mkLit(0), cardinal != 0);
  CPPUNIT_ASSERT_EQUAL(~mkLit(1), cardinal != 1);
  CPPUNIT_ASSERT_EQUAL(~mkLit(2), cardinal != 2);
}

void CardinalTest::testEqualsCardinal(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal1(dummyManager, 0, 3, var);
  Cardinal cardinal2(dummyManager, 0, 3, var);

  Requirement expected;

  // Range requirements
  expected &= cardinal1 >= 0;
  expected &= cardinal1 <  3;
  expected &= cardinal2 >= 0;
  expected &= cardinal2 <  3;

  // cardinal1 == 0 iff cardinal2 == 0
  expected &= cardinal1 != 0 | cardinal2 == 0;
  expected &= cardinal1 == 0 | cardinal2 != 0;

  // ... == 1
  expected &= cardinal1 != 1 | cardinal2 == 1;
  expected &= cardinal1 == 1 | cardinal2 != 1;

  // ... == 2
  expected &= cardinal1 != 2 | cardinal2 == 2;
  expected &= cardinal1 == 2 | cardinal2 != 2;

  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)10, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsCardinal2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal1(dummyManager, 0, 3, var);
  Cardinal cardinal2(dummyManager, 1, 4, var);

  Requirement expected;
  // Range requirements
  expected &= cardinal1 >= 1;
  expected &= cardinal1 <  4;
  expected &= cardinal2 >= 0;
  expected &= cardinal2 <  3;

  // cardinal1 == 1 iff cardinal2 == 1
  expected &= cardinal1 != 1 | cardinal2 == 1;
  expected &= cardinal1 == 1 | cardinal2 != 1;

  // ... == 2
  expected &= cardinal1 != 2 | cardinal2 == 2;
  expected &= cardinal1 == 2 | cardinal2 != 2;


  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)8, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsCardinal3(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal1(dummyManager, 0, 2, var);
  Cardinal cardinal2(dummyManager, 3, 5, var);

  Requirement expected;
  // All values are impossible, resulting in an empty clause.
  expected &= Clause();

  // We also require cardinal1, cardinal2 to take whatever values they want.
  expected &= cardinal1 >= 0;
  expected &= cardinal2 <  5;

  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testNotEqualCardinal(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal1(dummyManager, 0, 3, var);
  Cardinal cardinal2(dummyManager, 0, 3, var);

  Requirement expected;
  expected &= cardinal1 != 0 | cardinal2 != 0;
  expected &= cardinal1 != 1 | cardinal2 != 1;
  expected &= cardinal1 != 2 | cardinal2 != 2;

  Requirement result = (cardinal1 != cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testNotEqualCardinal2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal1(dummyManager, 0, 3, var);
  Cardinal cardinal2(dummyManager, 1, 4, var);

  Requirement expected;
  expected &= cardinal1 != 1 | cardinal2 != 1;
  expected &= cardinal1 != 2 | cardinal2 != 2;

  Requirement result = (cardinal1 != cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testAddition(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(cardinal+1 == 2, cardinal == 1);
  CPPUNIT_ASSERT_EQUAL(cardinal-1 == 0, cardinal == 1);
}


void CardinalTest::testGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 4, var);

  Clause expected = cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal > 1, expected);
}

void CardinalTest::testRedundantGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 4, var);

  Clause expected = cardinal == 0 | cardinal == 1 | cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal >= -10, expected);
}


void CardinalTest::testLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 4, var);

  Clause expected = cardinal == 0 | cardinal == 1;

  CPPUNIT_ASSERT_EQUAL(cardinal <= 1, expected);
}

void CardinalTest::testRedundantLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 0, 4, var);

  Clause expected = cardinal == 0 | cardinal == 1 | cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal < 10, expected);
}

void CardinalTest::testDomainError(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Cardinal cardinal(dummyManager, 1, 4, var);

  CPPUNIT_ASSERT_THROW(cardinal == 4, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal == 0, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal != 4, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal != 0, domain_error);

}

