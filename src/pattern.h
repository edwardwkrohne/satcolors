// -*- Mode:C++ -*-
//
// Stores a pattern, which is a common need I have in this code.
//

#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include "requirement.h"
#include "solvermanager.h"
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
