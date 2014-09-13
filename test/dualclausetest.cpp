// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/dualclause.h"

using namespace std;

class DualClauseTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(DualClauseTest);
  CPPUNIT_TEST(testCompileTimeTests);
  CPPUNIT_TEST(testOperatorAndLitLit);
  CPPUNIT_TEST(testOperatorAndLitDualClause);
  CPPUNIT_TEST(testOperatorAndEqualsLit);
  CPPUNIT_TEST(testOperatorAndEqualsDualClause);
  CPPUNIT_TEST(testOperatorAndDualClauseLit);
  CPPUNIT_TEST(testOperatorAndDualClauseDualClause);
  CPPUNIT_TEST(testClauseNegation);
  CPPUNIT_TEST(testDualClauseNegation);
  CPPUNIT_TEST(testOutputDualClauseEmpty);
  CPPUNIT_TEST(testOutputDualClauseSingleton);
  CPPUNIT_TEST(testOutputDualClause);
  CPPUNIT_TEST(testDoubleOutput);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCompileTimeTests(void);
  void testOperatorAndLitLit(void);
  void testOperatorAndLitDualClause(void);
  void testOperatorAndEqualsLit(void);
  void testOperatorAndEqualsDualClause(void);
  void testOperatorAndDualClauseLit(void);
  void testOperatorAndDualClauseDualClause(void);
  void testClauseNegation(void);
  void testDualClauseNegation(void);
  void testOutputDualClauseEmpty(void);
  void testOutputDualClauseSingleton(void);
  void testOutputDualClause(void);
  void testDoubleOutput(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( DualClauseTest );

void DualClauseTest::testCompileTimeTests(void) {
  Clause clause = Literal(1) | Literal(2);

  Literal lit1 = Literal(1);
  Literal lit2 = Literal(2);

  DualClause dc = lit1 & lit2;

  dc = lit1 & lit2;
  dc = dc & lit2;
  dc = lit2 & dc;
  dc = dc & dc;

  dc = ~(const Clause)clause;
  clause = ~(const DualClause)dc;
}

void DualClauseTest::testOperatorAndLitLit(void) {
  DualClause expected;
  expected &= Literal(1);
  expected &= Literal(2);
  
  DualClause result = Literal(1) & Literal(2);
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOperatorAndLitDualClause(void) {
  DualClause expected;
  expected &= Literal(1);
  expected &= Literal(2);
  expected &= ~Literal(3);

  const DualClause rhs = Literal(2) & ~Literal(3);
  DualClause result = Literal(1) & move(rhs);
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOperatorAndEqualsLit(void) {
  DualClause expected;
  expected &= ~Literal(1);
  expected &= Literal(2);
  expected &= ~Literal(3);

  DualClause result = ~Literal(1) & Literal(2);
  result &= ~Literal(3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOperatorAndEqualsDualClause(void) {
  DualClause expected;
  expected &= ~Literal(1);
  expected &= Literal(2);
  expected &= ~Literal(3);
  expected &= ~Literal(4);

  DualClause result = ~Literal(1) & Literal(2);
  const DualClause rhs = ~Literal(3) & ~Literal(4);
  result &= rhs;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOperatorAndDualClauseLit(void) {
  DualClause expected;
  expected &= ~Literal(1);
  expected &= Literal(2);
  expected &= ~Literal(3);

  const DualClause lhs = ~Literal(1) & Literal(2);
  DualClause result = lhs & ~Literal(3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOperatorAndDualClauseDualClause(void) {
  DualClause expected;
  expected &= ~Literal(1);
  expected &= Literal(2);
  expected &= ~Literal(3);
  expected &= ~Literal(4);

  const DualClause lhs = ~Literal(1) & Literal(2);
  const DualClause rhs = ~Literal(3) & ~Literal(4);
  DualClause result = lhs & rhs;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testOutputDualClauseEmpty(void) {
  string expected = "";
  const DualClause clause;

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void DualClauseTest::testClauseNegation(void) {
  Literal lit1 = Literal(1);
  Literal lit2 = Literal(2);
  Literal lit3 = Literal(3);

  Clause clause = lit1 | lit2 | lit3;
  DualClause result(~clause);

  DualClause expected(~lit1 & ~lit2 & ~lit3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void DualClauseTest::testDualClauseNegation(void) {
  Literal lit1 = Literal(1);
  Literal lit2 = Literal(2);
  Literal lit3 = Literal(3);

  DualClause clause = lit1 & lit2 & lit3;
  Clause result(~clause);

  Clause expected(~lit1 | ~lit2 | ~lit3);

  CPPUNIT_ASSERT_EQUAL(expected.size(), result.size());
  CPPUNIT_ASSERT_EQUAL(expected, result);
}


void DualClauseTest::testOutputDualClauseSingleton(void) {
  string expected = "~3";
  const DualClause clause(~Literal(3));

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void DualClauseTest::testOutputDualClause(void) {
  string expected = "1 & 2 & ~3 & 4";
  const DualClause clause = (Literal(1) & Literal(2) & ~Literal(3) & Literal(4));

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void DualClauseTest::testDoubleOutput(void) {
  const DualClause clause = (Literal(1) & Literal(2) & ~Literal(3) & Literal(4));

  ostringstream resultStream;
  CPPUNIT_ASSERT_NO_THROW(resultStream << clause << clause);
}
