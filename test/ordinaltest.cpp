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
#include "../src/minisatsolver.h"
#include "../src/ordinal.h"
#include "../src/ordinaladdexpr.h"

using namespace std;

class OrdinalTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrdinalTest);
  CPPUNIT_TEST(testCopy);
  CPPUNIT_TEST(testIsOrdinal);
  CPPUNIT_TEST(testEmptyConstructorRange);
  CPPUNIT_TEST(testDomainError);
  CPPUNIT_TEST(testEquality);
  CPPUNIT_TEST(testLessEq1);
  CPPUNIT_TEST(testLessEq2);
  CPPUNIT_TEST(testLessEqImpossible);
  CPPUNIT_TEST(testLessEqTrivial);
  CPPUNIT_TEST(testNotEq);
  CPPUNIT_TEST(testNegation);
  CPPUNIT_TEST(testNegation2);
  CPPUNIT_TEST(testSolveBug);
  CPPUNIT_TEST(testModelValue);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testCopy(void);
  void testIsOrdinal(void);
  void testEmptyConstructorRange(void);
  void testDomainError(void);
  void testEquality(void);
  void testLessEq1(void);
  void testLessEq2(void);
  void testLessEqImpossible(void);
  void testLessEqTrivial(void);
  void testNotEq(void);
  void testNegation(void);
  void testNegation2(void);
  void testSolveBug(void);
  void testModelValue(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( OrdinalTest );

void OrdinalTest::testCopy(void) {
  MockSolver solver;
  Ordinal ord(&solver, 0, 4);
  Ordinal cpy(ord);
  Ordinal mv(std::move(ord));

  cpy = ord;
  mv = std::move(ord);
}

void OrdinalTest::testIsOrdinal(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ordinal(&solver, 0, 4);
  CPPUNIT_ASSERT_EQUAL(3u, ordinal.numLiterals());

  Requirement result = ordinal.typeRequirement();
  
  Requirement expected;
  expected &= implication(ordinal <= 0, ordinal <= 1);
  expected &= implication(ordinal <= 1, ordinal <= 2);

  CPPUNIT_ASSERT_EQUAL(expected, result);

}

void OrdinalTest::testEmptyConstructorRange(void) {
  // Object under test.
  MockSolver solver;
  CPPUNIT_ASSERT_THROW(Ordinal(&solver, 2, 2), domain_error);
  CPPUNIT_ASSERT_THROW(Ordinal(&solver, 5, 3), domain_error);
}


void OrdinalTest::testEquality(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ordinal(&solver, 1, 5);

  typedef decltype(ordinal == 1) eqType;

  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal <= 1), ordinal == 1);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 2 & ordinal >= 2, ordinal == 2);
  CPPUNIT_ASSERT_EQUAL(ordinal <= 3 & ordinal >= 3, ordinal == 3);
  CPPUNIT_ASSERT_EQUAL((eqType)(ordinal >= 4), ordinal == 4);

  Ordinal smallOrd(&solver, 1, 2);
  CPPUNIT_ASSERT_EQUAL(eqType(), smallOrd == 1);
}

void OrdinalTest::testDomainError(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ordinal(&solver, 1, 4);

  CPPUNIT_ASSERT_THROW(ordinal == 4, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal == 0, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal != 4, domain_error);
  CPPUNIT_ASSERT_THROW(ordinal != 0, domain_error);

  CPPUNIT_ASSERT_NO_THROW(ordinal == 3);
  CPPUNIT_ASSERT_NO_THROW(ordinal != 3);
  CPPUNIT_ASSERT_NO_THROW(ordinal == 1);
  CPPUNIT_ASSERT_NO_THROW(ordinal != 1);

  CPPUNIT_ASSERT_THROW   (ordinal >= 1, domain_error);
  CPPUNIT_ASSERT_NO_THROW(ordinal >= 2);
  CPPUNIT_ASSERT_NO_THROW(ordinal <= 1);

  CPPUNIT_ASSERT_THROW   (ordinal <= 3, domain_error);
  CPPUNIT_ASSERT_NO_THROW(ordinal <= 2);
  CPPUNIT_ASSERT_NO_THROW(ordinal >= 2);
}

