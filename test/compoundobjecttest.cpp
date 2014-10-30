// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "testglue.h"
#include "../src/compoundobject.h"
#include "../src/minisatsolver.h"

using namespace std;

class CompoundObjectTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CompoundObjectTest);
  CPPUNIT_TEST(testLastArg);
  CPPUNIT_TEST(testExplicitVarConstruction);
  CPPUNIT_TEST(testCopyConstruction);
  CPPUNIT_TEST(testNoVarConstruction);
  CPPUNIT_TEST(testAllocateNewConstruction);
  CPPUNIT_TEST(testGetNumLiterals);
  CPPUNIT_TEST(testTypeRequirement);
  CPPUNIT_TEST(testCurrSolnReq);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testLastArg(void);
  void testExplicitVarConstruction(void);
  void testCopyConstruction(void);
  void testNoVarConstruction(void);
  void testAllocateNewConstruction(void);
  void testGetNumLiterals(void);
  void testTypeRequirement(void);
  void testCurrSolnReq(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( CompoundObjectTest );

namespace {

class TestImpl {
public:
  TestImpl(Solver* solver, unsigned int& var = Solver::allocateNew);

  Clause diffSolnReq() const;
  Requirement typeRequirement() const;

  Solver* solver;
  Cardinal cardinal1;
  Cardinal cardinal2;
  Matrix<> matrix1;

  tuple<Cardinal&, Cardinal&, Matrix<>&> objTuple;
};

typedef CompoundObject<TestImpl> Test;

TestImpl::TestImpl(Solver* _solver, unsigned int& var) :
  solver(_solver),
  cardinal1(solver, 0, 5, var),
  cardinal2(solver, 2, 3, var),
  matrix1(solver, 3, 5, 0, 2, var),
  objTuple(cardinal1, cardinal2, matrix1)
{

}

// Easy to test but basically arbitrary definitions of these functions.
Clause TestImpl::diffSolnReq() const {
  return cardinal1.diffSolnReq();
}

Requirement TestImpl::typeRequirement() const {
  return cardinal1 == cardinal2;
}

}

template<class T, class... U>
T& testProject(T& t, U&... u) {
  return t;
}

template<class... T>
auto testForward(T&&... t) -> decltype(testProject(t...)) {
  return testProject(t...);
}

void CompoundObjectTest::testLastArg(void) {
  // TODO This test should probably be moved to its own test class.
  unsigned int var = 0;
  char c = 0;

  static_assert(is_same<unsigned int&, decltype(lastArg(c,c,c,c,c,var))>::value, "Last argument should return a unsigned int&.");

  unsigned int& result = lastArg(c, var);
  unsigned int& result2 = lastArg(c, c, c, var);
  CPPUNIT_ASSERT_EQUAL(&var, &result);
  CPPUNIT_ASSERT_EQUAL(&var, &result2);
}

void CompoundObjectTest::testExplicitVarConstruction(void) {
  MinisatSolver solver;
  unsigned int var = 0;
  Test testObj(&solver, var);

  // Require the exact opposite of the requirement that testObj would
  // have required, if we had allowed it to.
  solver.newVars(testObj.numLiterals());
  solver.require(testObj.cardinal1 != testObj.cardinal2);

  ASSERT_SAT(solver);
}

void CompoundObjectTest::testCopyConstruction(void) {
  // These static asserts essentially test implementation.  The particular classes used
  // such as CopyConstructorArgs, etc., are implementation details.  Nonetheless, I
  // will probably be forgiven for testing them, given their newness and difficulty.
  static_assert(CopyConstructorArgs<TestImpl, Test>::value, "Test failed");
  static_assert(CopyConstructorArgs<TestImpl, Test&>::value, "Test failed");
  static_assert(CopyConstructorArgs<TestImpl, const Test&>::value, "Test failed");
  static_assert(CopyConstructorArgs<TestImpl, Test&&>::value, "Test failed");
  static_assert(!CopyConstructorArgs<TestImpl, int, Test&&>::value, "Test failed");

  static_assert(!ExplicitVarArgs<TestImpl, Test>::value, "Test failed");
  static_assert(!ImplicitVarArgs<TestImpl, Test>::value, "Test failed");

  MinisatSolver solver;
  unsigned int var = 0;
  Test testObj(&solver, var);
  const Test& testObjConst = testObj;

  Test testObjCopy(testObj);
  CPPUNIT_ASSERT_EQUAL((Solver*)&solver, testObjCopy.solver);

  Test testObjConstCopy(testObjConst);
  CPPUNIT_ASSERT_EQUAL((Solver*)&solver, testObjConstCopy.solver);

  Test testObjMove(move(testObj));
  CPPUNIT_ASSERT_EQUAL((Solver*)&solver, testObjMove.solver);

  // Require the exact opposite of the requirement that testObj or its copies
  // would have required, if any were allowed to.
  solver.newVars(testObj.numLiterals());
  solver.require(testObj.cardinal1 != testObj.cardinal2);

  ASSERT_SAT(solver);
}

void CompoundObjectTest::testNoVarConstruction(void) {
  MinisatSolver solver;
  Test testObj(&solver);

  // Require the exact opposite of the requirement that testObj should
  // have required.
  solver.require(testObj.cardinal1 != testObj.cardinal2);

  ASSERT_UNSAT(solver, "");
}


void CompoundObjectTest::testAllocateNewConstruction(void) {
  MinisatSolver solver;
  Test testObj(&solver, Solver::allocateNew);

  // Require the exact opposite of the requirement that testObj should
  // have required
  solver.require(testObj.cardinal1 != testObj.cardinal2);

  ASSERT_UNSAT(solver, "");
}

void CompoundObjectTest::testGetNumLiterals(void) {
  unsigned int var = 0;
  Test testObj(nullptr, var);

  int expected =
      testObj.cardinal1.numLiterals() +
      testObj.cardinal2.numLiterals() +
      testObj.matrix1.numLiterals();


  CPPUNIT_ASSERT_EQUAL(expected, testObj.numLiterals());
}

void CompoundObjectTest::testTypeRequirement(void) {
  unsigned int var = 0;
  Test testObj(nullptr, var);

  Requirement expected =
      testObj.cardinal1.typeRequirement() &
      testObj.cardinal2.typeRequirement() &
      testObj.matrix1.typeRequirement() &
      testObj.TestImpl::typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, testObj.typeRequirement());
}

void CompoundObjectTest::testCurrSolnReq(void) {
  MinisatSolver solver;
  Test testObj(&solver);

  ASSERT_SAT(solver);

  DualClause expected = ~testObj.TestImpl::diffSolnReq();

  CPPUNIT_ASSERT_EQUAL(expected, testObj.currSolnReq());
}
