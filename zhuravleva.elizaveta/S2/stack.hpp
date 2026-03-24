#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "common/list.hpp"
#include "../common/node.hpp"
#include "../common/iterator.hpp"

namespace zhuravleva
{
  template<class T>
  class Stack
  {
  private:
    List<T> data;

  public:
    void push(const T& value);
    void pop();
    T top();
    bool empty() const;
    void clear();
  };
}

#endif