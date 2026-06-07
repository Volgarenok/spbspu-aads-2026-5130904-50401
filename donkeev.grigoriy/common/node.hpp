#ifndef NODE_HPP
#define NODE_HPP
namespace detail
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
  };
}
#endif
