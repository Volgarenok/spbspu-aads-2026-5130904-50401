#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>
#include <utility>
#include <list.hpp>
namespace madieva {
  template< class T >
  class Stack {
  public:
    void push(const T & val);
    void push(T && val);
    template< class... Args >
    T & emplace(Args &&... args);
    T & top();
    const T & top() const;
    size_t size() const noexcept;
    void pop() noexcept;
    void clear() noexcept;
    bool empty() const noexcept;
  private:
    List< T > data_;
  };


  template< class T >
  void Stack< T >::push(const T & val)
  {
    data_.pushBack(val);
  }

  template< class T >
  void Stack< T >::push(T && val)
  {
    data_.pushBack(std::forward< T >(val));
  }

  template< class T >
  template< class... Args >
  T & Stack< T >::emplace(Args &&... args)
  {
    return data_.emplace_back(std::forward< Args >(args)...);
  }

  template< class T >
  T & Stack< T >::top()
  {
    if (!empty()) {
      LIter< T > it = data_.end();
      --it;
      return (*it);
    } else {
      throw std::runtime_error("Stack is empty");
    }
  }

  template< class T >
  const T & Stack< T >::top() const
  {
    if (!empty()) {
      LCIter< T > it = data_.end();
      --it;
      return (*it);
    } else {
      throw std::runtime_error("Stack is empty");
    }
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    data_.popBack();
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    data_.clear();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.size() == 0;
  }
}

#endif
