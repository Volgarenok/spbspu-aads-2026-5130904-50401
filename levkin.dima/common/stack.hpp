#ifndef MY_STACK
#define MY_STACK
#include "list.hpp"
namespace levkin {
  template < typename T > class Stack
  {

  private:
    levkin::List< T > list_;

  public:
    Stack() : list_(List< T >{}) {}
    void push(T rhs);
    T drop();
  };
  template < class T > T levkin::Stack< T >::drop()
  {
    auto iter = list_.cbegin();
    T val = *iter;
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Stack< T >::push(T v) { list_.pushFront(v); }

}

#endif
