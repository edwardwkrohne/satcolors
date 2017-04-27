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
// TwelveTiles
//
// Set of twelve interlocking tiles that can be used to continuously tile 2^(Z^2)
//
// The notation is from our forthcoming paper on 2^(Z^2)

#ifndef TWELVETILES_H
#define TWELVETILES_H

#include "matrix.h"
#include "matrixivew.h"
#include "cardinal.h"

template<typename Scalar = Cardinal>
class TwelveTiles {
 public:
  TwelveTiles() = delete;
  TwelveTiles(const TwelveTiles& copy) = default;
  TwelveTiles(TwelveTiles&& move) = default;
  TwelveTiles& operator=(const TwelveTiles&& copy) = default;
  TwelveTiles& operator=(Matrix&& move) = default;

  TwelveTiles(int p, int q, int boundarySize, int depth);

  class ViewsIter {
  public:
    MatrixView<Scalar> operator*();
    ViewsIter& operator++();
  }
  
  class Views {
  public:
    ViewsIter begin();
    ViewsIter end();
  } views;
  
 public:
  Matrix<Scalar> Tacac;
  Matrix<Scalar> Tadad;
  Matrix<Scalar> Tbcbc;
  Matrix<Scalar> Tbdbd;

  Matrix<Scalar> Tabcabc;
  Matrix<Scalar> Tbacbac;
  Matrix<Scalar> Tacdacd;
  Matrix<Scalar> Tadcadc;

  Matrix<Scalar> Taqcbpc;
  Matrix<Scalar> Tbpcaqc;
  Matrix<Scalar> Tacqadp;
  Matrix<Scalar> Tadpacq;
  
};

#endif
