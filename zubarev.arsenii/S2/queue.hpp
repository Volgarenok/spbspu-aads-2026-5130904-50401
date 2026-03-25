#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"
#include <iostream>
#include <stdexcept>
namespace zubarev
{
  template < typename T >

  class Queue
  {
  private:
    List< T > list_;
    size_t size_ = 0;

  public:
    void push(const T& rhs);
    T drop();
    const T& top() const;
    const T& last() const;
    bool empty() const;
    size_t size() const;
    void print(std::ostream& out = std::cout) const;
  };
  template < class T >
  void Queue< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
    size_++;
  }
  template < class T >
  T Queue< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    T val = top();
    list_.pop_front();
    size_--;
    return val;
  }
  template < class T >
  const T& Queue< T >::top() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }
  template < class T >
  const T& Queue< T >::last() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.back();
  }
  template < class T >
  bool Queue< T >::empty() const
  {
    return list_.empty();
  }
  template < class T >
  size_t Queue< T >::size() const
  {
    return size_;
  }
  template < class T >
  void Queue< T >::print(std::ostream& out) const
  {
    out << "Queue[ size = " << size_ << " ]: < ";

    for (auto it = list_.begin(); it != list_.end(); ++it) {
      out << *it << " ";
    }
    out << ">" << '\n';
  }
}

#endif