void OrdinalTest::testLessEq1(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord1(&solver, -3, 5);
  Ordinal ord2(&solver, -2, 2);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= implication(ord2 < -1, ord1 < -1);
  expected &= implication(ord2 <  0, ord1 <  0);
  expected &= implication(ord2 <  1, ord1 <  1);
  expected &= ord1 < 2;

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEq2(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord1(&solver, 3, 7);
  Ordinal ord2(&solver, 1, 10);

  Requirement result = ord1 <= ord2;
  
  Requirement expected;
  expected &= ord2 >= 3;
  expected &= implication(ord1 >= 4, ord2 >= 4);
  expected &= implication(ord1 >= 5, ord2 >= 5);
  expected &= implication(ord1 >= 6, ord2 >= 6);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqImpossible(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord1(&solver, 3, 7);
  Ordinal ord2(&solver, 0, 3);

  Requirement result = ord1 <= ord2;
  
  // Note most vexing parse
  Requirement expected((Clause())); // "FALSE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testLessEqTrivial(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord1(&solver, 0, 3);
  Ordinal ord2(&solver, 3, 7);

  Requirement result = ord1 <= ord2;
  
  Requirement expected; // "TRUE"

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void OrdinalTest::testNotEq(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord1(&solver, -1, 4);
  Ordinal ord2(&solver, 1, 7);

  Requirement result = ord1 != ord2;
  
  Requirement expected;
  expected &= (ord1 != 1 | ord2 != 1);
  expected &= (ord1 != 2 | ord2 != 2);
  expected &= (ord1 != 3 | ord2 != 3);

  CPPUNIT_ASSERT_EQUAL(expected, result);
  
}

void OrdinalTest::testNegation(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord(&solver,  0,  5);
  Ordinal negOrd = -ord;

  CPPUNIT_ASSERT_EQUAL(-4, negOrd.min());
  CPPUNIT_ASSERT_EQUAL( 1, negOrd.max());
  CPPUNIT_ASSERT_EQUAL(negOrd.numLiterals(), ord.numLiterals());
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrd >  -1); // Literal 0 true
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrd >  -2); // Literal 1 true
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrd >  -3); // Literal 2 true
  CPPUNIT_ASSERT_EQUAL(ord <  4, negOrd >  -4); // Literal 3 true

  Ordinal negNegOrd = -negOrd;
  CPPUNIT_ASSERT_EQUAL(negNegOrd >= 3, ord >= 3);
}

void OrdinalTest::testNegation2(void) {
  // Object under test.
  MockSolver solver;
  Ordinal ord(&solver,  0,  5);
  Ordinal negOrdPlusOne = -ord + 1;
  
  CPPUNIT_ASSERT_EQUAL(ord <  1, negOrdPlusOne >  -1+1);
  CPPUNIT_ASSERT_EQUAL(ord <  2, negOrdPlusOne >  -2+1);
  CPPUNIT_ASSERT_EQUAL(ord <  3, negOrdPlusOne >  -3+1);
}

void OrdinalTest::testSolveBug(void) {
  MinisatSolver solver;

  Ordinal ord(&solver,  0,  5);
  
  CPPUNIT_ASSERT(solver.solve(ord == 2));
}

void OrdinalTest::testModelValue(void) {
  MinisatSolver solver;

  Ordinal ord(&solver,  0,  5);
  Ordinal negOrd = -ord;

  Literal lit = Literal(solver.newVars(1));
  solver.require(implication(lit, ord == 2));

  CPPUNIT_ASSERT(solver.solve(ord == 0));
  CPPUNIT_ASSERT_EQUAL( 0, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL( 0, negOrd.modelValue());

  CPPUNIT_ASSERT(solver.solve(ord == 1));
  CPPUNIT_ASSERT_EQUAL( 1, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-1, negOrd.modelValue());

  CPPUNIT_ASSERT(solver.solve(ord == 2));
  CPPUNIT_ASSERT_EQUAL( 2, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-2, negOrd.modelValue());

  CPPUNIT_ASSERT(solver.solve(ord == 3));
  CPPUNIT_ASSERT_EQUAL( 3, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-3, negOrd.modelValue());

  CPPUNIT_ASSERT(solver.solve(ord == 4));
  CPPUNIT_ASSERT_EQUAL( 4, ord.modelValue());
  CPPUNIT_ASSERT_EQUAL(-4, negOrd.modelValue());
}
