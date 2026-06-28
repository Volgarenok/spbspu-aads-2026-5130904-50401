#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <utility>
#include "list.hpp"

namespace levkin {
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;

    void push(const T& v);
    void push(T&& v);
    void pop();

    T& top();
    const T& top() const;
    bool empty() const;
    size_t size() const;

  private:
    List< T > list_;
  };

  template < class T >
  void Stack< T >::push(const T& v)
  {
    list_.pushFront(v);
  }

  template < class T >
  void Stack< T >::push(T&& v)
  {
    list_.pushFront(std::move(v));
  }

  template < class T >
  void Stack< T >::pop()
  {
    if (!empty()) {
      list_.popFront();
    }
  }

  template < class T >
  T& Stack< T >::top()
  {
    if (empty()) {
      throw std::out_of_range("stack is empty!");
    }
    return *list_.begin();
  }

  template < class T >
  const T& Stack< T >::top() const
  {
    if (empty()) {
      throw std::out_of_range("stack is empty!");
    }
    return *list_.cbegin();
  }

  template < class T >
  bool Stack< T >::empty() const
  {
    return size() == 0;
  }

  template < class T >
  size_t Stack< T >::size() const
  {
    return list_.size();
  }
}

#endif
