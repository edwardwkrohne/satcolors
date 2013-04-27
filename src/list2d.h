// -*- Mode:C++ -*-
//
// A 2d list of objects

#ifndef LIST2D_H
#define LIST2D_H

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <minisat/core/SolverTypes.h>
#include "array2d.h"
#include "requirement.h"
#include "solvermanager.h"
#include "index.h"
#include "list.h"

// 2d lists are (ultimately) 1d lists.
template<class obj_type>
class List2d {
public:
  typedef Index::value_type value_type;
  typedef std::function<obj_type(SolverManager&, value_type, value_type, Minisat::Var&)> builder_type;

  List2d(
      SolverManager& manager,
      Index::value_type height,
      Index::value_type width,
      builder_type builder,
      Minisat::Var& startingVar = SolverManager::allocateNew);

  SubscriptWrapper<obj_type> operator[](value_type row) const;

  Requirement typeRequirement() const;
  unsigned int getNumLiterals() const;
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  const Index::value_type height;
  const Index::value_type width;
  List<obj_type> data;

private:
  static typename List<obj_type>::builder_type getUnderlyingListBuilder(value_type width, builder_type builder);
};

template<class obj_type>
List2d<obj_type>::List2d(
    SolverManager& _manager,
    Index::value_type _height,
    Index::value_type _width,
    builder_type _builder,
    Minisat::Var& _startingVar) :
  data(
      _manager,
      _height*_width < 0 ? 0 : _height*_width,
      getUnderlyingListBuilder(_width, _builder),
      _startingVar),
  height(_height),
  width(_width)
{
  if ( _height < 0 || _width < 0 ) {
    std::ostringstream sout;
    sout << "Invalid height or width " << _height << " and " << _width << " for List2d";
    throw std::invalid_argument(sout.str());
  }
}

// Function to simplify determining the type of a particular list.
template<class builder_type>
auto makeList(SolverManager& manager, Index::value_type height, Index::value_type width, builder_type builder, Minisat::Var& startingVar)
  -> List2d<decltype(builder(manager, height, width, startingVar))> {

  typedef decltype(builder(manager, height, width, startingVar)) obj_type;
  return List2d<obj_type>(manager, height, width, builder, startingVar);
}

template<class obj_type>
typename List<obj_type>::builder_type List2d<obj_type>::getUnderlyingListBuilder(value_type width, builder_type builder) {
  return [=] (SolverManager& manager, value_type index, Minisat::Var& startingVar) {
    return builder(manager, index/width, index%width, startingVar);
  };
}

template<class obj_type>
SubscriptWrapper<obj_type> List2d<obj_type>::operator[](value_type row) const {
  // Guard against nonsensical rows.
  if ( row < 0 || row >= height ) {
    std::ostringstream sout;
    sout << "Invalid row " << row << " for List2d of height " << height;
    throw std::out_of_range(sout.str());
  }

  return SubscriptWrapper<obj_type>([=] (value_type col) {
    // Guard against nonsensical columns.
    if ( col < 0 || col >= this->width ) {
      std::ostringstream sout;
      sout << "Invalid column " << col << " for List2d of width " << this->width;
      throw std::out_of_range(sout.str());
    }

    // Get the right cell out of the data list.
    return this->data[row*this->width + col];
  });
}

template<class obj_type>
Requirement List2d<obj_type>::typeRequirement() const {
  return data.typeRequirement();
}

template<class obj_type>
unsigned int List2d<obj_type>::getNumLiterals() const {
  return data.getNumLiterals();
}

template<class obj_type>
Clause List2d<obj_type>::diffSolnReq() const {
  return data.diffSolnReq();
}

template<class obj_type>
DualClause List2d<obj_type>::currSolnReq() const {
  return data.currSolnReq();
}

#endif // LIST2D_H
