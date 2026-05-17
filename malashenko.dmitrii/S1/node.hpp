#ifndef NODE
#define NODE

namespace detail
{
  template< class T >
  struct Node {
    T value_;
    Node< T >* next;
    Node< T >* prev;
  };
}

#endif
