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
#include "cardinal.h"
#include "objectcontainer.h"

// 2d lists are (ultimately) 1d lists.
template<class obj_type>
class List2d {
public:
  typedef Cardinal::value_type value_type;
  typedef std::function<obj_type(value_type, value_type)> builder_type;

  List2d(
      SolverManager* manager,
      Cardinal::value_type height,
      Cardinal::value_type width,
      builder_type builder,
      Minisat::Var& startingVar = SolverManager::allocateNew);

  SubscriptWrapper<obj_type> operator[](value_type row) const;

  Requirement typeRequirement() const;
  unsigned int numLiterals() const;
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  unsigned int height() const;
  unsigned int width() const;
  const ObjectContainer<obj_type>& data() const;

private:
  const Cardinal::value_type mHeight;
  const Cardinal::value_type mWidth;
  ObjectContainer<obj_type> mData;
};

template<class obj_type>
List2d<obj_type>::List2d(
    SolverManager* _manager,
    Cardinal::value_type _height,
    Cardinal::value_type _width,
    builder_type builder,
    Minisat::Var& _startingVar) :
  mData(),
  mHeight(_height),
  mWidth(_width)
{
  if ( _height < 0 || _width < 0 ) {
    std::ostringstream sout;
    sout << "Invalid height or width " << _height << " and " << _width << " for List2d";
    throw std::invalid_argument(sout.str());
  }

  mData.reserve(height()*width());

  for ( int row = 0; row < height(); row++ ) {
    for ( int col = 0; col < width(); col++ ) {
      mData.emplace_back(builder(row, col));
    }
  }
}

// Function to simplify determining the type of a particular list.
template<class builder_type>
auto makeList(SolverManager& manager, 
	      Cardinal::value_type height, 
	      Cardinal::value_type width, 
	      builder_type builder, 
	      Minisat::Var& startingVar)
  -> List2d<decltype(builder(height, width))> {

  typedef decltype(builder(height, width)) obj_type;
  return List2d<obj_type>(&manager, height, width, builder, startingVar);
}

template<class obj_type>
SubscriptWrapper<obj_type> List2d<obj_type>::operator[](value_type row) const {
  // Guard against nonsensical rows.
  if ( row < 0 || row >= height() ) {
    std::ostringstream sout;
    sout << "Invalid row " << row << " for List2d of height " << height();
    throw std::out_of_range(sout.str());
  }

  return SubscriptWrapper<obj_type>([=] (value_type col) {
    // Guard against nonsensical columns.
      if ( col < 0 || col >= this->width() ) {
	std::ostringstream sout;
	sout << "Invalid column " << col << " for List2d of width " << this->width();
	throw std::out_of_range(sout.str());
      }

      // Get the right cell out of the data list.
      return this->data()[row*this->width() + col];
    });
}

template<class obj_type>
unsigned int List2d<obj_type>::height() const  {
  return mHeight;
}

template<class obj_type>
unsigned int List2d<obj_type>::width() const  {
  return mWidth;
}

template<class obj_type>
const ObjectContainer<obj_type>& List2d<obj_type>::data() const  {
  return mData;
}

template<class obj_type>
Requirement List2d<obj_type>::typeRequirement() const {
  return data().typeRequirement();
}

template<class obj_type>
unsigned int List2d<obj_type>::numLiterals() const {
  return data().numLiterals();
}

template<class obj_type>
Clause List2d<obj_type>::diffSolnReq() const {
  return data().diffSolnReq();
}

template<class obj_type>
DualClause List2d<obj_type>::currSolnReq() const {
  return data().currSolnReq();
}

#endif // LIST2D_H
