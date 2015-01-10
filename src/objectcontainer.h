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
//
// Defines the usual methods (diffSolnReq, numLiterals, etc.) for stl containers.

#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include <vector>
#include "solver.h"
#include "requirement.h"

template<typename T>
class ObjectContainer : public std::vector<T> {
public:
  ObjectContainer() = default;
  ObjectContainer(const ObjectContainer& copy) = default;
  ObjectContainer(ObjectContainer&& move) = default;
  ObjectContainer& operator=(const ObjectContainer& copy) = default;
  ObjectContainer& operator=(ObjectContainer&& move) = default;

  ObjectContainer(std::size_t count) : std::vector<T>(count) {};

  unsigned int numLiterals() const;
  DualClause currSolnReq() const;
  Clause diffSolnReq() const;
  Requirement typeRequirement() const;

private:
  Solver* mSolver;
};

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
