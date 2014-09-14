// -*- Mode:C++ -*-
//
// Frequently, a compound object must be built out of primitives like cardinals and matrices,
//  and other compound objects.  The code for doing so can get rather redundant in some places.
// This template class can derive from a specially written, compact implementation class that
// contains only the bare necessities.
//
// If T is a class containing the appropriate members, then CompoundObject<T> will:
//  * Support all constructors that T does
//  * Support a getNumLiterals() that adds up its members' getNumLiterals()
//  * Support a typeRequierment() class that conjoins its members's typeRequirement() and T::typeRequirement()
//  * Support a currSolnReq() function that negates T::diffSolnReq()
//  * Register requirements or not, depending on the value of unsigned int.
//
// In the future, I would like to add additional functionality:
//  * Default typeRequirement() to just conjoin the member's typeRequirement if T::typeRequirement() does not exist
//  * Make currSolnReq(), diffSolnReq() use whichever (zero, one, or both) of T::currSolnReq(), T::diffSolnReq(), with
//    a default implementation if none are available.

#ifndef COMPOUNDOBJECT_H
#define COMPOUNDOBJECT_H

#include <utility>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include "requirement.h"
#include "lastarg.h"
#include "matrix.h"

template<class T>
class CompoundObject;

// The following classes help with classifying argument lists
template<class... ArgsT>
struct EndsWithVar : public std::is_same<typename lastArg_type<ArgsT...>::type, unsigned int&>
{};

template<class BaseT, class... ArgsT>
struct EndsWithBaseT : public std::is_same<typename std::decay<typename lastArg_type<ArgsT...>::type>::type, BaseT>
{};

template<class BaseT, class... ArgsT>
struct CopyConstructorArgs : public std::integral_constant<bool, sizeof...(ArgsT) == 1 && EndsWithBaseT<CompoundObject<BaseT>, ArgsT...>::value >
{};

template<class BaseT, class... ArgsT>
struct ExplicitVarArgs : public std::integral_constant<bool, !CopyConstructorArgs<BaseT, ArgsT...>::value && EndsWithVar<ArgsT...>::value >
{};

template<class BaseT, class... ArgsT>
struct ImplicitVarArgs : public std::integral_constant<bool, !CopyConstructorArgs<BaseT, ArgsT...>::value && !EndsWithVar<ArgsT...>::value >
{};


template<class T>
class CompoundObject : public T {
public:
  // Number of objects in T's tuple
  const static unsigned int numObjs =
      std::tuple_size<decltype(std::declval<T>().objTuple)>::value;

  template<class... ArgsT>
  CompoundObject(ArgsT&&... args);

  template<unsigned I = 0>
  typename std::enable_if<(I == numObjs), Cardinal::value_type>::type
  numLiterals() const;

  template<unsigned I = 0>
  typename std::enable_if<(I < numObjs), Cardinal::value_type>::type
  numLiterals() const;

  DualClause currSolnReq() const;

  template<unsigned I = 0>
  typename std::enable_if<(I == numObjs), Requirement>::type
  typeRequirement() const;

  template<unsigned I = 0>
  typename std::enable_if<(I < numObjs), Requirement>::type
  typeRequirement() const;

private:
  // FIXME Clean this up.
  // I only know how to do this sort of template manipulation if
  // I have a return type to work with, so I can't do it directly
  // on the constructor.  Perhaps when I learn more I won't need
  // this helper.
  template<class... ArgsT>
  typename std::enable_if<CopyConstructorArgs<T, ArgsT...>::value, void>::type
  constructorHelper(ArgsT&&... args);

  template<class... ArgsT>
  typename std::enable_if<ExplicitVarArgs<T, ArgsT...>::value, void>::type
  constructorHelper(ArgsT&&... args);

  template<class... ArgsT>
  typename std::enable_if<ImplicitVarArgs<T, ArgsT...>::value, void>::type
  constructorHelper(ArgsT&&... args);

};

template<class T>
template<class... ArgsT>
CompoundObject<T>::CompoundObject(ArgsT&&... args) :
  T(std::forward<ArgsT>(args)...)
{
  constructorHelper(std::forward<ArgsT>(args)...);
}

template<class T>
template<class... ArgsT>
typename std::enable_if<ExplicitVarArgs<T, ArgsT...>::value, void>::type
CompoundObject<T>::constructorHelper(ArgsT&&... args)
{
  // If this function is getting called, then the last argument is a unsigned int&.  Check to see if it indicates
  //  we should register requirements.
  if ( lastArg(args...) == SolverManager::allocateNew ) {
    T::manager->require(T::typeRequirement());
  }
}

template<class T>
template<class... ArgsT>
typename std::enable_if<ImplicitVarArgs<T, ArgsT...>::value, void>::type
CompoundObject<T>::constructorHelper(ArgsT&&... args)
{
  // If this function is getting called, then the last argument is NOT a unsigned int&.
  // Execute the default behavior of registering requirements anyway.
  T::manager->require(T::typeRequirement());
}

template<class T>
template<class... ArgsT>
typename std::enable_if<CopyConstructorArgs<T, ArgsT...>::value, void>::type
CompoundObject<T>::constructorHelper(ArgsT&&... args)
{
  // If this function is getting called, then the arguments amount to a copy
  // constructor.  Do NOT register requirements for a copy constructor.
  ;
}

template<class T>
template<unsigned I>
inline typename std::enable_if<(I == CompoundObject<T>::numObjs), Cardinal::value_type>::type
CompoundObject<T>::numLiterals() const
{
  return 0;
}

template<class T>
template<unsigned I>
inline typename std::enable_if<(I < CompoundObject<T>::numObjs), Cardinal::value_type>::type
CompoundObject<T>::numLiterals() const
{
  return std::get<I>(T::objTuple).numLiterals() + numLiterals<I+1>();
}

template<class T>
DualClause CompoundObject<T>::currSolnReq() const {
  return ~T::diffSolnReq();
}

template<class T>
template<unsigned I>
typename std::enable_if<(I == CompoundObject<T>::numObjs), Requirement>::type
CompoundObject<T>::typeRequirement() const
{
  return T::typeRequirement();
}

template<class T>
template<unsigned I>
typename std::enable_if<(I < CompoundObject<T>::numObjs), Requirement>::type
CompoundObject<T>::typeRequirement() const
{
  return std::get<I>(T::objTuple).typeRequirement() & typeRequirement<I+1>();
}
#endif // COMPOUNDOBJECT_H
