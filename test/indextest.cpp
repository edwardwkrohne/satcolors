// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/index.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class IndexTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(IndexTest);
  CPPUNIT_TEST(testIsIndex);
  CPPUNIT_TEST(testNonzeroMin);
  CPPUNIT_TEST(testEqualsValue);
  CPPUNIT_TEST(testEqualsIndex);
  CPPUNIT_TEST(testEqualsIndex2);
  CPPUNIT_TEST(testEqualsIndex3);
  CPPUNIT_TEST(testNotEqualIndex);
  CPPUNIT_TEST(testNotEqualIndex2);
  CPPUNIT_TEST(testAddition);
  CPPUNIT_TEST(testGreaterThan);
  CPPUNIT_TEST(testRedundantGreaterThan);
  CPPUNIT_TEST(testLessThan);
  CPPUNIT_TEST(testRedundantLessThan);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testIsIndex(void);
  void testNonzeroMin(void);
  void testEqualsValue(void);
  void testEqualsIndex(void);
  void testEqualsIndex2(void);
  void testEqualsIndex3(void);
  void testNotEqualIndex(void);
  void testNotEqualIndex2(void);
  void testAddition(void);
  void testGreaterThan(void);
  void testRedundantGreaterThan(void);
  void testLessThan(void);
  void testRedundantLessThan(void);
  void testDomainError(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( IndexTest );


void IndexTest::testIsIndex(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 3, var);
  Requirement result = index.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void IndexTest::testNonzeroMin(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 2, 5, var);
  Requirement result = index.typeRequirement();

  Requirement expected;
  expected &= mkLit(0) | mkLit(1) | mkLit(2);
  expected &= ~mkLit(0) | ~mkLit(1);
  expected &= ~mkLit(0) | ~mkLit(2);
  expected &= ~mkLit(1) | ~mkLit(2);

  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testEqualsValue(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(mkLit(0), index == 0);
  CPPUNIT_ASSERT_EQUAL(mkLit(1), index == 1);
  CPPUNIT_ASSERT_EQUAL(mkLit(2), index == 2);

  CPPUNIT_ASSERT_EQUAL(~mkLit(0), index != 0);
  CPPUNIT_ASSERT_EQUAL(~mkLit(1), index != 1);
  CPPUNIT_ASSERT_EQUAL(~mkLit(2), index != 2);
}

void IndexTest::testEqualsIndex(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index1(dummyManager, 0, 3, var);
  Index index2(dummyManager, 0, 3, var);

  Requirement expected;

  // Range requirements
  expected &= index1 >= 0;
  expected &= index1 <  3;
  expected &= index2 >= 0;
  expected &= index2 <  3;

  // index1 == 0 iff index2 == 0
  expected &= index1 != 0 | index2 == 0;
  expected &= index1 == 0 | index2 != 0;

  // ... == 1
  expected &= index1 != 1 | index2 == 1;
  expected &= index1 == 1 | index2 != 1;

  // ... == 2
  expected &= index1 != 2 | index2 == 2;
  expected &= index1 == 2 | index2 != 2;

  Requirement result = (index1 == index2);

  CPPUNIT_ASSERT_EQUAL((size_t)10, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testEqualsIndex2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index1(dummyManager, 0, 3, var);
  Index index2(dummyManager, 1, 4, var);

  Requirement expected;
  // Range requirements
  expected &= index1 >= 1;
  expected &= index1 <  4;
  expected &= index2 >= 0;
  expected &= index2 <  3;

  // index1 == 1 iff index2 == 1
  expected &= index1 != 1 | index2 == 1;
  expected &= index1 == 1 | index2 != 1;

  // ... == 2
  expected &= index1 != 2 | index2 == 2;
  expected &= index1 == 2 | index2 != 2;


  Requirement result = (index1 == index2);

  CPPUNIT_ASSERT_EQUAL((size_t)8, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testEqualsIndex3(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index1(dummyManager, 0, 2, var);
  Index index2(dummyManager, 3, 5, var);

  Requirement expected;
  // All values are impossible, resulting in an empty clause.
  expected &= Clause();

  // We also require index1, index2 to take whatever values they want.
  expected &= index1 >= 0;
  expected &= index2 <  5;

  Requirement result = (index1 == index2);

  CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testNotEqualIndex(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index1(dummyManager, 0, 3, var);
  Index index2(dummyManager, 0, 3, var);

  Requirement expected;
  expected &= index1 != 0 | index2 != 0;
  expected &= index1 != 1 | index2 != 1;
  expected &= index1 != 2 | index2 != 2;

  Requirement result = (index1 != index2);

  CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testNotEqualIndex2(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index1(dummyManager, 0, 3, var);
  Index index2(dummyManager, 1, 4, var);

  Requirement expected;
  expected &= index1 != 1 | index2 != 1;
  expected &= index1 != 2 | index2 != 2;

  Requirement result = (index1 != index2);

  CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void IndexTest::testAddition(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 3, var);

  CPPUNIT_ASSERT_EQUAL(index+1 == 2, index == 1);
  CPPUNIT_ASSERT_EQUAL(index-1 == 0, index == 1);
}


void IndexTest::testGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 4, var);

  Clause expected = index == 2 | index == 3;

  CPPUNIT_ASSERT_EQUAL(index > 1, expected);
}

void IndexTest::testRedundantGreaterThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 4, var);

  Clause expected = index == 0 | index == 1 | index == 2 | index == 3;

  CPPUNIT_ASSERT_EQUAL(index >= -10, expected);
}


void IndexTest::testLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 4, var);

  Clause expected = index == 0 | index == 1;

  CPPUNIT_ASSERT_EQUAL(index <= 1, expected);
}

void IndexTest::testRedundantLessThan(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 0, 4, var);

  Clause expected = index == 0 | index == 1 | index == 2 | index == 3;

  CPPUNIT_ASSERT_EQUAL(index < 10, expected);
}

void IndexTest::testDomainError(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  Index index(dummyManager, 1, 4, var);

  CPPUNIT_ASSERT_THROW(index == 4, domain_error);
  CPPUNIT_ASSERT_THROW(index == 0, domain_error);
  CPPUNIT_ASSERT_THROW(index != 4, domain_error);
  CPPUNIT_ASSERT_THROW(index != 0, domain_error);

}

