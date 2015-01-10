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
// Miscellaneous manipulators, including implementation of the Gnuplot class.

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include "manipulators.h"

using namespace std;

std::ostream& operator<<(std::ostream& stream, const std::function<std::ostream& (std::ostream&)>& manipulator) {
  return manipulator( stream );
}

GnuplotCommand::GnuplotCommand(const string& _command) :
    command(_command)
{
  ;
}

Gnuplot::Gnuplot(std::ostream& _out) :
  out(_out)
{
  out <<
    "set term wxt size 700,700\n"
    "set palette gray\n"
    "set cbrange [0:7]\n"
    "unset label\n"
    "unset title\n"
    "unset key\n"
    "unset colorbox\n"
    "set xtics 1 format \"\"\n"
    "set ytics 1 format \"\"\n"
    "set grid front lt 1 lc 8\n"
    "set size ratio -1\n"
    "set yrange [] reverse\n"
      << flush;
}

// Output gnuplot commands without trying to plot them
Gnuplot& Gnuplot::operator << (const GnuplotCommand& command) {
  out << command.command;
  return *this;
}

GnuplotCommand multiplot(unsigned int height, unsigned int width) {
  ostringstream sout;
  sout << "set multiplot layout " << height << "," << width << endl;
  return GnuplotCommand(sout.str());
}

GnuplotCommand nogrid(
    "unset xtics\n"
    "unset ytics\n"
    );

GnuplotCommand tics(unsigned int ytics, unsigned int xtics) {
  ostringstream sout;
  sout << "set ytics " << ytics << endl;
  sout << "set xtics " << xtics << endl;
  return GnuplotCommand(sout.str());
}

GnuplotCommand title(string title) {
  ostringstream sout;
  if ( title.find('"') != string::npos ) {
    throw invalid_argument("Quotes not yet supported for title.  Code to properly backslashify them must be written.");
  }
  sout << "set title \"" << title << "\"" << endl;
  return GnuplotCommand(sout.str());
}


ostream& timestamp(ostream & out)
{
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );
  char oldFill = out.fill();
  return out << setfill('0')
      << setw(2) << now->tm_hour << ':'
      << setw(2) << now->tm_min << ':'
      << setw(2) << now->tm_sec
      << setfill(oldFill);
}

ostream& newl(ostream & out)
{
  return out << '\n';
}
