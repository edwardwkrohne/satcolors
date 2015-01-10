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
// Addition expressions for ordinals.
//
// Used to allow syntax of the form min(ord1, ord2) <= ord3 and
// min(ord1, ord2) >= ord3, etc.
//
#ifndef ORDINALMINEXPR_H
#define ORDINALMINEXPR_H

#include <iostream>
#include "requirement.h"
#include "solver.h"
#include "ordinal.h"

class OrdinalMinExpr {
private:
  OrdinalMinExpr(const Ordinal& ord1, const Ordinal& ord2);
  OrdinalMinExpr() = delete;
public:
  OrdinalMinExpr(const OrdinalMinExpr& other) = default;

  friend OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2);

  Clause      operator <  (const int bound) const;
  DualClause  operator >  (const int bound) const;
  Clause      operator <= (const int bound) const;
  DualClause  operator >= (const int bound) const;

  Requirement operator <  (const Ordinal& ord3) const;
  Requirement operator >  (const Ordinal& ord3) const;
  Requirement operator <= (const Ordinal& ord3) const;
  Requirement operator >= (const Ordinal& ord3) const;

  Requirement operator == (const Ordinal& ord3) const;

  Requirement operator == (const int equality) const;

private:
  const Ordinal& ord1;
  const Ordinal& ord2;
};

DualClause  operator <  (const int bound, const OrdinalMinExpr& expr);
Clause      operator >  (const int bound, const OrdinalMinExpr& expr);
DualClause  operator <= (const int bound, const OrdinalMinExpr& expr);
Clause      operator >= (const int bound, const OrdinalMinExpr& expr);
Requirement operator == (const int equality, const OrdinalMinExpr& expr);

Requirement operator <  (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator >  (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator <= (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator >= (const Ordinal& ord3, const OrdinalMinExpr& expr);
Requirement operator == (const Ordinal& ord3, const OrdinalMinExpr& expr);

OrdinalMinExpr min(const Ordinal& ord1, const Ordinal& ord2);
OrdinalMinExpr max(const Ordinal& ord1, const Ordinal& ord2);

#endif // ORDINALMINEXPR_H
