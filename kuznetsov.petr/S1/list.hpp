#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
namespace kuznetsov {

  namespace {
    template< class T >
    struct Node {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }
  

}

#endif