// -*- Mode: C++ -*-
////////////////////////////////////////////////////////////////////////////
//
//  satcolors -- for writably and readably building problems for SAT-solving
//  Copyright (C) 2014 Edward W. Krohne III
//
//  The research that led to this product was partially supported by the
//  U.S. NSF grants DMS-0943870 and DMS-1201290.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "mocksolver.h"
#include "testglue.h"
#include "../src/minisatsolver.h"
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
  MockSolver solver;
  ObjectContainer<Ordinal> oc;
  CPPUNIT_ASSERT_EQUAL(0, (int)oc.size());

  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);

  CPPUNIT_ASSERT_EQUAL(3, (int)oc.size());

  unsigned int numLiterals = oc[0].numLiterals()*3;

  CPPUNIT_ASSERT_EQUAL(numLiterals, oc.numLiterals());
}

void ObjectContainerTest::testAllocateNewConstruction(void) {
  MinisatSolver solver;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);

  unsigned int numLiterals = oc[0].numLiterals()*3;

  CPPUNIT_ASSERT_EQUAL(numLiterals, solver.newVars(0));
}

void ObjectContainerTest::testCopyConstruction(void) {
  MockSolver solver;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);

  ObjectContainer<Ordinal> cpy = oc;
  CPPUNIT_ASSERT_EQUAL(3, (int)cpy.size());
  CPPUNIT_ASSERT_EQUAL(3, (int)oc.size());

  ObjectContainer<Ordinal> mv = move(oc);
  CPPUNIT_ASSERT_EQUAL(3, (int)mv.size());
  CPPUNIT_ASSERT_EQUAL(0, (int)oc.size());
}

void ObjectContainerTest::testTypeRequirement(void) {
  MockSolver solver;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);

  Requirement expected =
    oc[0].typeRequirement() &
    oc[1].typeRequirement() &
    oc[2].typeRequirement();

  CPPUNIT_ASSERT_EQUAL(expected, oc.typeRequirement());
}

void ObjectContainerTest::testCurrSolnReq(void) {
  MinisatSolver solver;
  ObjectContainer<Ordinal> oc;

  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);
  oc.emplace_back(&solver, 0, 4);

  ASSERT_SAT(solver);

  DualClause expected =
    oc[0].currSolnReq() &
    oc[1].currSolnReq() &
    oc[2].currSolnReq();

  CPPUNIT_ASSERT_EQUAL(expected, oc.currSolnReq());
 }
