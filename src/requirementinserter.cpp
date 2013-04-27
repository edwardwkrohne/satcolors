// -*- Mode:C++ -*-
//
// A simple output iterator designed for adding vectors of literals to a solver
// This class is a simple wrapper class.  It is designed to facilitate unit testing
// of classes that add clauses to a solver.

#include <algorithm>
#include <iterator>
#include "requirementinserter.h"

using Minisat::Lit;
using Minisat::vec;

// Constructor
RequirementInserter::RequirementInserter(Requirement& _req) :
  req(_req)
{
  
}

// Return assignable object
const RequirementInserter& RequirementInserter::operator*() const {
  return *this;
}

// Assign
const vec<Lit>& RequirementInserter::operator=(const vec<Lit>& vecClause) const {
  Clause listClause;
  for (int i=0; i < vecClause.size(); i++ ) {
    listClause |= vecClause[i];
  }

  req &= move(listClause);

  return vecClause;
}

// Assign
Lit RequirementInserter::operator=(const Lit unit) const {
  req &= unit;

  return unit;
}

// Assign
const Clause& RequirementInserter::operator=(const Clause& clause) const {
  req &= clause;
  return clause;
}

const Requirement& RequirementInserter::operator=(const Requirement& argumentReq) const {
  std::copy(argumentReq.begin(), argumentReq.end(), inserter(req, req.begin()));
  return req;
}


// Increment (no-op, as usual for output iterators)
const RequirementInserter& RequirementInserter::operator++() const {
  return *this;
}

// Increment (no-op, as usual for output iterators)
const RequirementInserter& RequirementInserter::operator++(int) const {
  return *this;
}
