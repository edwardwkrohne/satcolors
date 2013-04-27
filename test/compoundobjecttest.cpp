// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/compoundobject.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
using Minisat::Var;

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
  // TODO Remove the requirement that the implementation class supply a default argument
  // (not terribly important)
  TestImpl(SolverManager& manager, Var& var = SolverManager::allocateNew);

  Clause diffSolnReq() const;
  Requirement typeRequirement() const;

  SolverManager& manager;
  Index index1;
  Index index2;
  Matrix matrix1;

  tuple<Index&, Index&, Matrix&> objTuple;
};

typedef CompoundObject<TestImpl> Test;

TestImpl::TestImpl(SolverManager& _manager, Var& var) :
  manager(_manager),
  index1(manager, 0, 5, var),
  index2(manager, 2, 3, var),
  matrix1(manager, 3, 5, 0, 2, var),
  objTuple(index1, index2, matrix1)
{

}

// Easy to test but basically arbitrary definitions of these functions.
Clause TestImpl::diffSolnReq() const {
  return index1.diffSolnReq();
}

Requirement TestImpl::typeRequirement() const {
  return index1 == index2;
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
  Var var = 0;
  char c = 0;

  static_assert(is_same<Var&, decltype(lastArg(c,c,c,c,c,var))>::value, "Last argument should return a Var&.");

  Var& result = lastArg(c, var);
  Var& result2 = lastArg(c, c, c, var);
  CPPUNIT_ASSERT_EQUAL(&var, &result);
  CPPUNIT_ASSERT_EQUAL(&var, &result2);
}

void CompoundObjectTest::testExplicitVarConstruction(void) {
  SolverManager manager;
  Var var = 0;
  Test testObj(manager, var);

  // Require the exact opposite of the requirement that testObj would
  // have required, if we had allowed it to.
  manager.require(testObj.index1 != testObj.index2);

  ASSERT_SAT(manager);
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

  SolverManager manager;
  Var var = 0;
  Test testObj(manager, var);
  const Test& testObjConst = testObj;

  Test testObjCopy(testObj);
  CPPUNIT_ASSERT_EQUAL(&manager, &testObjCopy.manager);

  Test testObjConstCopy(testObjConst);
  CPPUNIT_ASSERT_EQUAL(&manager, &testObjConstCopy.manager);

  Test testObjMove(move(testObj));
  CPPUNIT_ASSERT_EQUAL(&manager, &testObjMove.manager);

  // Require the exact opposite of the requirement that testObj or its copies
  // would have required, if any were allowed to.
  manager.require(testObj.index1 != testObj.index2);

  ASSERT_SAT(manager);
}

void CompoundObjectTest::testNoVarConstruction(void) {
  SolverManager manager;
  Test testObj(manager);

  // Require the exact opposite of the requirement that testObj should
  // have required.
  manager.require(testObj.index1 != testObj.index2);

  ASSERT_UNSAT(manager, "");
}


void CompoundObjectTest::testAllocateNewConstruction(void) {
  SolverManager manager;
  Test testObj(manager, SolverManager::allocateNew);

  // Require the exact opposite of the requirement that testObj should
  // have required
  manager.require(testObj.index1 != testObj.index2);

  ASSERT_UNSAT(manager, "");
}

void CompoundObjectTest::testGetNumLiterals(void) {
  Var var = 0;
  SolverManager& manager = *(SolverManager*)0;
  Test testObj(manager, var);

  Matrix::value_type expected =
      testObj.index1.getNumLiterals() +
      testObj.index2.getNumLiterals() +
      testObj.matrix1.getNumLiterals();


  CPPUNIT_ASSERT_EQUAL(expected, testObj.getNumLiterals());
}

void CompoundObjectTest::testTypeRequirement(void) {
  Var var = 0;
  SolverManager& manager = *(SolverManager*)0;
  Test testObj(manager, var);

  Requirement expected =
      testObj.index1.typeRequirement() &
      testObj.index2.typeRequirement() &
      testObj.matrix1.typeRequirement() &
      testObj.TestImpl::typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, testObj.typeRequirement());
}

void CompoundObjectTest::testCurrSolnReq(void) {
  SolverManager manager;
  Test testObj(manager);

  ASSERT_SAT(manager);

  DualClause expected = ~testObj.TestImpl::diffSolnReq();

  CPPUNIT_ASSERT_EQUAL(expected, testObj.currSolnReq());
}
