// -*- Mode:C++ -*-
// 
// array2d.h
//
//  A nice stl-style 2d array.  Automatically deallocates memory when it goes out of scope.
//  To access, use [] syntax.  For example, array[i][j].
//

#ifndef ARRAY2D_H_
#define ARRAY2D_H_

#include <vector>
#include <functional>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template<typename _return_type, typename _size_type = std::vector<int>::size_type>
class SubscriptWrapper {
public:
  typedef _size_type size_type;
  typedef std::function<_return_type(_size_type)> functor_type;
  
  SubscriptWrapper(const functor_type functor);

  _return_type operator[] (size_type index) const;

private:
  functor_type functor;
};

template<typename return_type, typename size_type>
SubscriptWrapper<return_type, size_type>::SubscriptWrapper(const functor_type _functor) :
  functor(_functor)
{

}

template<typename return_type, typename size_type>
return_type SubscriptWrapper<return_type, size_type>::operator[](size_type index) const {
  return functor(index);
}

template<class T>
class Array2d {
public:
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef T value_type;

  Array2d() = delete;
  Array2d(size_type height, size_type width);
  Array2d(std::initializer_list<std::initializer_list<T>> initList);

  SubscriptWrapper<reference, size_type> operator[] (size_type row);
  SubscriptWrapper<const_reference, size_type> operator[] (size_type row) const;

  void swap(Array2d<T>& other);

  // Want these to be read-only by clients, so they are functions.
  size_type height() const { return m_height; };
  size_type width() const { return m_width; };
private:
  size_type m_height;
  size_type m_width;
  std::vector<T> m_data;
};

template<class T>
Array2d<T>::Array2d(size_type _height, size_type _width) :
  m_data (_height*_width),
  m_height(_height),
  m_width(_width)
{
}

template<class T>
Array2d<T>::Array2d(std::initializer_list<std::initializer_list<T>> initList) :
  m_data (0),
  m_height(0),
  m_width(0)
{
  // If there's nothing to work with, then just leave it empty.
  if ( initList.size() == 0 ) {
    return;
  }

  m_height = initList.size();
  m_width = initList.begin()->size();
  m_data.resize(m_width*m_height);

  // Perhaps there is a way to avoid this copy by plundering the intializer_list?
  auto dataIter = m_data.begin();
  for ( auto rowIter = initList.begin(); rowIter != initList.end(); rowIter++ ) {
    if ( rowIter->size() != m_width ) {
      std::ostringstream strout;
      strout
          << "Width mismatch in Array2d construction. Row 0 has width " << m_width
          << " and row " << (rowIter - initList.begin()) << " has width " << rowIter->size() << ".";
      throw std::invalid_argument(strout.str());
    }

    dataIter = copy(rowIter->begin(), rowIter->end(), dataIter);
  }
}


template<class T>
SubscriptWrapper<typename Array2d<T>::reference, typename Array2d<T>::size_type> 
Array2d<T>::operator[] (size_type row) {
  // Return an object whose subscript operator returns row*width + col
  auto lambda = [=] (size_type col)  -> reference {
    return this->m_data.begin()[row*this->m_width + col];
  };
  return SubscriptWrapper<reference, size_type>(lambda);
}

template<class T>
SubscriptWrapper<typename Array2d<T>::const_reference, typename Array2d<T>::size_type> Array2d<T>::operator[] (size_type row) const {
  // Return an object whose subscript operator returns row*width + col
  auto lambda = [=] (size_type col)  -> const_reference {
    return this->m_data.begin()[row*this->m_width + col];
  };
  return SubscriptWrapper<const_reference, size_type>(lambda);
}


template<class T>
void Array2d<T>::swap (Array2d<T>& other) {
  m_data.swap(other.m_data);
  std::swap(m_height, other.m_height);
  std::swap(m_width, other.m_width);
}

#endif /* ARRAY2D_H_ */
