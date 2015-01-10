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
// Stores a pattern, which is a common need I have in this code.
//

#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include "requirement.h"
#include "solver.h"
#include "array2d.h"

class Pattern {
public:
  Pattern() = delete;
  Pattern(const Pattern& copy) = default;
  Pattern(Pattern&& move) = default;

  Pattern& operator=(const Pattern& copy) = default;
  Pattern& operator=(Pattern&& move) = default;

  Pattern(const std::string& pattern);
  Pattern(std::istream&& in);
  Pattern(std::istream& in);

  int height;
  int width;
  int centerRow;
  int centerCol;
  bool isBlank;

  Array2d<bool> pattern;
  typedef decltype(pattern[0]) subscript_type;
  typedef decltype(((const Array2d<bool>&)pattern)[0]) const_subscript_type;

  subscript_type operator[] (int index);
  const_subscript_type operator[] (int index) const;
private:
  void computePattern(std::istream& in);
};

#endif // PATTERN_H
