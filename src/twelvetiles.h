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
//    p and q are as in the paper, the coprime lengths of certain tiles and certain shared regions.
//    boundarySize is n from the paper.  This is the thickness of shared regions.
//    depth is the number of distinct values each square in the tiling can take.

#ifndef TWELVETILES_H
#define TWELVETILES_H

#include "matrix.h"
#include "matrixview.h"
#include "cardinal.h"

template<typename Scalar = Cardinal>
class TwelveTiles {
 public:
  TwelveTiles() = delete;
  TwelveTiles(const TwelveTiles& copy) = default;
  TwelveTiles(TwelveTiles&& move) = default;
  TwelveTiles& operator=(const TwelveTiles& copy) = default;
  TwelveTiles& operator=(TwelveTiles&& move) = default;

  TwelveTiles(MinisatSolver* solver, int p, int q, int boundarySize, int depth);

  class ViewsIter {
  public:
    MatrixView<Scalar> operator*();
    ViewsIter& operator++();
  };
  
  class Views {
  public:
    ViewsIter begin();
    ViewsIter end();
  } views;
  
 public:
  // Toroidal tiles
  Matrix<Scalar> Tacac;
  Matrix<Scalar> Tadad;
  Matrix<Scalar> Tbcbc;
  Matrix<Scalar> Tbdbd;

  // Commutation tiles
  Matrix<Scalar> Tcabcab;
  Matrix<Scalar> Tcbacba;
  Matrix<Scalar> Tcdacda;
  Matrix<Scalar> Tdcadca;

  // "Long" tiles
  Matrix<Scalar> Tcqadpa;
  Matrix<Scalar> Tdpacqa;
  Matrix<Scalar> Tcbpcaq;
  Matrix<Scalar> Tcaqcbp;
  
};

