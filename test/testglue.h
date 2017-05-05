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

#ifndef TESTGLUE_H_
#define TESTGLUE_H_

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/solver.h"

#define ASSERT_UNSAT_ASSUMP(solver, assumptions, obj) assertUnsat((solver), (assumptions), #assumptions, (obj), CPPUNIT_SOURCELINE());
#define ASSERT_UNSAT(solver, obj)                     assertUnsat((solver), (obj), CPPUNIT_SOURCELINE());
#define ASSERT_SAT_ASSUMP(solver, assumptions)        assertSat  ((solver), (assumptions), #assumptions, CPPUNIT_SOURCELINE());
#define ASSERT_SAT(solver)                            assertSat  ((solver), CPPUNIT_SOURCELINE());

template<class T, class U>
void assertUnsat(Solver& solver,
		 const U& assumptions,
		 const std::string& strAssumptions,
		 T& obj,
		 CPPUNIT_NS::SourceLine sourceLine) {
  using namespace std;
  if ( solver.solve(assumptions) ) {
    ostringstream sout;
    sout << "Illegal solution to constraints under assumptions:" << endl << strAssumptions << endl;
    sout << "Solution:" << endl << obj;
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
void assertSat(Solver& solver,
	       const T& assumptions,
	       const std::string& strAssumptions,
	       CPPUNIT_NS::SourceLine sourceLine) {
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
