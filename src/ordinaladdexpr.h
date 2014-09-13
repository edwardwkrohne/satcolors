// -*- Mode:C++ -*-
//
// Addition expressions for ordinals.
//
// Used to allow syntax of the form ord1 + ord2 <= ord3;
//
#ifndef ORDINALADDEXPR_H
#define ORDINALADDEXPR_H

#include <iostream>
#include "requirement.h"
#include "solvermanager.h"
#include "ordinal.h"

// Direct addition of two ordinals
class OrdinalAddExpr;

// Addition
class OrdinalAddExpr {
  typedef Ordinal::value_type value_type;
private:
  OrdinalAddExpr(const Ordinal& ord1, const Ordinal& ord2);
  OrdinalAddExpr() = delete;
public:
  OrdinalAddExpr(const OrdinalAddExpr& other) = default;

  friend OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2);

  Requirement operator <  (const value_type bound) const;
  Requirement operator >  (const value_type bound) const;
  Requirement operator <= (const value_type bound) const;
  Requirement operator >= (const value_type bound) const;

  Requirement operator <  (const Ordinal& ord3) const;
  Requirement operator >  (const Ordinal& ord3) const;
  Requirement operator <= (const Ordinal& ord3) const;
  Requirement operator >= (const Ordinal& ord3) const;

  Requirement operator == (const Ordinal& ord3) const;

  Requirement operator == (const value_type equality) const;

private:
  const Ordinal& ord1;
  const Ordinal& ord2;
};


Requirement operator <  (const Ordinal::value_type bound, const OrdinalAddExpr& expr);
Requirement operator >  (const Ordinal::value_type bound, const OrdinalAddExpr& expr);
Requirement operator <= (const Ordinal::value_type bound, const OrdinalAddExpr& expr);
Requirement operator >= (const Ordinal::value_type bound, const OrdinalAddExpr& expr);

Requirement operator == (const Ordinal::value_type equality, const OrdinalAddExpr& expr);

Requirement operator <  (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator >  (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator <= (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator >= (const Ordinal& ord3, const OrdinalAddExpr& expr);

Requirement operator == (const Ordinal& equality, const OrdinalAddExpr& expr);

OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2);
OrdinalAddExpr operator-(const Ordinal& ord1, const Ordinal& ord2);

#endif // ORDINALADDEXPR_H