template<typename Scalar>
TwelveTiles<Scalar>::TwelveTiles(MinisatSolver* solver, int p, int q, int n, int depth) :
  Tacac(solver, p+n, p+n, 0, depth),
  Tadad(solver, p+n, q+n, 0, depth),
  Tbcbc(solver, q+n, p+n, 0, depth),
  Tbdbd(solver, q+n, q+n, 0, depth),
  Tcabcab(solver, p+q+n, p+n, 0, depth),
  Tcbacba(solver, p+q+n, p+n, 0, depth),
  Tcdacda(solver, p+n, p+q+n, 0, depth),
  Tdcadca(solver, p+n, p+q+n, 0, depth),
  Tcqadpa(solver, p+n, p*q + n, 0, depth),
  Tdpacqa(solver, p+n, p*q + n, 0, depth),
  Tcbpcaq(solver, p*q + n, p+n, 0, depth),
  Tcaqcbp(solver, p*q + n, p+n, 0, depth)
{
  // Link all the edges of all the tiles together, as in the paper.

  // Tacac
  auto regionA =  Tacac.restrict(0,0, p+n,n);
  solver->require(Tacac.restrict(0,p, p+n,p+n) == regionA);
  auto regionC =  Tacac.restrict(0,0, n,  p+n);
  solver->require(Tacac.restrict(p,0, p+n,p+n) == regionC);

  // Tbcbc
  auto regionB =  Tbcbc.restrict(0,0, q+n,n);
  solver->require(Tbcbc.restrict(0,p, q+n,p+n) == regionB);
  solver->require(Tbcbc.restrict(0,0, n,  p+n) == regionC);
  solver->require(Tbcbc.restrict(q,0, q+n,p+n) == regionC);

  // Tadad
  solver->require(Tadad.restrict(0,0, p+n,n)   == regionA);
  solver->require(Tadad.restrict(0,q, p+n,q+n) == regionA);
  auto regionD =  Tadad.restrict(0,0, n,  q+n);
  solver->require(Tadad.restrict(p,0, p+n,q+n) == regionD);

  // Tbdbd
  solver->require(Tbdbd.restrict(0,0, q+n,n)   == regionB);
  solver->require(Tbdbd.restrict(0,q, q+n,q+n) == regionB);
  solver->require(Tbdbd.restrict(0,0, n,  q+n) == regionD);
  solver->require(Tbdbd.restrict(q,0, q+n,q+n) == regionD);

  // Tcabcab
  solver->require(Tcabcab.restrict(0,0, q+n,  n)   == regionB);
  solver->require(Tcabcab.restrict(q,0, p+q+n,n)   == regionA);
  solver->require(Tcabcab.restrict(0,p, p+n,  p+n) == regionA);
  solver->require(Tcabcab.restrict(p,p, p+q+n,p+n) == regionB);
  solver->require(Tcabcab.restrict(0,  0, n,    p+n) == regionC);
  solver->require(Tcabcab.restrict(p+q,0, p+q+n,p+n) == regionC);

  // Tcbacba
  solver->require(Tcbacba.restrict(0,0, p+n,  n)   == regionA);
  solver->require(Tcbacba.restrict(p,0, p+q+n,n)   == regionB);
  solver->require(Tcbacba.restrict(0,p, q+n,  p+n) == regionB);
  solver->require(Tcbacba.restrict(q,p, p+q+n,p+n) == regionA);
  solver->require(Tcbacba.restrict(0,  0, n,    p+n) == regionC);
  solver->require(Tcbacba.restrict(p+q,0, p+q+n,p+n) == regionC);  

  // Tcdacda
  solver->require(Tcdacda.restrict(0,0, n,  q+n)   == regionD);
  solver->require(Tcdacda.restrict(0,q, n,  p+q+n) == regionC);
  solver->require(Tcdacda.restrict(p,0, p+n,p+n)   == regionC);
  solver->require(Tcdacda.restrict(p,p, p+n,p+q+n) == regionD);
  solver->require(Tcdacda.restrict(0,0,   p+n,n)     == regionA);
  solver->require(Tcdacda.restrict(0,p+q, p+n,p+q+n) == regionA);

  // Tdcadca
  solver->require(Tdcadca.restrict(0,0, n,  p+n)   == regionC);
  solver->require(Tdcadca.restrict(0,p, n,  p+q+n) == regionD);
  solver->require(Tdcadca.restrict(p,0, p+n,q+n)   == regionD);
  solver->require(Tdcadca.restrict(p,q, p+n,p+q+n) == regionC);
  solver->require(Tdcadca.restrict(0,0,   p+n,n)     == regionA);
  solver->require(Tdcadca.restrict(0,p+q, p+n,p+q+n) == regionA);

  // Tcqadpa
  for ( int i = 0; i < q; i++ ) {
    solver->require(Tcqadpa.restrict(0,i*p,  n,  (i+1)*p+n) == regionC);
  }
  for ( int i = 0; i < p; i++ ) {
    solver->require(Tcqadpa.restrict(p,i*q,  p+n,(i+1)*q+n) == regionD);
  }
  solver->require(Tcqadpa.restrict(0,0,   p+n,n)     == regionA);
  solver->require(Tcqadpa.restrict(0,q*p, p+n,q*p+n) == regionA);

  // Tdpacqa
  for ( int i = 0; i < p; i++ ) {
    solver->require(Tdpacqa.restrict(0,i*q,  n,  (i+1)*q+n) == regionD);
  }
  for ( int i = 0; i < q; i++ ) {
    solver->require(Tdpacqa.restrict(p,i*p,  p+n,(i+1)*p+n) == regionC);
  }
  solver->require(Tdpacqa.restrict(0,0,   p+n,n)     == regionA);
  solver->require(Tdpacqa.restrict(0,q*p, p+n,q*p+n) == regionA);

  // Tcbpcaq
  for ( int i = 0; i < q; i++ ) {
    solver->require(Tcbpcaq.restrict(i*p,0,  (i+1)*p+n,n)   == regionA);
  }
  for ( int i = 0; i < p; i++ ) {
    solver->require(Tcbpcaq.restrict(i*q,p,  (i+1)*q+n,p+n) == regionB);
  }
  solver->require(Tcbpcaq.restrict(0,  0, n,    p+n) == regionC);
  solver->require(Tcbpcaq.restrict(q*p,0, q*p+n,p+n) == regionC);

  // Tcaqcbp
  for ( int i = 0; i < p; i++ ) {
    solver->require(Tcaqcbp.restrict(i*q,0,  (i+1)*q+n,n)   == regionB);
  }						           
  for ( int i = 0; i < q; i++ ) {		           
    solver->require(Tcaqcbp.restrict(i*p,p,  (i+1)*p+n,p+n) == regionA);
  }
  solver->require(Tcaqcbp.restrict(0,  0, n,    p+n) == regionC);
  solver->require(Tcaqcbp.restrict(q*p,0, q*p+n,p+n) == regionC);
}  
  

template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const TwelveTiles<Scalar>& tt) {
  using std::endl;
  out << "Tacac:" << endl;
  out << tt.Tacac;

  out << endl;
  out << "Tadad:" << endl;
  out << tt.Tadad << endl;

  out << endl;
  out << "Tbcbc:" << endl;
  out << tt.Tbcbc << endl;

  out << endl;
  out << "Tbdbd:" << endl;
  out << tt.Tbdbd << endl;
  

  out << endl;
  out << "Tcabcab:" << endl;
  out << tt.Tcabcab <<endl;

  out << endl;
  out << "Tcbacba:" << endl;
  out << tt.Tcbacba << endl;

  out << endl;
  out << "Tcdacda:" << endl;
  out << tt.Tcdacda << endl;

  out << endl;
  out << "Tdcadca:" << endl;
  out << tt.Tdcadca << endl;


  out << endl;
  out << "Tcqadpa:" << endl;
  out << tt.Tcqadpa << endl;

  out << endl;
  out << "Tdpacqa:" << endl;
  out << tt.Tdpacqa << endl;

  out << endl;
  out << "Tcbpcaq:" << endl;
  out << tt.Tcbpcaq << endl;

  out << endl;
  out << "Tcaqcbp:" << endl;
  out << tt.Tcaqcbp << endl;

}

#endif
