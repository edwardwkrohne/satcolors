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
// Utility class to help with outputting a gnuplot script.
//
// Accepts several ostream-style manipulators (of its own) which adjust plotting settings.
// Everything else is considered matrix data to be plotted, and is wrapped in suitable plot commands.
#ifndef MANIPULATORS_H
#define MANIPULATORS_H

#include <iostream>
#include <functional>
#include <string>
#include <vector>

// Tell ostreams to interpret std::function as a manipulator, wherever it sees one.
std::ostream& operator<<(std::ostream& stream, const std::function<std::ostream& (std::ostream&)>& manipulator);

// Gnuplot commands are passed directly to the interpreter, without plotting commands
class GnuplotCommand {
public:
  GnuplotCommand(const std::string& command);
  std::string command;
};

class Gnuplot {
public:
  Gnuplot(std::ostream& out);

  template <class T>
  Gnuplot& operator << (const T& input);

  // This first-class (overloaded) function takes precedence over function templates (I think)
  Gnuplot& operator << (const GnuplotCommand& command);
private:
  Gnuplot(const Gnuplot& copy);
  std::ostream& out;
};

template<class T>
Gnuplot& Gnuplot::operator << (const T& input) {
  out << "plot '-' matrix using (0.5+$1):(0.5+$2):($3) with image\n";
  out << input;
  out << "e\ne\n" << std::flush;
  return *this;
}

GnuplotCommand multiplot(unsigned int height, unsigned int width);
GnuplotCommand tics(unsigned int ytics, unsigned int xtics);
GnuplotCommand title(std::string title);
extern GnuplotCommand nogrid;

std::ostream& timestamp(std::ostream & out);
std::ostream& newl(std::ostream & out);

#endif // MANIPULATORS_H
