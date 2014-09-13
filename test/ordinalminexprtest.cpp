// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/ordinal.h"
#include "../src/ordinalminexpr.h"

using namespace std;
using Minisat::Var;

class OrdinalMinExprTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalMinExprTest);
  CPPUNIT_TEST(testMinLessThanValue);
  CPPUNIT_TEST(testMinLessThan);
  CPPUNIT_TEST(testMinLessThan2);
  CPPUNIT_TEST(testMinLessThanImpossible);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testMinLessThanValue(void);
  void testMinLessThan(void);
  void testMinLessThan2(void);
  void testMinLessThanImpossible(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalMinExprTest );

void OrdinalMinExprTest::testMinLessThanValue(void) {
  Var var = 0;
  Ordinal ord1(nullptr, -3, 2, var);
  Ordinal ord2(nullptr, -1, 4, var);

  Requirement result, expected;

  // No way to represent clauses that must always be true
  CPPUNIT_ASSERT_THROW(min(ord1, ord2) <= 1, domain_error);

  // "Typical" case
  CPPUNIT_ASSERT_EQUAL(ord1 <= 0 | ord2 <= 0, min(ord1, ord2) <= 0);

  // Not possible for ord1 <= -3, so omit that possibility in the clause.
  CPPUNIT_ASSERT_EQUAL(Clause(ord1 <= -3), min(ord1, ord2) <= -3);

  // Not possible for ord1 or ord2 <= -4, so the returned clause should be "false".
  CPPUNIT_ASSERT_EQUAL(Clause(), min(ord1, ord2) <= -4);
}

void OrdinalMinExprTest::testMinLessThan(void) {
  Var var = 0;
  Ordinal ord1(nullptr, -10, 10, var);
  Ordinal ord2(nullptr, -10, 10, var);
  Ordinal ord3(nullptr,  -2,  2, var);

  Requirement result = (min(ord1, ord2) <= ord3);

  // This requirement is (should be) equivalent to:
  // ord1 <= ord3 or ord2 <= ord3.
  //
  // But just using the generic "or" would result in chaos.  Instead,
  // we require that for each value x that ord3 can take, we have
  //
  //   ord1 <= x    or    ord2 <= x.
  //
  // Specifically, if ord3 <= x, ord1 <= x or ord2 <= x.
  //
  // Now, it may very well be that for some values of x, both
  // conditions are trivial, in which case it is inefficient
  // to repeatedly exclude values of x.  Further inefficiencies
  // will be discussed in the later tests.
  Requirement expected;
  expected &= ~(ord3 <= -2 ) | (ord1 <= -2) | (ord2 <= -2);
  expected &= ~(ord3 <= -1 ) | (ord1 <= -1) | (ord2 <= -1);
  expected &= ~(ord3 <=  0 ) | (ord1 <=  0) | (ord2 <=  0);
  expected &=                  (ord1 <=  1) | (ord2 <=  1);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalMinExprTest::testMinLessThan2(void) {
  Var var = 0;
  Ordinal ord1(nullptr,  -5,  3, var);
  Ordinal ord2(nullptr,  -1,  5, var);
  Ordinal ord3(nullptr, -10, 10, var);

  Requirement result = (min(ord1, ord2) <= ord3);

  Requirement expected;
  expected &= ~(ord3 <= -6);
  expected &= ~(ord3 <= -5)  | (ord1 <= -5);
  expected &= ~(ord3 <= -4)  | (ord1 <= -4);
  expected &= ~(ord3 <= -3)  | (ord1 <= -3);
  expected &= ~(ord3 <= -2)  | (ord1 <= -2);
  expected &= ~(ord3 <= -1)  | (ord1 <= -1) | (ord2 <= -1);
  expected &= ~(ord3 <=  0)  | (ord1 <=  0) | (ord2 <=  0);
  expected &= ~(ord3 <=  1)  | (ord1 <=  1) | (ord2 <=  1);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalMinExprTest::testMinLessThanImpossible(void) {
  Var var = 0;
  Ordinal ord1(nullptr,  2,  5, var);
  Ordinal ord2(nullptr,  2,  5, var);
  Ordinal ord3(nullptr,  0,  2, var);

  Requirement result = (min(ord1, ord2) <= ord3);

  Requirement expected = Clause(); // FALSE

  CPPUNIT_ASSERT_EQUAL(expected, result);
}
