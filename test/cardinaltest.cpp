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
#include <stdexcept>
#include "mocksolver.h"
#include "../src/cardinal.h"

using namespace std;

class CardinalTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CardinalTest);
  CPPUNIT_TEST(testCopy);
  CPPUNIT_TEST(testIsCardinal);
  CPPUNIT_TEST(testNonzeroMin);
  CPPUNIT_TEST(testEqualsValue);
  CPPUNIT_TEST(testEqualsCardinal);
  CPPUNIT_TEST(testEqualsCardinal2);
  CPPUNIT_TEST(testEqualsCardinal3);
  CPPUNIT_TEST(testNotEqualCardinal);
  CPPUNIT_TEST(testNotEqualCardinal2);
  CPPUNIT_TEST(testAddition);
  CPPUNIT_TEST(testGreaterThan);
  CPPUNIT_TEST(testRedundantGreaterThan);
  CPPUNIT_TEST(testLessThan);
  CPPUNIT_TEST(testRedundantLessThan);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST(testNegation);
  CPPUNIT_TEST(testNegNeg);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCopy(void);
  void testIsCardinal(void);
  void testNonzeroMin(void);
  void testEqualsValue(void);
  void testEqualsCardinal(void);
  void testEqualsCardinal2(void);
  void testEqualsCardinal3(void);
  void testNotEqualCardinal(void);
  void testNotEqualCardinal2(void);
  void testAddition(void);
  void testGreaterThan(void);
  void testRedundantGreaterThan(void);
  void testLessThan(void);
  void testRedundantLessThan(void);
  void testDomainError(void);
  void testNegation(void);
  void testNegNeg(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( CardinalTest );

void CardinalTest::testCopy(void) {
  MockSolver solver;
  Cardinal card(&solver, 0, 4);
  Cardinal cpy(card);
  Cardinal mv(std::move(card));

  cpy = card;
  mv = std::move(card);
}

void CardinalTest::testIsCardinal(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 3);
  Requirement result = cardinal.typeRequirement();

  Requirement expected;
  expected &= Literal(0) | Literal(1) | Literal(2);
  expected &= ~Literal(0) | ~Literal(1);
  expected &= ~Literal(0) | ~Literal(2);
  expected &= ~Literal(1) | ~Literal(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void CardinalTest::testNonzeroMin(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 2, 5);
  Requirement result = cardinal.typeRequirement();

  Requirement expected;
  expected &= Literal(0) | Literal(1) | Literal(2);
  expected &= ~Literal(0) | ~Literal(1);
  expected &= ~Literal(0) | ~Literal(2);
  expected &= ~Literal(1) | ~Literal(2);

  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsValue(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 3);

  CPPUNIT_ASSERT_EQUAL(Literal(0), cardinal == 0);
  CPPUNIT_ASSERT_EQUAL(Literal(1), cardinal == 1);
  CPPUNIT_ASSERT_EQUAL(Literal(2), cardinal == 2);

  CPPUNIT_ASSERT_EQUAL(~Literal(0), cardinal != 0);
  CPPUNIT_ASSERT_EQUAL(~Literal(1), cardinal != 1);
  CPPUNIT_ASSERT_EQUAL(~Literal(2), cardinal != 2);
}

void CardinalTest::testEqualsCardinal(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal1(&solver, 0, 3);
  Cardinal cardinal2(&solver, 0, 3);

  Requirement expected;

  // Range requirements
  expected &= cardinal1 >= 0;
  expected &= cardinal1 <  3;
  expected &= cardinal2 >= 0;
  expected &= cardinal2 <  3;

  // cardinal1 == 0 iff cardinal2 == 0
  expected &= cardinal1 != 0 | cardinal2 == 0;
  expected &= cardinal1 == 0 | cardinal2 != 0;

  // ... == 1
  expected &= cardinal1 != 1 | cardinal2 == 1;
  expected &= cardinal1 == 1 | cardinal2 != 1;

  // ... == 2
  expected &= cardinal1 != 2 | cardinal2 == 2;
  expected &= cardinal1 == 2 | cardinal2 != 2;

  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)10, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsCardinal2(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal1(&solver, 0, 3);
  Cardinal cardinal2(&solver, 1, 4);

  Requirement expected;
  // Range requirements
  expected &= cardinal1 >= 1;
  expected &= cardinal1 <  4;
  expected &= cardinal2 >= 0;
  expected &= cardinal2 <  3;

  // cardinal1 == 1 iff cardinal2 == 1
  expected &= cardinal1 != 1 | cardinal2 == 1;
  expected &= cardinal1 == 1 | cardinal2 != 1;

  // ... == 2
  expected &= cardinal1 != 2 | cardinal2 == 2;
  expected &= cardinal1 == 2 | cardinal2 != 2;


  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)8, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testEqualsCardinal3(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal1(&solver, 0, 2);
  Cardinal cardinal2(&solver, 3, 5);

  Requirement expected;
  // All values are impossible, resulting in an empty clause.
  expected &= Clause();

  // We also require cardinal1, cardinal2 to take whatever values they want.
  expected &= cardinal1 >= 0;
  expected &= cardinal2 <  5;

  Requirement result = (cardinal1 == cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testNotEqualCardinal(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal1(&solver, 0, 3);
  Cardinal cardinal2(&solver, 0, 3);

  Requirement expected;
  expected &= cardinal1 != 0 | cardinal2 != 0;
  expected &= cardinal1 != 1 | cardinal2 != 1;
  expected &= cardinal1 != 2 | cardinal2 != 2;

  Requirement result = (cardinal1 != cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testNotEqualCardinal2(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal1(&solver, 0, 3);
  Cardinal cardinal2(&solver, 1, 4);

  Requirement expected;
  expected &= cardinal1 != 1 | cardinal2 != 1;
  expected &= cardinal1 != 2 | cardinal2 != 2;

  Requirement result = (cardinal1 != cardinal2);

  CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());
  CPPUNIT_ASSERT_EQUAL(result, expected);
}

void CardinalTest::testAddition(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 3);

  CPPUNIT_ASSERT_EQUAL(cardinal+1 == 2, cardinal == 1);
  CPPUNIT_ASSERT_EQUAL(cardinal-1 == 0, cardinal == 1);

  CPPUNIT_ASSERT_EQUAL( 1+cardinal == 2, cardinal == 1);
  CPPUNIT_ASSERT_EQUAL(-1+cardinal == 0, cardinal == 1);
}


void CardinalTest::testGreaterThan(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 4);

  Clause expected = cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal > 1, expected);
}

void CardinalTest::testRedundantGreaterThan(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 4);

  Clause expected = cardinal == 0 | cardinal == 1 | cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal >= -10, expected);
}


