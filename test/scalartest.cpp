// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/scalar.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class ScalarTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ScalarTest);
  CPPUNIT_TEST(testIsScalar);
  CPPUNIT_TEST(testNonzeroMin);
  CPPUNIT_TEST(testEqualsValue);
  CPPUNIT_TEST(testEqualsScalar);
  CPPUNIT_TEST(testEqualsScalar2);
  CPPUNIT_TEST(testEqualsScalar3);
  CPPUNIT_TEST(testNotEqualScalar);
  CPPUNIT_TEST(testNotEqualScalar2);
  CPPUNIT_TEST(testAddition);
  CPPUNIT_TEST(testGreaterThan);
  CPPUNIT_TEST(testRedundantGreaterThan);
  CPPUNIT_TEST(testLessThan);
  CPPUNIT_TEST(testRedundantLessThan);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsScalar(void);
  void testNonzeroMin(void);
  void testEqualsValue(void);
  void testEqualsScalar(void);
  void testEqualsScalar2(void);
  void testEqualsScalar3(void);
  void testNotEqualScalar(void);
  void testNotEqualScalar2(void);
  void testAddition(void);
  void testGreaterThan(void);
  void testRedundantGreaterThan(void);
  void testLessThan(void);
  void testRedundantLessThan(void);
  void testDomainError(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( ScalarTest );


void ScalarTest::testIsScalar(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 3, var);
  Requirement result = scalar.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ScalarTest::testNonzeroMin(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 2, 5, var);
  Requirement result = scalar.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testEqualsValue(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(mkLit(0), scalar == 0);
  CPPUNIT_ASSERT_EQUAL(mkLit(1), scalar == 1);
  CPPUNIT_ASSERT_EQUAL(mkLit(2), scalar == 2);

  CPPUNIT_ASSERT_EQUAL(~mkLit(0), scalar != 0);
  CPPUNIT_ASSERT_EQUAL(~mkLit(1), scalar != 1);
  CPPUNIT_ASSERT_EQUAL(~mkLit(2), scalar != 2);
}

void ScalarTest::testEqualsScalar(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar1(dummyManager, 0, 3, var);
  Scalar scalar2(dummyManager, 0, 3, var);

  Requirement expected;

  // Range requirements
  expected &= scalar1 >= 0;
  expected &= scalar1 <  3;
  expected &= scalar2 >= 0;
  expected &= scalar2 <  3;

  // scalar1 == 0 iff scalar2 == 0
  expected &= scalar1 != 0 | scalar2 == 0;
  expected &= scalar1 == 0 | scalar2 != 0;

  // ... == 1
  expected &= scalar1 != 1 | scalar2 == 1;
  expected &= scalar1 == 1 | scalar2 != 1;

  // ... == 2
  expected &= scalar1 != 2 | scalar2 == 2;
  expected &= scalar1 == 2 | scalar2 != 2;

  Requirement result = (scalar1 == scalar2);

  CPPUNIT_ASSERT_EQUAL((size_t)10, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testEqualsScalar2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar1(dummyManager, 0, 3, var);
  Scalar scalar2(dummyManager, 1, 4, var);

  Requirement expected;
  // Range requirements
  expected &= scalar1 >= 1;
  expected &= scalar1 <  4;
  expected &= scalar2 >= 0;
  expected &= scalar2 <  3;

  // scalar1 == 1 iff scalar2 == 1
  expected &= scalar1 != 1 | scalar2 == 1;
  expected &= scalar1 == 1 | scalar2 != 1;

  // ... == 2
  expected &= scalar1 != 2 | scalar2 == 2;
  expected &= scalar1 == 2 | scalar2 != 2;


  Requirement result = (scalar1 == scalar2);

  CPPUNIT_ASSERT_EQUAL((size_t)8, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testEqualsScalar3(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar1(dummyManager, 0, 2, var);
  Scalar scalar2(dummyManager, 3, 5, var);

  Requirement expected;
  // All values are impossible, resulting in an empty clause.
  expected &= Clause();

  // We also require scalar1, scalar2 to take whatever values they want.
  expected &= scalar1 >= 0;
  expected &= scalar2 <  5;

  Requirement result = (scalar1 == scalar2);

  CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testNotEqualScalar(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar1(dummyManager, 0, 3, var);
  Scalar scalar2(dummyManager, 0, 3, var);

  Requirement expected;
  expected &= scalar1 != 0 | scalar2 != 0;
  expected &= scalar1 != 1 | scalar2 != 1;
  expected &= scalar1 != 2 | scalar2 != 2;

  Requirement result = (scalar1 != scalar2);

  CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testNotEqualScalar2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar1(dummyManager, 0, 3, var);
  Scalar scalar2(dummyManager, 1, 4, var);

  Requirement expected;
  expected &= scalar1 != 1 | scalar2 != 1;
  expected &= scalar1 != 2 | scalar2 != 2;

  Requirement result = (scalar1 != scalar2);

  CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void ScalarTest::testAddition(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(scalar+1 == 2, scalar == 1);
  CPPUNIT_ASSERT_EQUAL(scalar-1 == 0, scalar == 1);
}


void ScalarTest::testGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 4, var);

  Clause expected = scalar == 2 | scalar == 3;

  CPPUNIT_ASSERT_EQUAL(scalar > 1, expected);
}

void ScalarTest::testRedundantGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 4, var);

  Clause expected = scalar == 0 | scalar == 1 | scalar == 2 | scalar == 3;

  CPPUNIT_ASSERT_EQUAL(scalar >= -10, expected);
}


void ScalarTest::testLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 4, var);

  Clause expected = scalar == 0 | scalar == 1;

  CPPUNIT_ASSERT_EQUAL(scalar <= 1, expected);
}

void ScalarTest::testRedundantLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 0, 4, var);

  Clause expected = scalar == 0 | scalar == 1 | scalar == 2 | scalar == 3;

  CPPUNIT_ASSERT_EQUAL(scalar < 10, expected);
}

void ScalarTest::testDomainError(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Scalar scalar(dummyManager, 1, 4, var);

  CPPUNIT_ASSERT_THROW(scalar == 4, domain_error);
  CPPUNIT_ASSERT_THROW(scalar == 0, domain_error);
  CPPUNIT_ASSERT_THROW(scalar != 4, domain_error);
  CPPUNIT_ASSERT_THROW(scalar != 0, domain_error);

}

