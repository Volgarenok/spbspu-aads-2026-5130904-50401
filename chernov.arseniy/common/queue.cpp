#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list.hpp>

namespace chernov {
  template< class T >
  class Queue {
  public:
    Queue();
    ~Queue();
    void push(T rhs);
    T drop();
  private:
    List< T > list_;
  };
}

#endif
