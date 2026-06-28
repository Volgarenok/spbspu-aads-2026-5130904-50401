#ifndef STACK_HPP
#define STACK_HPP
#include "../my_list/list.hpp"
#include <iostream>
#include <stdexcept>

namespace zubarev
{
  template< typename T >
  class Stack
  {

  public:
    void push(const T& rhs);
    void drop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
    void print(std::ostream& out = std::cout) const;

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
  void Stack< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Stack is empty");
    }
    list_.pop_front();
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
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }
  template< class T >
  size_t Stack< T >::size() const
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
