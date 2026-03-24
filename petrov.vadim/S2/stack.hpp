#ifndef STACK_HPP
#define STACK_HPP

#include "common/STRUCTS.hpp"

namespace petrov
{
  template<typename T>
  class Stack {
  public:
    Stack();
    Stack(const Stack<T>& s);
    Stack(Stack<T>&& s);

    Stack<T>& operator=(const Stack<T>& s);
    Stack<T>& operator=(Stack<T>&& s);

    void push(const T& rhs);
    T drop();

    const T& top() const;
    void pop();
    bool empty() const;
    size_t getSize() const;
    void clear();
    ~Stack();

  private:
    List<T> list_;
  };

  template<typename T>
  Stack<T>::Stack():
    list_()
  {}

  template<typename T>
  Stack<T>::Stack(const Stack<T>& s):
    list_(s.list_)
  {}

  template<typename T>
  Stack<T>::Stack(Stack<T>&& s):
    list_(std::move(s.list_))
  {}

  template<typename T>
  Stack<T>& Stack<T>::operator=(const Stack<T>& s)
  {
    if (this != &s)
    {
      list_ = s.list_;
    }
    return *this;
  }

  template<typename T>
  Stack<T>& Stack<T>::operator=(Stack<T>&& s)
  {
    if (this != &s)
    {
      list_ = std::move(s.list_);
    }
    return *this;
  }

  template<typename T>
  void Stack<T>::push(const T& rhs)
  {
    list_.insert(list_.begin(), rhs);
  }

  template<typename T>
  T Stack<T>::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    T res = top();
    pop();
    return res;
  }

  template<typename T>
  const T& Stack<T>::top() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template<typename T>
  void Stack<T>::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    list_.popStart();
  }

  template<typename T>
  bool Stack<T>::empty() const
  {
    return list_.size() == 0;
  }

  template<typename T>
  size_t Stack<T>::getSize() const
  {
    return list_.size();
  }

  template<typename T>
  void Stack<T>::clear()
  {
    list_.clear();
  }

  template<typename T>
  Stack<T>::~Stack()
  {
    clear();
  }
}

#endif
