#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include "iterator.hpp"

namespace donkeev
{
  template< class T > class LIter;
  template< class T > class Node;

  template< class T >
  class List
  {
    Node< T >* head;
    Node< T >* tail;
    size_t length;
  public:
    List< T >();
    List< T >(size_t, T);
    List< T >(const List< T >&);

    LIter< T >* begin();
    LIter< T >* end();

    void pushAfter();
    void pushFront();
    void pushEnd();
    
    void popFront();
    void popEnd();
    void cut(LIter< T >);
    void clearAll();

    bool isEmpty();
    size_t size();
  };
}
#endif