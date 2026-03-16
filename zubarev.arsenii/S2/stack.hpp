// #include <iostream>
#include "list.hpp"
namespace zubarev
{
  template < typename T >
  class Stack
  {
  private:
    List< T > list_;
    size_t size_ = 0;

  public:
    void push(const T& rhs);
    T drop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
  };

  template < class T >
  void Stack< T >::push(const T& rhs)
  {
    list_.push_front(rhs);
    size_++;
  }
  template < class T >
  T Stack< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    T val = top() list_.pop_front();
    size_--;
    return val;
  }
  template < class T >
  const T& Stack< T >::top() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }
  template < class T >
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }
  template < class T >
  size_t Stack< T >::size() const
  {
    return size_;
  }
}
