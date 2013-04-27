// Unit test prototype

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
