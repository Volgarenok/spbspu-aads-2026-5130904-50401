#ifndef STACK_HPP
#define STACK_HPP
#include <iostream>
#include <stdexcept>

#include <list.hpp>

namespace zubarev
{
  template< typename T >
  class Stack
  {

  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void drop();
    void pop();
    const T& top() const;
    T& top();
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > list_;
    size_t size_ = 0;
    template< class U >
    friend std::ostream& operator<<(std::ostream& out, const Stack< U >& s);
  };

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    list_.push_front(rhs);
    size_++;
  }
  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    list_.push_front(std::move(rhs));
    size_++;
  }
  template< class T >
  void Stack< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    list_.pop_front();
    size_--;
  }
  template< class T >
  void Stack< T >::pop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    list_.pop_back();
    size_--;
  }
  template< class T >
  const T& Stack< T >::top() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  T& Stack< T >::top()
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    return *list_.begin();
  }

  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }
  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }
  template< class U >
  std::ostream& operator<<(std::ostream& out, const Stack< U >& s)
  {

    out << "Stack[ size = " << s.size_ << " ]: < ";

    for (auto it = s.list_.begin(); it != s.list_.end(); ++it) {
      out << *it << " ";
    }
    out << ">" << '\n';
    return out;
  }
}

#endif
