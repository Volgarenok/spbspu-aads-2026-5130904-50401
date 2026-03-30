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
    size_t size();
    void pop();
    T front();
  };
  template < class T > T levkin::Stack< T >::drop()
  {
    T val = front();
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Stack< T >::pop() { list_.popFront(); }

  template < class T > T levkin::Stack< T >::front()
  {
    auto iter = list_.cbegin();
    T val = *iter;
    return val;
  }

  template < class T > size_t levkin::Stack< T >::size()
  {
    return list_.size();
  }

  template < class T > void levkin::Stack< T >::push(T v)
  {
    list_.pushFront(v);
  }

}

#endif
