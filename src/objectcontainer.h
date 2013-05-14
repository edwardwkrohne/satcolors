// -*- Mode:C++ -*-
//
// Defines the usual methods (diffSolnReq, numLiterals, etc.) for stl containers.

#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include <minisat/core/SolverTypes.h>
#include <vector>
#include "solvermanager.h"
#include "requirement.h"

template<class T>
class ObjectContainer : public std::vector<T> {
public:
  ObjectContainer(SolverManager* manager);

  ObjectContainer() = delete;
  ObjectContainer(const ObjectContainer& copy) = default;
  ObjectContainer(ObjectContainer&& move) = default;
  ObjectContainer& operator=(const ObjectContainer& copy) = default;
  ObjectContainer& operator=(ObjectContainer&& move) = default;

  unsigned int numLiterals() const;
  DualClause currSolnReq() const;
  Clause diffSolnReq() const;
  Requirement typeRequirement() const;

private:
  SolverManager* mManager;
};

template<class T>
ObjectContainer<T>::ObjectContainer(SolverManager* manager) :
  mManager(manager)
{

}

template<class T>
unsigned int ObjectContainer<T>::numLiterals() const {
  unsigned int result = 0;
  for ( const T& obj : *this ) {
    result += obj.numLiterals();
  }
  return result;
}

template<class T>
DualClause ObjectContainer<T>::currSolnReq() const {
  DualClause result;
  for ( const T& obj : *this ) {
    result &= obj.currSolnReq();
  }
  return result;
}

template<class T>
Clause ObjectContainer<T>::diffSolnReq() const {
  return ~currSolnReq();
}

template<class T>
Requirement ObjectContainer<T>::typeRequirement() const {
  Requirement result;
  for ( const T& obj : *this ) {
    result &= obj.typeRequirement();
  }
  return result;  
}

#endif // OBJECTCONTAINER_H
