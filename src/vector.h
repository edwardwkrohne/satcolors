// -*- Mode:C++ -*-
//
// An vector of cardinals
//
// One or more cardinals confined to the same range.  Overloads the subscript operator
// to return a corresponding cardinal in a particular location, upon which requirements
// can be established.
//
// The naming is, of course, unfortunate. There exist std::vector, Minisat::vec, and ::Vector, all of
// which are slightly different.  And yet, how many names are there for particular kinds of aggregations?
// What's the point of namespaces?  ::Vector is by far the most mathematically precedented name.

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"
#include "cardinal.h"

class Vector {
public:
  typedef Cardinal::value_type value_type;
  typedef value_type size_type;

  // Creates an object representing a vector.
  // With the default startingVar, registers requirements and allocates a new object using the next available literals.
  // With a non-default startingVar, acts as a reference-like object for a already created object with the given
  //  startingvar, does not register new requirements, and increments allocateNew by the number of literals used.
  Vector(SolverManager& manager, size_type size, value_type min, value_type max, Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  Vector(const Vector& copy);

  // The corresponding requirement of being a vector
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this vector.
  unsigned int getNumLiterals() const;

  // After a solution has been found, return a clause requiring the current/a different solution
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  // Return a cardinal representing an element of the vector
  Cardinal operator[](size_type cardinal) const;

  class CardinalIndexedCardinal;

  // Index into the vector using a Cardinal type.  Very powerful, slightly expensive.
  // Assignments will depend on the ultimate value of the cardinal in the model.
  // Not yet implemented.
  CardinalIndexedCardinal operator[](Cardinal index);

  const size_type size;
  const value_type min;
  const value_type max;
  SolverManager& manager;
  const Minisat::Var startingVar;
};

std::ostream& operator<<(std::ostream& out, const Vector& vector);
#endif // VECTOR_H
