#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace kuznetsov {
  template< class T >
  class Stack {
  public:
    Stack();
    Stack(const Stack< T >& s);

    Stack& operator=(const Stack< T >& s);
    Stack& operator=(Stack< T >&& s);

    T& top();

    bool empty();
    size_t size();

    void push();
    void pop();

  private:
    List< T > list_;
  };


}
#endif
