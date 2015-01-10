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

#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;
using namespace CPPUNIT_NS;

int main (int argc, char** argv) {
  // Create an object to store the test results
  TestResult testResult;
  
  // Register listener for collecting the test-results
  TestResultCollector collectedResults;
  testResult.addListener(&collectedResults);

  // Register listener for per-test progress output
//  BriefTestProgressListener progress;
//  testResult.addListener(&progress);

  // Insert test-suite at test-runner by registry
  TestRunner testRunner;
  testRunner.addTest(TestFactoryRegistry::getRegistry().makeTest());
  testRunner.run(testResult);
  
  // Output results in compiler-format
  CompilerOutputter compilerOutputter(&collectedResults, cerr, "%p:%l: ");
  compilerOutputter.write();

  // Return 0 if tests were successful
  return collectedResults.wasSuccessful() ? 0 : 1;
}
