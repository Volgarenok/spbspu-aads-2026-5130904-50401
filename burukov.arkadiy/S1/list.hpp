#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace burukov
{
  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T val_;
    Node< T > *next_;
    Node(const T &value, Node< T > *next):
      val_(value),
      next_(next)
    {}
  };
}

#endif