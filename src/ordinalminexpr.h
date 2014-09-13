// -*- Mode:C++ -*-
//
// Addition expressions for ordinals.
//
// Used to allow syntax of the form min(ord1, ord2) <= ord3 and
// min(ord1, ord2) >= ord3, etc.
//
#ifndef ORDINALMINEXPR_H
#define ORDINALMINEXPR_H

#include <iostream>
#include "requirement.h"
#include "solvermanager.h"
#include "ordinal.h"

class OrdinalMinExpr {
  typedef Ordinal::value_type value_type;
private:
  OrdinalMinExpr(const Ordinal& ord1, const Ordinal& ord2);
  OrdinalMinExpr() = delete;
public:
  OrdinalMinExpr(const OrdinalMinExpr& other) = default;

  friend OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2);

  Clause      operator <  (const value_type bound) const;
  DualClause  operator >  (const value_type bound) const;
  Clause      operator <= (const value_type bound) const;
  DualClause  operator >= (const value_type bound) const;

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

DualClause  operator <  (const Ordinal::value_type bound, const OrdinalMinExpr& expr);
Clause      operator >  (const Ordinal::value_type bound, const OrdinalMinExpr& expr);
DualClause  operator <= (const Ordinal::value_type bound, const OrdinalMinExpr& expr);
Clause      operator >= (const Ordinal::value_type bound, const OrdinalMinExpr& expr);
Requirement operator == (const Ordinal::value_type equality, const OrdinalMinExpr& expr);

Requirement operator <  (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator >  (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator <= (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator >= (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator == (const Ordinal& ord3, const OrdinalMinExpr& expr);

OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2);
OrdinalMinExpr max(const Ordinal& ord1, const Ordinal& ord2);

#endif // ORDINALMINEXPR_H
