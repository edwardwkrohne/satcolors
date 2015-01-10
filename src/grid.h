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
// A 2d list of objects

#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <functional>
#include "array2d.h" // For subscriptwrapper
#include "requirement.h"
#include "solver.h"
#include "cardinal.h"
#include "objectcontainer.h"

// 2d lists are (ultimately) 1d lists.
template<class obj_type>
class Grid {
public:
  typedef std::function<obj_type(int, int)> builder_type;

  Grid(int height,
       int width,
       builder_type builder);

  SubscriptWrapper<obj_type> operator[](int row) const;

  Requirement typeRequirement() const;
  unsigned int numLiterals() const;
  Clause diffSolnReq() const;
  DualClause currSolnReq() const;

  unsigned int height() const;
  unsigned int width() const;
  const ObjectContainer<obj_type>& data() const;

private:
  int mHeight;
  int mWidth;
  ObjectContainer<obj_type> mData;
};

template<class obj_type>
Grid<obj_type>::Grid(
    int _height,
    int _width,
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
      mData.emplace_back(builder(row, col));
    }
  }
}

// Function to simplify determining the type of a particular list.
template<class builder_type>
auto makeList(int height, 
	      int width, 
	      builder_type builder)
  -> Grid<decltype(builder(height, width))> {

  typedef decltype(builder(height, width)) obj_type;
  return Grid<obj_type>(height, width, builder);
}

template<class obj_type>
SubscriptWrapper<obj_type> Grid<obj_type>::operator[](int row) const {
  // Guard against nonsensical rows.
  if ( row < 0 || row >= height() ) {
    std::ostringstream sout;
    sout << "Invalid row " << row << " for Grid of height " << height();
    throw std::out_of_range(sout.str());
  }

  return SubscriptWrapper<obj_type>([=] (int col) {
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
