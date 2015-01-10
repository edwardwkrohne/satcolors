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
// Implementation of the Pattern class

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "pattern.h"

using namespace std;

namespace {
// Increase readability.
enum {
  BLANK,
  FILLED,
  BLANK_CENTER,
  FILLED_CENTER,
  NEWLINE
};
}


void Pattern::computePattern(istream& strin) {
  // Will need to pass through the strings more than once, so build a list of tokens to do it with.
  vector<int> tokens(0);
  isBlank = true;

  for ( string token; strin >> token; ) {
    if        ( token == ";" ) {
      tokens.push_back(NEWLINE);
    } else if ( token == "x" ) {
      tokens.push_back(FILLED);
    } else if ( token == "." ) {
      tokens.push_back(BLANK);
    } else if ( token == "(x)" ) {
      tokens.push_back(FILLED_CENTER);
    } else if ( token == "(.)" ) {
      tokens.push_back(BLANK_CENTER);
    } else {
      throw runtime_error("Unrecognized token for Pattern constructor: " + token);
    }
  }

  // Compute dimensions and look for center
  int row = 0;
  int col = 0;
  centerRow = -1;
  centerCol = -1;
  height = 0;
  width = 0;
  for ( auto iter = tokens.begin(); iter != tokens.end(); ++iter ) {
    // Look for end of row
    if ( *iter == NEWLINE) {
      col = 0;
      height = max(++row, height);
      continue;
    }

    // Look for center
    if ( *iter == FILLED_CENTER || *iter == BLANK_CENTER ) {
      if ( centerRow != -1 ) {
        throw runtime_error("Multiple centers found by Pattern constructor.");
      }
      centerRow = row;
      centerCol = col;
    }

    // Increment column
    width  = max(++col, width);
  }

  // Create space in the pattern (should probably make a resize function).
  {
    decltype(pattern) tempPat(height,width);
    pattern.swap(tempPat);
  }

  // Get the actual pattern
  row = col = 0;
  for ( auto iter = tokens.begin(); iter != tokens.end(); ++iter ) {
    // Look for end of row
    if ( *iter == NEWLINE) {
      // If we see a newline, fill zeros out to the end.
      while ( col < width ) {
        pattern[row][col++] = false;
      }
      row++;
      col = 0;
      continue;
    }

    // Fill in any spaces
    bool cellFilled = *iter == FILLED || *iter == FILLED_CENTER;
    pattern[row][col] = cellFilled;
    isBlank &= !cellFilled;
    col++;
  }
}

Pattern::Pattern(const string& str) :
  pattern(0,0)
{
  istringstream strin(str);
  computePattern(strin);
}

Pattern::Pattern(istream&& strin) :
  pattern(0,0)
{
  computePattern(strin);
}

Pattern::Pattern(istream& strin) :
  pattern(0,0)
{
  computePattern(strin);
}

Pattern::subscript_type Pattern::operator[] (int index) {
  return pattern[index];
}

Pattern::const_subscript_type Pattern::operator[] (int index) const {
  return pattern[index];
}
