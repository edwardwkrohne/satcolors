// -*- Mode:C++ -*-
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
