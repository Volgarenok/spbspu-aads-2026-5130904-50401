#ifndef NODE_HPP
#define NODE_HPP
namespace donkeev
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
    Node(); 
  };

  template< class T >
  Node< T >::Node():
    val(T()),
    next(nullptr)
  {}
}
#endif