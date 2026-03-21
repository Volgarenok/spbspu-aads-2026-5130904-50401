#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"
namespace kuznetsov {
  template< class T >
  class Queue {
  public:
    Queue();
    Queue(Queue< T >&& q);

    Queue& operator=(const Queue< T >& q);
    Queue& operator=(Queue< T >&& q);

    T& front();
    T& back();

    void push(T rhs);
    T pop();

    bool empty();
    size_t size();
  private:
    List< T > list_;
  };





}

#endif

