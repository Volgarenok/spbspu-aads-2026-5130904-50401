#ifndef NODE
#define NODE

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
