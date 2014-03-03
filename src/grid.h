// -*- Mode:C++ -*-
//
// A 2d list of objects

#ifndef GRID_H
#define GRID_H

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
class Grid {
public:
  typedef Cardinal::value_type value_type;
  typedef std::function<obj_type(value_type, value_type)> builder_type;

  Grid(
      Cardinal::value_type height,
      Cardinal::value_type width,
      builder_type builder);

  SubscriptWrapper<obj_type> operator[](value_type row) const;

  Requirement typeRequirement() const;
  unsigned int numLiterals() const;
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  unsigned int height() const;
  unsigned int width() const;
  const ObjectContainer<obj_type>& data() const;

private:
  Cardinal::value_type mHeight;
  Cardinal::value_type mWidth;
  ObjectContainer<obj_type> mData;
};

template<class obj_type>
Grid<obj_type>::Grid(
    Cardinal::value_type _height,
    Cardinal::value_type _width,
    builder_type builder) :
  mData(),
  mHeight(_height),
  mWidth(_width)
{
  if ( _height < 0 || _width < 0 ) {
    std::ostringstream sout;
    sout << "Invalid height or width " << _height << " and " << _width << " for Grid";
    throw std::invalid_argument(sout.str());
  }

  mData.reserve(height()*width());

  for ( int row = 0; row < height(); row++ ) {
    for ( int col = 0; col < width(); col++ ) {
      obj_type pat = builder(row, col);
      obj_type pat2 = obj_type(pat);
      mData.emplace_back(pat2);
    }
  }
}

// Function to simplify determining the type of a particular list.
template<class builder_type>
auto makeList(Cardinal::value_type height, 
	      Cardinal::value_type width, 
	      builder_type builder)
  -> Grid<decltype(builder(height, width))> {

  typedef decltype(builder(height, width)) obj_type;
  return Grid<obj_type>(height, width, builder);
}

template<class obj_type>
SubscriptWrapper<obj_type> Grid<obj_type>::operator[](value_type row) const {
  // Guard against nonsensical rows.
  if ( row < 0 || row >= height() ) {
    std::ostringstream sout;
    sout << "Invalid row " << row << " for Grid of height " << height();
    throw std::out_of_range(sout.str());
  }

  return SubscriptWrapper<obj_type>([=] (value_type col) {
    // Guard against nonsensical columns.
      if ( col < 0 || col >= this->width() ) {
	std::ostringstream sout;
	sout << "Invalid column " << col << " for Grid of width " << this->width();
	throw std::out_of_range(sout.str());
      }

      // Get the right cell out of the data list.
      return this->data()[row*this->width() + col];
    });
}

template<class obj_type>
unsigned int Grid<obj_type>::height() const  {
  return mHeight;
}

template<class obj_type>
unsigned int Grid<obj_type>::width() const  {
  return mWidth;
}

template<class obj_type>
const ObjectContainer<obj_type>& Grid<obj_type>::data() const  {
  return mData;
}

template<class obj_type>
Requirement Grid<obj_type>::typeRequirement() const {
  return data().typeRequirement();
}

template<class obj_type>
unsigned int Grid<obj_type>::numLiterals() const {
  return data().numLiterals();
}

template<class obj_type>
Clause Grid<obj_type>::diffSolnReq() const {
  return data().diffSolnReq();
}

template<class obj_type>
DualClause Grid<obj_type>::currSolnReq() const {
  return data().currSolnReq();
}

#endif // GRID_H
