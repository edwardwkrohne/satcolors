// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "testglue.h"
#include "../src/objectcontainer.h"
#include "../src/ordinal.h"

using namespace std;

class ObjectContainerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ObjectContainerTest);
  CPPUNIT_TEST(testConstruction);
  CPPUNIT_TEST(testCopyConstruction);
  CPPUNIT_TEST(testAllocateNewConstruction);
  CPPUNIT_TEST(testTypeRequirement);
  CPPUNIT_TEST(testCurrSolnReq);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction(void);
  void testCopyConstruction(void);
  void testAllocateNewConstruction(void);
  void testTypeRequirement(void);
  void testCurrSolnReq(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectContainerTest );

void ObjectContainerTest::testConstruction(void) {
  unsigned int var = 0;
  ObjectContainer<Ordinal> oc;
  CPPUNIT_ASSERT_EQUAL(0, (int)oc.size());

  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);

  CPPUNIT_ASSERT_EQUAL(3, (int)oc.size());

  unsigned int numLiterals = oc[0].numLiterals()*3;

  CPPUNIT_ASSERT_EQUAL(numLiterals, oc.numLiterals());
  CPPUNIT_ASSERT_EQUAL(numLiterals, var);
}

void ObjectContainerTest::testAllocateNewConstruction(void) {
  SolverManager manager;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&manager, 0, 4);
  oc.emplace_back(&manager, 0, 4);
  oc.emplace_back(&manager, 0, 4);

  unsigned int numLiterals = oc[0].numLiterals()*3;

  CPPUNIT_ASSERT_EQUAL(numLiterals, manager.newVars(0));
}

void ObjectContainerTest::testCopyConstruction(void) {
  unsigned int var = 0;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);

  ObjectContainer<Ordinal> cpy = oc;
  CPPUNIT_ASSERT_EQUAL(3, (int)cpy.size());
  CPPUNIT_ASSERT_EQUAL(3, (int)oc.size());

  ObjectContainer<Ordinal> mv = move(oc);
  CPPUNIT_ASSERT_EQUAL(3, (int)mv.size());
  CPPUNIT_ASSERT_EQUAL(0, (int)oc.size());
}

void ObjectContainerTest::testTypeRequirement(void) {
  unsigned int var = 0;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);
  oc.emplace_back(nullptr, 0, 4, var);

  Requirement expected =
    oc[0].typeRequirement() &
    oc[1].typeRequirement() &
    oc[2].typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, oc.typeRequirement());
}

void ObjectContainerTest::testCurrSolnReq(void) {
  SolverManager manager;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&manager, 0, 4);
  oc.emplace_back(&manager, 0, 4);
  oc.emplace_back(&manager, 0, 4);

  ASSERT_SAT(manager);

  DualClause expected =
    oc[0].currSolnReq() &
    oc[1].currSolnReq() &
    oc[2].currSolnReq();

  CPPUNIT_ASSERT_EQUAL(expected, oc.currSolnReq());
}
