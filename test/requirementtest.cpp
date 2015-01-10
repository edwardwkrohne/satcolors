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
#include "../src/requirement.h"

using namespace std;
typedef Requirement::size_type size_type;

class RequirementTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(RequirementTest);
  CPPUNIT_TEST(testCompileTimeTests);
  CPPUNIT_TEST(testOperatorAndClauseClause);
  CPPUNIT_TEST(testCopyOnInsert);
  CPPUNIT_TEST(testInstantiation);
  CPPUNIT_TEST(testClauseDisjunction);
  CPPUNIT_TEST(testDualClauseAndClauseDisjunction);
  CPPUNIT_TEST(testDualClauseAndDualClauseDisjunction);
  CPPUNIT_TEST(testDualClauseAndClauseConjunction);
  CPPUNIT_TEST(testRequirementDisjunction);
  CPPUNIT_TEST(testClauseNegation);
  CPPUNIT_TEST(testMoveConstruction);
  CPPUNIT_TEST(testImplication);
  CPPUNIT_TEST(testEquivalence);
  CPPUNIT_TEST(testOutput);
  CPPUNIT_TEST(testOutput2);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCompileTimeTests(void);
  void testOperatorAndClauseClause(void);
  void testCopyOnInsert(void);
  void testInstantiation(void);
  void testClauseDisjunction(void);
  void testDualClauseAndClauseDisjunction(void);
  void testDualClauseAndDualClauseDisjunction(void);
  void testDualClauseAndClauseConjunction(void);
  void testRequirementDisjunction(void);
  void testClauseNegation(void);
  void testMoveConstruction(void);
  void testEquivalence(void);
  void testImplication(void);
  void testOutput(void);
  void testOutput2(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( RequirementTest );

// TODO Use a special allocator to test move semantics.
// Using an allocator, it may be possible and elegant to detect when
// deallocations occur, and then count them for the purposes of testing.

void RequirementTest::testCompileTimeTests(void) {
  const Clause clause1 = Literal(1) | Literal(2);
  const Clause clause2 = Literal(2) | Literal(3);
  const Clause clause3 = Literal(3) | Literal(4);

  const DualClause dClause1 = Literal(1) & Literal(2);
  const Literal lit1 = Literal(1);
  const Literal lit2 = Literal(2);

  const Requirement req1 = clause1 & clause2;
  const Requirement req2 = clause2 & clause3;

  Requirement req;
  DualClause dc = lit1 & lit2;

  req = req1;
  req = req1 & req2;
  req = clause1 & req2;
  req = lit1 & req2;
  req = req1 & clause2;
  req = clause1 & clause2;
  req = lit1 & clause2;
  req = req1 & lit2;
  req = clause1 & lit2;

  req = req1 & clause1 & lit1;
  req = clause1 & clause2 & clause3;
  req = (Literal(1) | Literal(2)) & clause1;

  req = req1 & dClause1;
  req = req1 | dClause1;
  req = dClause1 & req1;
  req = dClause1 | req1;
  req = dClause1 & dClause1 & req1;
  req = dClause1 & req1 & dClause1;
  req = req & dClause1 & dClause1;
  req = dClause1 | dClause1 | req1;
  req = dClause1 | req1 | dClause1;
  req = req | dClause1 | dClause1;
  req = dClause1 | dClause1;

  req = dClause1 | lit1 | lit2;
  req = lit1 | dClause1 | lit2;
  req = lit1 | lit2 | dClause1;

  req |= dClause1;
  req &= dClause1;
}

void RequirementTest::testOperatorAndClauseClause(void) {
  Clause clause1 = Literal(1) | Literal(2);
  Clause clause2 = Literal(2) | Literal(3);

  Requirement expected;
  expected &= clause1;
  expected &= clause2;
  
  Requirement result = clause1 & clause2;
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testCopyOnInsert(void) {
  Clause clause1 = Literal(1) | Literal(2);

  Requirement expected;
  expected &= Clause(Literal(1) | Literal(2));
  
  Requirement result(clause1);
  clause1 |= Literal(3);
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testInstantiation(void) {
  Clause clause1 = Literal(1) | Literal(2);

  Literal lit1 = Literal(1);

  Requirement req1(lit1);

  { Requirement req(lit1); };
  { Requirement req(clause1); };
  { Requirement req(req1); };

  { Requirement req = Requirement(lit1); };
  { Requirement req = Requirement(clause1); };
  { Requirement req = req1; };
  { Requirement req = Requirement(req1); };
}

void RequirementTest::testClauseDisjunction(void) {
  Clause clause1 = Literal(1) | Literal(2);
  Clause clause2 = Literal(3) | Literal(4);
  Clause clause3 = Literal(5) | Literal(6);

  Requirement expected;
  expected &= clause1 | clause2;
  expected &= clause1 | clause3;

  Requirement result = clause2 & clause3;
  result |= clause1;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndClauseDisjunction(void) {
  Clause     clause = Literal(1)  | Literal(2);
  DualClause dClause = Literal(3) & Literal(4);

  Requirement expected;
  expected &= clause | Literal(3);
  expected &= clause | Literal(4);

  Requirement result = clause | dClause;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndClauseConjunction(void) {
  Clause     clause = Literal(1)  | Literal(2);
  DualClause dClause = Literal(3) & Literal(4);

  Requirement expected;
  expected &= clause;
  expected &= Literal(3);
  expected &= Literal(4);

  Requirement result = clause & dClause;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndDualClauseDisjunction(void) {
  DualClause dClause1 = Literal(1) & Literal(2);
  DualClause dClause2 = Literal(3) & Literal(4);

  Requirement expected;
  expected &= Literal(1) | Literal(3);
  expected &= Literal(1) | Literal(4);
  expected &= Literal(2) | Literal(3);
  expected &= Literal(2) | Literal(4);

  Requirement result = dClause1 | dClause2;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testRequirementDisjunction(void) {
  Clause clause1 = Literal(1) | Literal(2);
  Clause clause2 = Literal(3) | Literal(4);
  Clause clause3 = Literal(5) | Literal(6);
  Clause clause4 = Literal(7) | Literal(8);

  Requirement expected;
  expected &= clause1 | clause3;
  expected &= clause1 | clause4;
  expected &= clause2 | clause3;
  expected &= clause2 | clause4;

  CPPUNIT_ASSERT_EQUAL(expected, (clause1 & clause2) | (clause3 & clause4));
}

void RequirementTest::testClauseNegation(void) {
  Literal lit1 = Literal(1);
  Literal lit2 = Literal(2);
  Literal lit3 = Literal(3);

  Clause clause = lit1 | lit2 | lit3;
  Requirement result(~clause);

  Requirement expected(~lit1 & ~lit2 & ~lit3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testMoveConstruction(void) {
  Requirement req = Literal(1) & Literal(2);
  Requirement result = move(req);

  Requirement expected = Literal(1) & Literal(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testImplication(void) {
  Requirement req = Literal(1) & Literal(2);
  Requirement result = implication(Literal(3), move(req));
  Requirement expected =
      (~Literal(3) | Literal(1)) &
      (~Literal(3) | Literal(2));

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testEquivalence(void) {
  Requirement result = equivalence(Literal(3), Literal(1) & Literal(2));
  Requirement expected =
      (~Literal(3) | Literal(1)) &
      (~Literal(3) | Literal(2)) &
      (~Literal(1) | ~Literal(2) | Literal(3));

  CPPUNIT_ASSERT_EQUAL(expected, result);
}


void RequirementTest::testOutput(void) {
  ostringstream sout;
  Clause clause = Literal(1) | Literal(2);
  Requirement req(clause);
  sout << req;

  string expected = "(1 | 2)";

  CPPUNIT_ASSERT_EQUAL(expected, sout.str());
}

void RequirementTest::testOutput2(void) {
  ostringstream sout;
  Requirement req(Literal(1));
  sout << req;

  string expected = "1";

  CPPUNIT_ASSERT_EQUAL(expected, sout.str());
}