void CardinalTest::testLessThan(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 4);

  Clause expected = cardinal == 0 | cardinal == 1;

  CPPUNIT_ASSERT_EQUAL(cardinal <= 1, expected);
}

void CardinalTest::testRedundantLessThan(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 4);

  Clause expected = cardinal == 0 | cardinal == 1 | cardinal == 2 | cardinal == 3;

  CPPUNIT_ASSERT_EQUAL(cardinal < 10, expected);
}

void CardinalTest::testDomainError(void) {
  // Object under test.
  MockSolver solver;
  Cardinal cardinal(&solver, 1, 4);

  CPPUNIT_ASSERT_THROW(cardinal == 4, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal == 0, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal != 4, domain_error);
  CPPUNIT_ASSERT_THROW(cardinal != 0, domain_error);

}

void CardinalTest::testNegation(void) {
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 10);
  Cardinal negation = -cardinal;
  Cardinal subtraction = 12-cardinal;

  CPPUNIT_ASSERT_EQUAL(-9, negation.min());
  CPPUNIT_ASSERT_EQUAL(1, negation.max());
  CPPUNIT_ASSERT_EQUAL(cardinal == 3, negation == -3);

  CPPUNIT_ASSERT_EQUAL(3,  subtraction.min());
  CPPUNIT_ASSERT_EQUAL(13, subtraction.max());
  CPPUNIT_ASSERT_EQUAL(cardinal == 3, subtraction == 9);
}

void CardinalTest::testNegNeg(void) {
  MockSolver solver;
  Cardinal cardinal(&solver, 0, 10);
  Cardinal negneg = -(-cardinal);

  CPPUNIT_ASSERT_EQUAL(0,  negneg.min());
  CPPUNIT_ASSERT_EQUAL(10, negneg.max());
  CPPUNIT_ASSERT_EQUAL(cardinal == 3, negneg == 3);
}
