#ifndef STACK_HPP
#define STACK_HPP
#include "../common/list.hpp"
#include <stdexcept>

namespace ulanova
{
  template <class T>
  class Stack
  {
  private:
    List<T> list;
  public:
    Stack() = default;

    void push(const T& value);
    void clear();
    T drop();
    T& front();
    bool empty();
  };

  template <class T>
  void Stack<T>::push(const T& value)
  {
    list.push_front(value);
  }
  template <class T>
  void Stack<T>::clear()
  {
    while (!empty())
    {
      drop();
    }
  }
  template <class T>
  T Stack<T>::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return list.pop_front();
  }
  template <class T>
  T& Stack<T>::front()
  {
    if(empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return list.front();
  }
  template <class T>
  bool Stack<T>::empty()
  {
    return list.empty
  }
}
#endif
