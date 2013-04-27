// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../src/list.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

class ListTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ListTest);
  CPPUNIT_TEST(testConstruction);
  CPPUNIT_TEST(testConstructionFailure);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction(void);
  void testConstructionFailure(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( ListTest );

namespace {

typedef Scalar::value_type value_type;

Scalar builder(SolverManager& manager, value_type index, Var& var) {
  return Scalar(manager, 0, 2*index+1, var);
}
}

void ListTest::testConstruction(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  // Object under test.
  Var var = 0;
  auto list = makeList(dummyManager, 3, builder, var);

  // Just hit it with a battery of checks.
  CPPUNIT_ASSERT_EQUAL(3, list.size);
  CPPUNIT_ASSERT_EQUAL(0, list[0].min);
  CPPUNIT_ASSERT_EQUAL(1, list[0].max);
  CPPUNIT_ASSERT_EQUAL(0, list[1].min);
  CPPUNIT_ASSERT_EQUAL(3, list[1].max);
  CPPUNIT_ASSERT_EQUAL(0, list[2].min);
  CPPUNIT_ASSERT_EQUAL(5, list[2].max);

  CPPUNIT_ASSERT_EQUAL(list[0].getNumLiterals(),                                                       (unsigned int)list[1].startingVar);
  CPPUNIT_ASSERT_EQUAL(list[0].getNumLiterals() + list[1].getNumLiterals(),                            (unsigned int)list[2].startingVar);
  CPPUNIT_ASSERT_EQUAL(list[0].getNumLiterals() + list[1].getNumLiterals() + list[2].getNumLiterals(), list.getNumLiterals());

  CPPUNIT_ASSERT_EQUAL(list.getNumLiterals(), (unsigned int)var);
  CPPUNIT_ASSERT_EQUAL(list[0].typeRequirement() & list[1].typeRequirement() & list[2].typeRequirement(), list.typeRequirement());

  CPPUNIT_ASSERT_THROW(list[-1], out_of_range);
  CPPUNIT_ASSERT_THROW(list[3], out_of_range);
}

void ListTest::testConstructionFailure(void) {
  // This exceedingly crude "dummy object" will segfault on any attempted use.
  // I'm ok with that.  Use something more sophisticated if desired.
  SolverManager& dummyManager = *(SolverManager*)0;

  Var var = 0;
  CPPUNIT_ASSERT_THROW(auto list = makeList(dummyManager, -1, builder, var), invalid_argument);
}
