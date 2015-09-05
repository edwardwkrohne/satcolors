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
#include <type_traits>
#include <stdexcept>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/atom.h"

using namespace std;

class AtomTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(AtomTest);
  CPPUNIT_TEST(testTriviality);
  CPPUNIT_TEST(testTruthFalsity);
  CPPUNIT_TEST(testDoubleNegative);
  CPPUNIT_TEST(testNegateTruthFalsity);
  CPPUNIT_TEST(testAssignLiteral);
  CPPUNIT_TEST(testOutputAtomPositive);
  CPPUNIT_TEST(testOutputAtomNegative);
  CPPUNIT_TEST(testOutputAtomTruth);
  CPPUNIT_TEST(testOutputAtomFalsity);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testTriviality(void);
  void testTruthFalsity(void);
  void testDoubleNegative(void);
  void testNegateTruthFalsity(void);
  void testAssignLiteral(void);
  void testOutputAtomPositive(void);
  void testOutputAtomNegative(void);
  void testOutputAtomTruth(void);
  void testOutputAtomFalsity(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( AtomTest );

void AtomTest::testTriviality(void) {
  CPPUNIT_ASSERT(is_trivial<Atom>::value);
}

void AtomTest::testTruthFalsity(void) {
  CPPUNIT_ASSERT( Atom::truth.isTruth());
  CPPUNIT_ASSERT(!Atom::truth.isFalsity());
  CPPUNIT_ASSERT(!Atom::truth.isLiteral());
  CPPUNIT_ASSERT_THROW(Atom::truth.getLiteral(), std::exception);
  
  CPPUNIT_ASSERT(!Atom::falsity.isTruth());
  CPPUNIT_ASSERT( Atom::falsity.isFalsity());
  CPPUNIT_ASSERT(!Atom::falsity.isLiteral());
  CPPUNIT_ASSERT_THROW(Atom::falsity.getLiteral(), std::exception);

  Atom literalAtom(Literal(2));

  CPPUNIT_ASSERT(!literalAtom.isTruth());
  CPPUNIT_ASSERT(!literalAtom.isFalsity());
  CPPUNIT_ASSERT(literalAtom.isLiteral());
  CPPUNIT_ASSERT_EQUAL(Literal(2),literalAtom.getLiteral()); 
}

void AtomTest::testDoubleNegative(void) {
  Atom expected(Literal(2));
  Atom result = ~~Atom(Literal(2));

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void AtomTest::testNegateTruthFalsity(void) {
  CPPUNIT_ASSERT_EQUAL(Atom::truth,   ~Atom::falsity);
  CPPUNIT_ASSERT_EQUAL(Atom::falsity, ~Atom::truth);
}

void AtomTest::testAssignLiteral(void) {
  Atom expected = Atom(Literal(2));

  Atom result1, result2;
  result2 = (result1 = Literal(2));
  CPPUNIT_ASSERT_EQUAL(expected, result1);
  CPPUNIT_ASSERT_EQUAL(expected, result2);
  CPPUNIT_ASSERT(result1.isLiteral());
  CPPUNIT_ASSERT(result2.isLiteral());
}


void AtomTest::testOutputAtomPositive(void) {
  string expected = "1";

  ostringstream resultStream;
  resultStream << Atom(Literal(1));
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void AtomTest::testOutputAtomNegative(void) {
  string expected = "~2";

  ostringstream resultStream;
  resultStream << ~Atom(Literal(2));
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void AtomTest::testOutputAtomTruth(void) {
  string expected = "truth";

  ostringstream resultStream;
  resultStream << Atom::truth;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void AtomTest::testOutputAtomFalsity(void) {
  string expected = "falsity";

  ostringstream resultStream;
  resultStream << Atom::falsity;
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}
