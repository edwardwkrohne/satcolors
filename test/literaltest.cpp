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
  CPPUNIT_TEST(testOutputLitPositive);
  CPPUNIT_TEST(testOutputLitNegative);
  CPPUNIT_TEST(testDoubleNegative);
  CPPUNIT_TEST(testThrow);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testTriviality(void);
  void testOutputLitPositive(void);
  void testOutputLitNegative(void);
  void testDoubleNegative(void);
  void testThrow(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( LiteralTest );

void LiteralTest::testTriviality(void) {
  CPPUNIT_ASSERT(is_trivial<Literal>::value);
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

void LiteralTest::testDoubleNegative(void) {
  Literal expected(2);
  Literal result = ~~Literal(2);

  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void LiteralTest::testThrow(void) {
  Literal lit;
  CPPUNIT_ASSERT_THROW(~Literal(0), std::logic_error);
}
