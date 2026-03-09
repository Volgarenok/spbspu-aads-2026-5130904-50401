#ifndef NODE
#define NODE

namespace malashenko
{
  template< class T >
  struct Node {
    Node(T value, Node< T >* next, Node< T >* prev);
    T value_;
    Node< T >* next;
    Node< T >* prev;
  };

  template< class T >
  Node< T >::Node(T value, Node< T >* n, Node< T >* p):
    value_(value),
    next(n),
    prev(p)
  {}
}

#endif
