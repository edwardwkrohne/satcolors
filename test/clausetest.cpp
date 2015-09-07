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
  CPPUNIT_TEST(testDisjoinTrueFalseClause);
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
  void testDisjoinTrueFalseClause(void);
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

  // test interaction with atoms while we're doing this; lazy hack onto an old test.
  const Clause lhs = ~Literal(1) | Atom(Literal(2));
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

void ClauseTest::testDisjoinTrueFalseClause(void) {
  Clause clause = Literal(1) | Literal(2);

  // Truth dominates the clause
  CPPUNIT_ASSERT_EQUAL(Clause::truth, clause | Clause::truth);
  CPPUNIT_ASSERT_EQUAL(Clause::truth, clause | Atom::truth);
  CPPUNIT_ASSERT_EQUAL(Clause::truth, Clause::truth | clause);
  CPPUNIT_ASSERT_EQUAL(Clause::truth, Clause::truth | Literal(3));
  CPPUNIT_ASSERT_EQUAL(Clause::truth, Atom::truth | Literal(3));

  // Falsity preserves the clause
  CPPUNIT_ASSERT_EQUAL(clause,        clause | Clause::falsity);
  CPPUNIT_ASSERT_EQUAL(clause,        clause | Atom::falsity);

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
