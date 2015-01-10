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
// Used to allow syntax of the form ord1 + ord2 <= ord3;
//
#ifndef ORDINALADDEXPR_H
#define ORDINALADDEXPR_H

#include <iostream>
#include "requirement.h"
#include "solver.h"
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

  Requirement operator <  (const int bound) const;
  Requirement operator >  (const int bound) const;
  Requirement operator <= (const int bound) const;
  Requirement operator >= (const int bound) const;

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


Requirement operator <  (const int bound, const OrdinalAddExpr& expr);
Requirement operator >  (const int bound, const OrdinalAddExpr& expr);
Requirement operator <= (const int bound, const OrdinalAddExpr& expr);
Requirement operator >= (const int bound, const OrdinalAddExpr& expr);

Requirement operator == (const int equality, const OrdinalAddExpr& expr);

Requirement operator <  (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator >  (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator <= (const Ordinal& ord3, const OrdinalAddExpr& expr);
Requirement operator >= (const Ordinal& ord3, const OrdinalAddExpr& expr);

Requirement operator == (const Ordinal& equality, const OrdinalAddExpr& expr);

OrdinalAddExpr operator+(const Ordinal& ord1, const Ordinal& ord2);
OrdinalAddExpr operator-(const Ordinal& ord1, const Ordinal& ord2);

#endif // ORDINALADDEXPR_H
