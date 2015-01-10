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
#include "testglue.h"
#include "../src/pattern.h"

using namespace std;

class PatternTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PatternTest);
  CPPUNIT_TEST(testConstruction);
  CPPUNIT_TEST(testConstruction2);
  CPPUNIT_TEST(testConstructionStream);
  CPPUNIT_TEST(testBlank);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testConstruction(void);
  void testConstruction2(void);
  void testConstructionStream(void);
  void testBlank(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( PatternTest );


void PatternTest::testConstruction(void) {
  Pattern pattern(
      "  .  x  .  .   ; "
      " (x) x  .  .   ; "
      "  .  x  .  .   ; ");

  // The parentheses in the pattern string indicate where the "center" (row, col) is.  The semicolons serve as line breaks.
  CPPUNIT_ASSERT_EQUAL(1, pattern.centerRow);
  CPPUNIT_ASSERT_EQUAL(0, pattern.centerCol);
  CPPUNIT_ASSERT_EQUAL(3, pattern.height);
  CPPUNIT_ASSERT_EQUAL(4, pattern.width);
  CPPUNIT_ASSERT_EQUAL(false, pattern.isBlank);

  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[0][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)pattern[0][1]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)pattern[1][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)pattern[1][1]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[2][3]);
}

void PatternTest::testConstruction2(void) {
  Pattern pattern(
      " .  x        ; "
      " .  x x .    ; "
      " . (.)       ; "
      " .           ; "
      " .           ; ");

  // The parentheses in the pattern string indicate where the "center" (row, col) is.  The semicolons serve as line breaks.
  CPPUNIT_ASSERT_EQUAL(2,  pattern.centerRow);
  CPPUNIT_ASSERT_EQUAL(1,  pattern.centerCol);
  CPPUNIT_ASSERT_EQUAL(5,  pattern.height);
  CPPUNIT_ASSERT_EQUAL(4,  pattern.width);
  CPPUNIT_ASSERT_EQUAL(false, pattern.isBlank);

  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[0][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)pattern[0][1]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[1][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)pattern[1][1]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[2][0]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[2][1]);

  CPPUNIT_ASSERT_EQUAL(false, (bool)pattern[3][2]);
}

void PatternTest::testConstructionStream(void) {
  string strPat =
      " .  x  .  .    ; "
      " . (x) x  .    ; ";

  Pattern pattern((istringstream)strPat);

  // The parentheses in the pattern string indicate where the "center" (row, col) is.  The semicolons serve as line breaks.
  CPPUNIT_ASSERT_EQUAL(1,  pattern.centerRow);
  CPPUNIT_ASSERT_EQUAL(1,  pattern.centerCol);
  CPPUNIT_ASSERT_EQUAL(2,  pattern.height);
  CPPUNIT_ASSERT_EQUAL(4,  pattern.width);
  CPPUNIT_ASSERT_EQUAL(false, pattern.isBlank);
}

void PatternTest::testBlank(void) {
  Pattern pattern(
      " .  .  .  .    ; "
      " .  .  .  .    ; ");

  CPPUNIT_ASSERT_EQUAL(true, pattern.isBlank);
}
