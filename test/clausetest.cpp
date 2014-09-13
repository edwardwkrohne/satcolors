// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/clause.h"

using namespace std;

class ClauseTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ClauseTest);
  CPPUNIT_TEST(testOperatorOrLitLit);
  CPPUNIT_TEST(testOperatorOrLitClause);
  CPPUNIT_TEST(testOperatorOrEqualsLit);
  CPPUNIT_TEST(testOperatorOrEqualsClause);
  CPPUNIT_TEST(testOperatorOrClauseLit);
  CPPUNIT_TEST(testOperatorOrClauseClause);
  CPPUNIT_TEST(testOutputLitPositive);
  CPPUNIT_TEST(testOutputLitNegative);
  CPPUNIT_TEST(testOutputClauseEmpty);
  CPPUNIT_TEST(testOutputClauseSingleton);
  CPPUNIT_TEST(testOutputClause);
  CPPUNIT_TEST(testDoubleOutput);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testOperatorOrLitLit(void);
  void testOperatorOrLitClause(void);
  void testOperatorOrEqualsLit(void);
  void testOperatorOrEqualsClause(void);
  void testOperatorOrClauseLit(void);
  void testOperatorOrClauseClause(void);
  void testOutputLitPositive(void);
  void testOutputLitNegative(void);
  void testOutputClauseEmpty(void);
  void testOutputClauseSingleton(void);
  void testOutputClause(void);
  void testDoubleOutput(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( ClauseTest );

void ClauseTest::testOperatorOrLitLit(void) {
  Clause expected;
  expected |= Literal(1);
  expected |= Literal(2);
  
  Clause result = Literal(1) | Literal(2);
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOperatorOrLitClause(void) {
  Clause expected;
  expected |= Literal(1);
  expected |= Literal(2);
  expected |= ~Literal(3);

  const Clause rhs = Literal(2) | ~Literal(3);
  Clause result = Literal(1) | rhs;
  
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOperatorOrEqualsLit(void) {
  Clause expected;
  expected |= ~Literal(1);
  expected |= Literal(2);
  expected |= ~Literal(3);

  Clause result = ~Literal(1) | Literal(2);
  result |= ~Literal(3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOperatorOrEqualsClause(void) {
  Clause expected;
  expected |= ~Literal(1);
  expected |= Literal(2);
  expected |= ~Literal(3);
  expected |= ~Literal(4);

  Clause result = ~Literal(1) | Literal(2);
  const Clause rhs = ~Literal(3) | ~Literal(4);
  result |= rhs;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOperatorOrClauseLit(void) {
  Clause expected;
  expected |= ~Literal(1);
  expected |= Literal(2);
  expected |= ~Literal(3);

  const Clause lhs = ~Literal(1) | Literal(2);
  Clause result = lhs | ~Literal(3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOperatorOrClauseClause(void) {
  Clause expected;
  expected |= ~Literal(1);
  expected |= Literal(2);
  expected |= ~Literal(3);
  expected |= ~Literal(4);

  const Clause lhs = ~Literal(1) | Literal(2);
  const Clause rhs = ~Literal(3) | ~Literal(4);
  Clause result = lhs | rhs;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void ClauseTest::testOutputLitPositive(void) {
  string expected = "1";

  ostringstream resultStream;
  resultStream << Literal(1);
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void ClauseTest::testOutputLitNegative(void) {
  string expected = "~2";

  ostringstream resultStream;
  resultStream << ~Literal(2);
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void ClauseTest::testOutputClauseEmpty(void) {
  string expected = "";
  const Clause clause;

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void ClauseTest::testOutputClauseSingleton(void) {
  string expected = "~3";
  const Clause clause(~Literal(3));

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void ClauseTest::testOutputClause(void) {
  string expected = "1 | 2 | ~3 | 4";
  Clause clause = (Literal(1) | Literal(2) | ~Literal(3) | Literal(4));

  ostringstream resultStream;
  resultStream << clause;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void ClauseTest::testDoubleOutput(void) {
  const Clause clause = (Literal(1) | Literal(2) | ~Literal(3) | Literal(4));

  ostringstream resultStream;
  CPPUNIT_ASSERT_NO_THROW(resultStream << clause << clause);
}
