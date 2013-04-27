// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <minisat/core/SolverTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/pattern.h"
#include "../src/testglue.h"

using namespace std;
using Minisat::mkLit;
using Minisat::Lit;

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
