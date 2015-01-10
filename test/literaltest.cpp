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
#include "../src/literal.h"

using namespace std;

class LiteralTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LiteralTest);
  CPPUNIT_TEST(testTriviality);
  CPPUNIT_TEST(testNegPos);
  CPPUNIT_TEST(testGetVar);
  CPPUNIT_TEST(testDoubleNegative);
  CPPUNIT_TEST(testOutputLitPositive);
  CPPUNIT_TEST(testOutputLitNegative);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testTriviality(void);
  void testGetVar(void);
  void testNegPos(void);
  void testDoubleNegative(void);
  void testOutputLitPositive(void);
  void testOutputLitNegative(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( LiteralTest );

void LiteralTest::testTriviality(void) {
  CPPUNIT_ASSERT(is_trivial<Literal>::value);
}

void LiteralTest::testNegPos(void) {
  Literal posLit(0, true);
  Literal posLitDefault(0);
  Literal negLit(0, false);

  CPPUNIT_ASSERT_EQUAL(posLit, posLitDefault);

  CPPUNIT_ASSERT(posLit.isPos());
  CPPUNIT_ASSERT(!negLit.isPos());

  CPPUNIT_ASSERT_EQUAL(posLit, ~negLit);
}

void LiteralTest::testGetVar(void) {
  Literal posLit0(0, true);
  Literal negLit0(0, false);
  Literal posLit1(1, true);
  Literal negLit1(1, false);

  CPPUNIT_ASSERT_EQUAL(0u, posLit0.getVar());
  CPPUNIT_ASSERT_EQUAL(0u, negLit0.getVar());
  CPPUNIT_ASSERT_EQUAL(1u, posLit1.getVar());
  CPPUNIT_ASSERT_EQUAL(1u, negLit1.getVar());
}

void LiteralTest::testDoubleNegative(void) {
  Literal expected(2);
  Literal result = ~~Literal(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void LiteralTest::testOutputLitPositive(void) {
  string expected = "1";

  ostringstream resultStream;
  resultStream << Literal(1);
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}

void LiteralTest::testOutputLitNegative(void) {
  string expected = "~2";

  ostringstream resultStream;
  resultStream << ~Literal(2);
  
  CPPUNIT_ASSERT_EQUAL(expected, resultStream.str());  
}
