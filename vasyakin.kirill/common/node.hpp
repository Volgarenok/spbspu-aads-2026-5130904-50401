#ifndef NODE_HPP
#define NODE_HPP

template< class T > class List;
template< class T > class LIter;
template< class T > class LCIter;

namespace vasyakin
{
  template< class T >
  class Node
  {
  private:
    T val;
    Node< T >* next;
    friend class List< T >;
    friend class LIter< T >;
    friend class LCIter< T >;

  public:
    explicit Node(const T& value);
  };

  template< class T >
  Node< T >::Node(const T& value):
    val(value),
    next(nullptr)
  {}
}

#endif
