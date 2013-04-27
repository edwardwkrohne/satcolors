// -*- Mode:C++ -*-

#include <string>
#include <sstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/manipulators.h"

using namespace std;

class ManipulatorsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ManipulatorsTest);
  CPPUNIT_TEST(testGnuplotWrapper);
  CPPUNIT_TEST_SUITE_END();
protected:
  void testGnuplotWrapper(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION( ManipulatorsTest );


void ManipulatorsTest::testGnuplotWrapper(void) {
  string expected =
      "set term pdf size 8,10.5\n"
      "set output \"out.pdf\"\n"
      "set palette defined ( -1 \"gray\", 0 \"white\", 1 \"black\", 2 \"blue\", 3 \"red\", 4 \"green\")\n"
      "set cbrange [-1:4]\n"
      "unset label\n"
      "unset title\n"
      "unset key\n"
      "unset colorbox\n"
      "set xtics 1 format \"\"\n"
      "set ytics 1 format \"\"\n"
      "set grid front lt 1 lc 8\n"
      "set size ratio -1\n"
      "set yrange [] reverse\n"
      "set multiplot layout 3,5\n"
      "plot '-' matrix using (0.5+$1):(0.5+$2):($3) with image\n"
      "0 0 0\n"
      "e\n"
      "e\n";

  ostringstream sout;
  Gnuplot plot(sout);
  plot << multiplot(3,5);
  plot << "0 0 0\n";

//  CPPUNIT_ASSERT_EQUAL(expected, sout.str());
}
