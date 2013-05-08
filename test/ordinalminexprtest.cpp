// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/ordinal.h"
#include "../src/ordinalminexpr.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class OrdinalMinExprTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalMinExprTest);
  CPPUNIT_TEST(testMinLessThanValue);
  CPPUNIT_TEST(testMinLessThan);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testMinLessThanValue(void);
  void testMinLessThan(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalMinExprTest );

void OrdinalMinExprTest::testMinLessThanValue(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  Var var = 0;
  Ordinal ord1(dummyManager, -3, 2, var);
  Ordinal ord2(dummyManager, -1, 4, var);

  Requirement result, expected;

  CPPUNIT_ASSERT_THROW(min(ord1, ord2) <= 2, domain_error);

  CPPUNIT_ASSERT_EQUAL(ord1 <= 0 | ord2 <= 0, min(ord1, ord2) <= 0);

  CPPUNIT_ASSERT_EQUAL(Clause(ord1 <= -3), min(ord1, ord2) <= -3);

  CPPUNIT_ASSERT_EQUAL(Clause(), min(ord1, ord2) <= -4);
}

void OrdinalMinExprTest::testMinLessThan(void) {
  SolverManager& dummyManager = *(SolverManager*)0;

  Var var = 0;
  Ordinal ord1(dummyManager, -10, 10, var);
  Ordinal ord2(dummyManager, -10, 10, var);
  Ordinal ord3(dummyManager,  -2,  2, var);

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


// void OrdinalMinExprTest::testMinLessThan2(void) {
//   SolverManager& dummyManager = *(SolverManager*)0;

//   Var var = 0;
//   Ordinal ord1(dummyManager,  -2,  2, var);
//   Ordinal ord2(dummyManager,  -2,  2, var);
//   Ordinal ord3(dummyManager, -10, 10, var);

//   Requirement result = (min(ord1, ord2) <= ord3);

//   Requirement expected;
//   expected &= ~(ord3 <= -2 ) | (ord1 <= -2) | (ord2 <= -2);
//   expected &= ~(ord3 <= -1 ) | (ord1 <= -1) | (ord2 <= -1);
//   expected &= ~(ord3 <=  0 ) | (ord1 <=  0) | (ord2 <=  0);
//   expected &=                  (ord1 <=  1) | (ord2 <=  1);

//   CPPUNIT_ASSERT_EQUAL(expected, result);
// }
