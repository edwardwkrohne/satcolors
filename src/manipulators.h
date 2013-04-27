// -*- Mode:C++ -*-
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
