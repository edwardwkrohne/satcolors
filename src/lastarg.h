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
// Retrieves the last argument in a variable length argument list

#ifndef LASTARG_H
#define LASTARG_H

template<class... Types>
struct lastArg_type;

template<>
struct lastArg_type<>
{
  typedef void type;
};

template<class First>
struct lastArg_type<First>
{
  typedef First type;
};


template<class First, class... Types>
struct lastArg_type<First, Types...> : public lastArg_type<Types...>
{
};

template<class First>
First& lastArg(First& arg1) {
  return arg1;
}

template<class First, class Second, class... Types>
typename lastArg_type<First, Second, Types...>::type& lastArg(First& arg1, Second& arg2, Types&... args)
{
  return lastArg(arg2, args...);
}

#endif // LASTARG_H
