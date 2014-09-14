// -*- Mode:C++ -*-
//
// Represents an unknown number between "min" and "max" (inclusive of min,
// exclusive of max). Thus, if min is 0, and max is 3, then the cardinal can
// take the values 0,1,2.
//
// When the Cardinal is created, it allocates enough literals
//  to indicate (however it likes) the value the cardinal takes.


#ifndef CARDINAL_H
#define CARDINAL_H

#include <iostream>
#include "requirement.h"
#include "solvermanager.h"

class Cardinal {
public:
  typedef signed int value_type;

  // Builds a cardinal object with the corresponding parameters.  With
  // the default startingVar, registers requirements and allocates a
  // new object using the next available literals.
  //
  // With a non-default startingVar, acts as a reference-like object
  // for a already created object with the given startingvar, does not
  // register new requirements, and increments allocateNew by the
  // number of literals used.
  Cardinal(SolverManager* manager, 
	   value_type min, 
	   value_type max, 
	   unsigned int& startingVar = SolverManager::allocateNew);

  Cardinal() = delete;
  Cardinal(const Cardinal& copy) = default;
  Cardinal(Cardinal&& move) = default;
  Cardinal& operator=(const Cardinal& copy) = default;
  Cardinal& operator=(Cardinal&& move) = default;

  // After a solution has been found, a requirement for the current/a different solution
  Literal diffSolnReq() const;
  Literal currSolnReq() const;

  // Addition of a cardinal by a constant.  Surprisingly easy to implement, and useful.
  // If idx is a Cardinal, then idx+1 returns a cardinal that is equal to n+1 iff idx is equal to n.
  // Uses no additional literals or requirements.
  Cardinal operator+(const value_type rhs) const;
  Cardinal operator-(const value_type rhs) const;

  // Additive inversion
  Cardinal operator-() const;
  
  // Simple literals indicating equality with a specific cardinal rhs.  If rhs is out of bounds,
  // behavior is undefined.
  Literal operator==(value_type rhs) const;
  Literal operator!=(value_type rhs) const;

  // Ordering requirements
  Clause operator>(value_type rhs) const;
  Clause operator>=(value_type rhs) const;
  Clause operator<(value_type rhs) const;
  Clause operator<=(value_type rhs) const;

  // Requirements that two Cardinals be equal, whatever values they
  // take.  Requires that both Cardinals have the same manager.  Does
  // not require the range for each cardinal to be the same, or even
  // overlap.
  Requirement operator==(const Cardinal& rhs) const;
  Requirement operator!=(const Cardinal& rhs) const;

  // Requirements that two Cardinals take a particular order
  Requirement operator>(const Cardinal& rhs) const;
  Requirement operator>=(const Cardinal& rhs) const;
  Requirement operator<(const Cardinal& rhs) const;
  Requirement operator<=(const Cardinal& rhs) const;

  // The corresponding requirement of being a cardinal
  Requirement typeRequirement() const;

  // The number of (contiguous) literals required to represent this cardinal.
  // Equal to max-min.
  // TODO make this a constant in all solver objects.
  unsigned int numLiterals() const;

  // Access basic information
  value_type min() const;
  value_type max() const;
  unsigned int startingVar() const;

  // The value assigned in the model, after solving, if a solution is available.
  value_type modelValue() const;
  operator value_type() const;

private:
  bool inverted;
  SolverManager* mManager;
  value_type mMin;
  value_type mMax;
  unsigned int mStartingVar;

  void checkDomain(value_type arg) const;
};

// Arithmetic (by constants)
Cardinal operator+(const Cardinal::value_type rhs, const Cardinal& lhs);
Cardinal operator-(const Cardinal::value_type rhs, const Cardinal& lhs);

// Ordering requirements
Clause operator>(Cardinal::value_type lhs, const Cardinal& rhs);
Clause operator>=(Cardinal::value_type lhs, const Cardinal& rhs);
Clause operator<(Cardinal::value_type lhs, const Cardinal& rhs);
Clause operator<=(Cardinal::value_type lhs, const Cardinal& rhs);

// Comparison operators
Literal operator==(Cardinal::value_type lhs, const Cardinal& rhs);
Literal operator!=(Cardinal::value_type lhs, const Cardinal& rhs);

#endif // CARDINAL_H
