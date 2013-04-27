// -*- Mode:C++ -*-
//
// A simple output iterator designed for adding vectors of literals to a solver
// This class is a simple wrapper class.  It is designed to facilitate unit testing
// of classes that add clauses to a solver.

#ifndef SOLVERITER_H
#define SOLVERITER_H

#include <iterator>
#include <minisat/core/Solver.h>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"

class SolverIter : public std::iterator<std::output_iterator_tag, void, void, void, void> {
 public:
  // Initialize the iterator
  SolverIter(Minisat::Solver& _solver);

  // Return an object suitable for storage.  The typical
  // implementation trick for output iterators is to
  // just return "this".
  const SolverIter& operator*() const;

  // The assignment operator -- places a vector of literals
  // into the solver
  const Minisat::vec<Minisat::Lit>& operator=(const Minisat::vec<Minisat::Lit>& clause) const;

  // The assignment operator -- places a literal as a unit clause into
  // the solver
  Minisat::Lit operator=(Minisat::Lit unit) const;

  // The assignment operator -- places a Clause into the solver
  const Clause& operator=(const Clause& clause) const;

  // The assignment operator -- places a Requirement into the solver
  const Requirement& operator=(const Requirement& req) const;

  // Increment operator.  Must be defined for consistency with
  // richer operators, but typically does nothing.
  const SolverIter& operator++() const;

  // Increment operator.  Must be defined for consistency with
  // richer operators, but typically does nothing.
  const SolverIter& operator++(int) const;

 private:
  Minisat::Solver& solver;
};

#endif // SOLVERITER_H
