#ifndef MY_QUEUE
#define MY_QUEUE
#include "list.hpp"
namespace levkin {
  template < typename T > class Queue
  {

  private:
    levkin::List< T > list_;

  public:
    Queue() : list_(List< T >{}) {}
    void push(T rhs);
    T drop();
  };
  template < class T > T levkin::Queue< T >::drop()
  {
    auto iter = list_.cbegin();
    T val = *iter;
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Queue< T >::push(T v) { list_.pushBack(v); }

}

#endif
