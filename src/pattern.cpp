// -*- Mode:C++ -*-
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
