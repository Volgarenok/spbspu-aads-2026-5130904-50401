#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "../common/list.hpp"
#include "../common/node.hpp"
#include "../common/iterator.hpp"

namespace zhuravleva
{
  template<class T>
  class Queue
  {
  private:
    List<T> data;

  public:
    void push(const T& value);
    void pop();
    T front();
    bool empty() const;
    void clear();
  };
}

#endif