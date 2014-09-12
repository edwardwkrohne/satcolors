// -*- Mode:C++ -*-
//
// A class making manipulation of literals easier while building a
// problem.  This also in theory makes it easier to use a different
// engine, other than minisat, as the solving engine, by reducing the
// number of places that directly reference engine details.
//
// Optimized for readability and ease of use; speed is not a concern
// since everything gets converted to Minisat vecs before the heavy
// lifting happens.

#ifndef LITERAL_H
#define LITERAL_H

#include <list>
#include <iostream>

class Literal {
public:
  // Assignment and constructors (all default, all trivial)
  Literal() = default;
  Literal(const Literal& other) = default;
  Literal(Literal&& other) = default;
  Literal& operator=(const Literal&rhs ) = default;
  Literal& operator=(Literal&& rhs) = default;

  // Construction from integer
  Literal(unsigned int var);

  // Only one basic logic operation that doesn't use more complicated objects like clauses or dualclauses.
  Literal operator~() const;

  // Equality (primarily to support std::list objects)
  bool operator==(Literal rhs) const;
  bool operator!=(Literal rhs) const;
  bool operator<(Literal rhs) const;

  // Output a "nice" representation (for debugging, primarily)
  friend std::ostream& operator<<(std::ostream& out, Literal rhs);

private:
  signed long m_lit;
};

#endif // LITERAL_H
