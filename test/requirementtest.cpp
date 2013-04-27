// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/requirement.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;
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
  const Clause clause1 = mkLit(1) | mkLit(2);
  const Clause clause2 = mkLit(2) | mkLit(3);
  const Clause clause3 = mkLit(3) | mkLit(4);

  const DualClause dClause1 = mkLit(1) & mkLit(2);
  const Lit lit1 = mkLit(1);
  const Lit lit2 = mkLit(2);

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
  req = (mkLit(1) | mkLit(2)) & clause1;

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
  Clause clause1 = mkLit(1) | mkLit(2);
  Clause clause2 = mkLit(2) | mkLit(3);

  Requirement expected;
  expected &= clause1;
  expected &= clause2;
  
  Requirement result = clause1 & clause2;
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testCopyOnInsert(void) {
  Clause clause1 = mkLit(1) | mkLit(2);

  Requirement expected;
  expected &= Clause(mkLit(1) | mkLit(2));
  
  Requirement result(clause1);
  clause1 |= mkLit(3);
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testInstantiation(void) {
  Clause clause1 = mkLit(1) | mkLit(2);

  Lit lit1 = mkLit(1);

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
  Clause clause1 = mkLit(1) | mkLit(2);
  Clause clause2 = mkLit(3) | mkLit(4);
  Clause clause3 = mkLit(5) | mkLit(6);

  Requirement expected;
  expected &= clause1 | clause2;
  expected &= clause1 | clause3;

  Requirement result = clause2 & clause3;
  result |= clause1;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndClauseDisjunction(void) {
  Clause     clause = mkLit(1)  | mkLit(2);
  DualClause dClause = mkLit(3) & mkLit(4);

  Requirement expected;
  expected &= clause | mkLit(3);
  expected &= clause | mkLit(4);

  Requirement result = clause | dClause;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndClauseConjunction(void) {
  Clause     clause = mkLit(1)  | mkLit(2);
  DualClause dClause = mkLit(3) & mkLit(4);

  Requirement expected;
  expected &= clause;
  expected &= mkLit(3);
  expected &= mkLit(4);

  Requirement result = clause & dClause;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testDualClauseAndDualClauseDisjunction(void) {
  DualClause dClause1 = mkLit(1) & mkLit(2);
  DualClause dClause2 = mkLit(3) & mkLit(4);

  Requirement expected;
  expected &= mkLit(1) | mkLit(3);
  expected &= mkLit(1) | mkLit(4);
  expected &= mkLit(2) | mkLit(3);
  expected &= mkLit(2) | mkLit(4);

  Requirement result = dClause1 | dClause2;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testRequirementDisjunction(void) {
  Clause clause1 = mkLit(1) | mkLit(2);
  Clause clause2 = mkLit(3) | mkLit(4);
  Clause clause3 = mkLit(5) | mkLit(6);
  Clause clause4 = mkLit(7) | mkLit(8);

  Requirement expected;
  expected &= clause1 | clause3;
  expected &= clause1 | clause4;
  expected &= clause2 | clause3;
  expected &= clause2 | clause4;

  CPPUNIT_ASSERT_EQUAL(expected, (clause1 & clause2) | (clause3 & clause4));
}

void RequirementTest::testClauseNegation(void) {
  Lit lit1 = mkLit(1);
  Lit lit2 = mkLit(2);
  Lit lit3 = mkLit(3);

  Clause clause = lit1 | lit2 | lit3;
  Requirement result(~clause);

  Requirement expected(~lit1 & ~lit2 & ~lit3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testMoveConstruction(void) {
  Requirement req = mkLit(1) & mkLit(2);
  Requirement result = move(req);

  Requirement expected = mkLit(1) & mkLit(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testImplication(void) {
  Requirement req = mkLit(1) & mkLit(2);
  Requirement result = implication(mkLit(3), move(req));
  Requirement expected =
      (~mkLit(3) | mkLit(1)) &
      (~mkLit(3) | mkLit(2));

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void RequirementTest::testEquivalence(void) {
  Requirement result = equivalence(mkLit(3), mkLit(1) & mkLit(2));
  Requirement expected =
      (~mkLit(3) | mkLit(1)) &
      (~mkLit(3) | mkLit(2)) &
      (~mkLit(1) | ~mkLit(2) | mkLit(3));

  CPPUNIT_ASSERT_EQUAL(expected, result);
}


void RequirementTest::testOutput(void) {
  ostringstream sout;
  Clause clause = mkLit(1) | mkLit(2);
  Requirement req(clause);
  sout << req;

  string expected = "(1 | 2)";

  CPPUNIT_ASSERT_EQUAL(expected, sout.str());
}

void RequirementTest::testOutput2(void) {
  ostringstream sout;
  Requirement req(mkLit(1));
  sout << req;

  string expected = "1";

  CPPUNIT_ASSERT_EQUAL(expected, sout.str());
}
