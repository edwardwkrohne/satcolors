/*
 * testglue.h
 *
 *  Created on: Oct 12, 2012
 *      Author: ekrohne
 */

#ifndef TESTGLUE_H_
#define TESTGLUE_H_

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/solvermanager.h"

#define ASSERT_UNSAT_ASSUMP(manager, assumptions, obj) assertUnsat((manager), (assumptions), (obj), CPPUNIT_SOURCELINE());
#define ASSERT_UNSAT(manager, obj)                     assertUnsat((manager), (obj), CPPUNIT_SOURCELINE());
#define ASSERT_SAT_ASSUMP(manager, assumptions)        assertSat  ((manager), (assumptions), #assumptions, CPPUNIT_SOURCELINE());
#define ASSERT_SAT(manager)                            assertSat  ((manager), CPPUNIT_SOURCELINE());

template<class T, class U>
void assertUnsat(SolverManager& manager, const U& assumptions, T& obj, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( manager.solve(assumptions) ) {
    ostringstream sout;
    sout << "Illegal solution to constraints and assumptions." << endl << obj;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}

template<class T>
void assertUnsat(SolverManager& manager, T& obj, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( manager.solve() ) {
    ostringstream sout;
    sout << "Illegal solution to constraints." << endl << obj;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}


template<class T>
void assertSat(SolverManager& manager, const T& assumptions, const std::string& strAssumptions, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( !manager.solve(assumptions) ) {
    ostringstream sout;
    sout << "Constraints unsolvable under assumptions." << endl << strAssumptions << endl;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}

inline void assertSat(SolverManager& manager, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( !manager.solve() ) {
    CPPUNIT_NS::Asserter::fail( "Constraints unsolvable.", sourceLine );
  }
}

#endif /* TESTGLUE_H_ */
