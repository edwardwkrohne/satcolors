// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <stdexcept>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/array2d.h"

using namespace std;

class Array2dTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(Array2dTest);
  CPPUNIT_TEST(testStorage);
  CPPUNIT_TEST(testStorageBool);
  CPPUNIT_TEST(testCpp11InitializerList);
  CPPUNIT_TEST(testCpp11InitializerListWidthMismatch);
  CPPUNIT_TEST(testConstAccess);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testStorage(void);
  void testStorageBool(void);
  void testCpp11InitializerList(void);
  void testCpp11InitializerListWidthMismatch(void);
  void testConstAccess(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( Array2dTest );

void Array2dTest::testStorage() {
  Array2d<int> array(3,4);
  array[0][0] = 4;
  array[0][1] = 5;
  array[0][2] = 8;
  array[1][0] = 6;
  array[1][1] = 7;
  array[1][2] = 9;

  CPPUNIT_ASSERT_EQUAL(4, array[0][0]);
  CPPUNIT_ASSERT_EQUAL(5, array[0][1]);
  CPPUNIT_ASSERT_EQUAL(8, array[0][2]);
  CPPUNIT_ASSERT_EQUAL(6, array[1][0]);
  CPPUNIT_ASSERT_EQUAL(7, array[1][1]);
  CPPUNIT_ASSERT_EQUAL(9, array[1][2]);
}

void Array2dTest::testStorageBool() {
  // Execute a similar test as above, but this time with bools.
  //  Array2d<bool> converted to bitarrays in the underlying implementation by the stl, so it's
  //  useful to see how differently such objects behave.
  Array2d<bool> array(3,4);
  array[0][0] = true;
  array[0][1] = true;
  array[0][2] = false;
  array[1][0] = false;
  array[1][1] = true;
  array[1][2] = false;

  CPPUNIT_ASSERT_EQUAL(true,  (bool)array[0][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)array[0][1]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)array[0][2]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)array[1][0]);
  CPPUNIT_ASSERT_EQUAL(true,  (bool)array[1][1]);
  CPPUNIT_ASSERT_EQUAL(false, (bool)array[1][2]);
}

void Array2dTest::testCpp11InitializerList(void) {
  Array2d<string> array = {
      {"hello", "world", "this"},
      {"is", "a", "list"}
  };

  CPPUNIT_ASSERT_EQUAL((string)"hello", array[0][0]);
  CPPUNIT_ASSERT_EQUAL((string)"a",     array[1][1]);
  CPPUNIT_ASSERT_EQUAL((string)"this",  array[0][2]);

}

void Array2dTest::testCpp11InitializerListWidthMismatch(void) {
  // EK 2012???
  // This test fails intermittently, and strangely segfaults when
  // other tests run.  I've been able to control whether THIS TEST
  // segfaults by adjusting the length of a string stream operation in
  // a completely unrelated test.  This seems to be a bizarre
  // memory/multithreading issue.
  //
  // I've removed it from the test list to see if similar faults crop up.
  //
  // EK Sep 2014
  //
  // I wrote the above comment so long ago I don't remember writing it
  // (I had to guess on the date just now), so "similar faults" did
  // not crop up.  So I reenabled the test, and it passes.  This test
  // is worth watching, for sure.

  // EK Oct 2014 
  //
  // I think I fixed the problem; I was creating an "initializer list"
  // separately to make the code easier to read, and it probably
  // doesn't copy nicely.  Was definitely seeing corrupted memory
  // problems.  Should be fixed, but I'm leaving these comments in in
  // case it breaks again.

  // Create an initializer list where one row has three elements, but one row has two.
  CPPUNIT_ASSERT_THROW(Array2d<string> array({
	{"hello", "world", "this"},
	{"is", "a"}
      }), std::invalid_argument);
  
  cout << "Exception thrown correctly" << endl;
}

void Array2dTest::testConstAccess(void) {
  const Array2d<string> array = {
      {"hello", "world", "this"},
      {"is", "a", "list"}
  };

  CPPUNIT_ASSERT_EQUAL((string)"hello", array[0][0]);
  CPPUNIT_ASSERT_EQUAL((string)"a",     array[1][1]);
  CPPUNIT_ASSERT_EQUAL((string)"this",  array[0][2]);

}
