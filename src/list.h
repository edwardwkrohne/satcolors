// -*- Mode:C++ -*-
//
// A list of objects

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <minisat/core/SolverTypes.h>
#include "array2d.h"
#include "requirement.h"
#include "solvermanager.h"
#include "cardinal.h"

template<class obj_type>
class List {
public:
  typedef Cardinal::value_type value_type;
  typedef std::function<obj_type(SolverManager&, value_type, Minisat::Var&)> builder_type;
  // Creates an object representing the list
  List(SolverManager& manager, value_type size, builder_type builder, Minisat::Var& startingVar = SolverManager::allocateNew);

  // Copy constructor.  Does not register requirements.
  List(const List& copy);

  Requirement typeRequirement() const;
  unsigned int getNumLiterals() const;
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  // Return a cardinal representing an element of the vector
  obj_type operator[](value_type index) const;

  const value_type size;
  SolverManager& manager;
  const Minisat::Var startingVar;
  const builder_type builder;

private:
  // These are not const to make implementing the constructor easier, but they are effectively const;
  // all the functions of the List object are const, so there's no way these will get changed.
  std::vector<Minisat::Var> startingVars;
  unsigned int numLiterals;
};


// Function to simplify determining the type of a particular list.
template<class builder_type>
auto makeList(SolverManager& manager, Cardinal::value_type size, builder_type builder, Minisat::Var& startingVar = SolverManager::allocateNew)
  -> List<decltype(builder(manager, size, startingVar))> {

  typedef decltype(builder(manager, size, startingVar)) obj_type;
  return List<obj_type>(manager, size, builder, startingVar);
}

template<class obj_type>
List<obj_type>::List(
    SolverManager& _manager,
    value_type _size,
    builder_type _builder,
    Minisat::Var& _startingVar) :
  manager(_manager),
  size(_size),
  builder(_builder),
  startingVar(_startingVar == SolverManager::allocateNew ? _manager.newVars(0) : _startingVar),
  startingVars(_size < 0 ? 0 : _size) // Will throw exception later if _size < 0.
{
  // In this function, note the difference between startingVar, which is initialized once and remains constant,
  //  and _startingVar, which is a reference that may or may not change as objects are created.
  if ( _size < 0 ) {
    std::ostringstream sout;
    sout << "List creation attempted with negative size " << _size;
    throw std::invalid_argument(sout.str());
  }

  // Just run through and create dummies of all the objects, thus allocating literals
  //  and adjusting _startingVar as needed
  for ( int i = 0; i < size; i++ ) {
    startingVars[i] = _startingVar == SolverManager::allocateNew ? manager.newVars(0) : _startingVar;
    obj_type obj = builder(manager, i, _startingVar);
  }

  // Compute the number of literals used (bypass getNumLiterals for objects, to avoid assuming).
  Minisat::Var endVar = _startingVar == SolverManager::allocateNew ? manager.newVars(0) : _startingVar;
  numLiterals = endVar - startingVar;
}

template<class obj_type>
List<obj_type>::List(const List<obj_type>& copy) :
  manager(copy.manager),
  size(copy.size),
  startingVar(copy.startingVar),
  builder(copy.builder),
  startingVars(copy.startingVars),
  numLiterals(copy.numLiterals)
{

}

template<class obj_type>
obj_type List<obj_type>::operator[](value_type index) const {
  if ( index < 0 || index >= size ) {
    std::ostringstream sout;
    sout << "Invalid List index.  List has size " << size << ", but index " << index << " requested.";
    throw std::out_of_range(sout.str());
  }
  Minisat::Var var = startingVars[index];
  return builder(manager, index, var);
}

template<class obj_type>
Requirement List<obj_type>::typeRequirement() const {
  Requirement req;
  for ( int i = 0; i < size; i++ ) {
    req &= (*this)[i].typeRequirement();
  }
  return req;
}

template<class obj_type>
unsigned int List<obj_type>::getNumLiterals() const {
  return numLiterals;
}

template<class obj_type>
Clause List<obj_type>::diffSolnReq() const {
  Clause clause;
  for ( int i = 0; i < size; i++ ) {
    clause |= (*this)[i].diffSolnReq();
  }
  return clause;
}

template<class obj_type>
DualClause List<obj_type>::currSolnReq() const {
  DualClause clause;
  for ( int i = 0; i < size; i++ ) {
    clause &= (*this)[i].currSolnReq();
  }
  return clause;
}


#endif // LIST_H
