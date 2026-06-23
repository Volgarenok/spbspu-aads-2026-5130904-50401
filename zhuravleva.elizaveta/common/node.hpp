#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace zhuravleva
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T data;
      Node< T > * next;

      Node(const T & value, Node< T >* n = nullptr):
        data(value),
        next(n)
      {}

      template< class... Args >
      Node(Node< T > * n, Args&&... args):
        data(std::forward< Args >(args)...),
        next(n)
      {}
    };
  }
}

#endif
