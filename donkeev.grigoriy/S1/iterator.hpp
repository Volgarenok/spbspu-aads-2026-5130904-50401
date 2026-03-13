#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "list.hpp"
#include "node.hpp"

template< class T >
class LIter
{
  friend class List< T >;
  Node< T >* n;

  LIter();
  ~LIter();

  LIter< T >& operator++();
  LIter< T > operator++(int);

  T& operator*();
  T* operator ->();

  bool operator==(LIter< T >) const;
  bool operator!=(LIter< T >) const;

};
#endif