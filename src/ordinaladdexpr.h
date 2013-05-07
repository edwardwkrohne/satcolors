// -*- Mode:C++ -*-
//
// Addition expressions for ordinals.
//
// Used to allow syntax of the form ord1 + ord2 <= ord3;
//
#ifndef ORDINALADDEXPR_H
#define ORDINALADDEXPR_H

#include <iostream>
#include <minisat/core/SolverTypes.h>
#include "requirement.h"
#include "solvermanager.h"
#include "ordinal.h"

// Direct addition of two ordinals
class OrdinalAddExpr;

// Addition
class OrdinalAddExpr {
private:
  OrdinalAddExpr(const Ordinal& ord1, const Ordinal& ord2);
  OrdinalAddExpr() = delete;
public:
  OrdinalAddExpr(const OrdinalAddExpr& other) = default;

  friend OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2);

  Requirement operator <= (const Ordinal& ord3);

private:
  const Ordinal& ord1;
  const Ordinal& ord2;
};

OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2);

#endif // ORDINALADDEXPR_H
