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
    size_t size();
    void pop();
    T front();
  };
  template < class T > T levkin::Queue< T >::drop()
  {
    T val = front();
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Queue< T >::pop() { list_.popFront(); }

  template < class T > T levkin::Queue< T >::front()
  {
    auto iter = list_.cbegin();
    T val = *iter;
    return val;
  }

  template < class T > size_t levkin::Queue< T >::size()
  {
    return list_.size();
  }

  template < class T > void levkin::Queue< T >::push(T v) { list_.pushBack(v); }

}

#endif
