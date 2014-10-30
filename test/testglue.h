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
#include "../src/solver.h"

#define ASSERT_UNSAT_ASSUMP(solver, assumptions, obj) assertUnsat((solver), (assumptions), (obj), CPPUNIT_SOURCELINE());
#define ASSERT_UNSAT(solver, obj)                     assertUnsat((solver), (obj), CPPUNIT_SOURCELINE());
#define ASSERT_SAT_ASSUMP(solver, assumptions)        assertSat  ((solver), (assumptions), #assumptions, CPPUNIT_SOURCELINE());
#define ASSERT_SAT(solver)                            assertSat  ((solver), CPPUNIT_SOURCELINE());

template<class T, class U>
void assertUnsat(Solver& solver, const U& assumptions, T& obj, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( solver.solve(assumptions) ) {
    ostringstream sout;
    sout << "Illegal solution to constraints and assumptions." << endl << obj;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}

template<class T>
void assertUnsat(Solver& solver, T& obj, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( solver.solve() ) {
    ostringstream sout;
    sout << "Illegal solution to constraints." << endl << obj;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}


template<class T>
void assertSat(Solver& solver, const T& assumptions, const std::string& strAssumptions, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( !solver.solve(assumptions) ) {
    ostringstream sout;
    sout << "Constraints unsolvable under assumptions." << endl << strAssumptions << endl;
    CPPUNIT_NS::Asserter::fail( sout.str(), sourceLine );
  }
}

inline void assertSat(Solver& solver, CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( !solver.solve() ) {
    CPPUNIT_NS::Asserter::fail( "Constraints unsolvable.", sourceLine );
  }
}

#endif /* TESTGLUE_H_ */
