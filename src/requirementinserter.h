// -*- Mode:C++ -*-
//
// A simple output iterator designed to insert requirements into a requirement.

#ifndef REQUIREMENTINSERTER_H
#define REQUIREMENTINSERTER_H

#include "requirement.h"

class RequirementInserter {
 public:
  // Initialize the iterator
  RequirementInserter(Requirement& req);

  // Return an object suitable for storage.  The typical
  // implementation trick for output iterators is to
  // just return "this".
  const RequirementInserter& operator*() const;

  // The assignment operator -- places a vector of literals
  // into the requirement
  const Minisat::vec<Minisat::Lit>& operator=(const Minisat::vec<Minisat::Lit>& clause) const;

  // The assignment operator -- places a literal as a unit clause into
  // the requirement
  Minisat::Lit operator=(Minisat::Lit unit) const;

  // The assignment operator -- places a Clause into the requirement
  const Clause& operator=(const Clause& clause) const;

  // The assignment operator -- places a Requirement into the requirement
  const Requirement& operator=(const Requirement& req) const;

  // Increment operator.  Must be defined for consistency with
  // richer operators, but typically does nothing.
  const RequirementInserter& operator++() const;

  // Increment operator.  Must be defined for consistency with
  // richer operators, but typically does nothing.
  const RequirementInserter& operator++(int) const;

 private:
 Requirement& req;
};

#endif // REQUIREMENTINSERTER_H
