// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/list2d.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class List2dTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(List2dTest);
  CPPUNIT_TEST(testConstruction2d);
  CPPUNIT_TEST(testConstructionFailure);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction2d(void);
  void testConstructionFailure(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( List2dTest );

namespace {

typedef Cardinal::value_type value_type;

Cardinal builder2d(SolverManager& manager, value_type row, value_type col, Var& var) {
  return Cardinal(&manager, row, row+2*col+1, var);
}

}

void List2dTest::testConstruction2d(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  auto list = makeList(dummyManager, 3, 4, builder2d, var);

  // Just hit it with a battery of checks.
  CPPUNIT_ASSERT_EQUAL(3, list.height);
  CPPUNIT_ASSERT_EQUAL(4, list.width);

  CPPUNIT_ASSERT_EQUAL(0, list[0][0].min());
  CPPUNIT_ASSERT_EQUAL(1, list[0][0].max());
  CPPUNIT_ASSERT_EQUAL(2, list[2][0].min());
  CPPUNIT_ASSERT_EQUAL(3, list[2][0].max());
  CPPUNIT_ASSERT_EQUAL(1, list[1][3].min());
  CPPUNIT_ASSERT_EQUAL(8, list[1][3].max());

  CPPUNIT_ASSERT_EQUAL(list.data.getNumLiterals(), list.getNumLiterals());
  CPPUNIT_ASSERT_EQUAL(list.getNumLiterals(), (unsigned int)var);

  CPPUNIT_ASSERT_THROW(list[-1], out_of_range);
  CPPUNIT_ASSERT_THROW(list[3], out_of_range);
  CPPUNIT_ASSERT_THROW(list[1][-1], out_of_range);
  CPPUNIT_ASSERT_THROW(list[1][4], out_of_range);

}


void List2dTest::testConstructionFailure(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  Var var = 0;
  CPPUNIT_ASSERT_THROW(auto list = makeList(dummyManager, 4, -1, builder2d, var), invalid_argument);
  CPPUNIT_ASSERT_THROW(auto list = makeList(dummyManager, -1, 4, builder2d, var), invalid_argument);
  CPPUNIT_ASSERT_THROW(auto list = makeList(dummyManager, -1, -1, builder2d, var), invalid_argument);
}
