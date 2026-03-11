#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "list.hpp"
#include "node.hpp"

template< class T >
class LIter
{
  friend class List< T >;
  Node< T >* n;

};
#endif