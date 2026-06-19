#ifndef NODE_HPP
#define NODE_HPP

namespace malashenko
{
  namespace detail
  {
    template< class T >
    struct Node {
      T value_;
      Node< T >* next;
      Node< T >* prev;
    };
  }
}


#endif
