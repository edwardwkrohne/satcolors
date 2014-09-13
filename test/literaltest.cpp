// -*- Mode:C++ -*-

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
