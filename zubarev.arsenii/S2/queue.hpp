#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include <stdexcept>

#include <list.hpp>

namespace zubarev
{
  template< typename T >

  class Queue
  {

  public:
    void push(const T& rhs);
    void push(T&& rhs);
    void drop();
    void pop();
    const T& top() const;
    T& top();
    const T& last() const;
    T& last();
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > list_;
    size_t size_ = 0;
    template< class U >
    friend std::ostream& operator<<(std::ostream& out, const Queue< U >& q);
  };
  template< class T >
  void Queue< T >::push(const T& rhs)
  {
    list_.push_back(rhs);
    size_++;
  }
  template< class T >
  void Queue< T >::push(T&& rhs)
  {
    list_.push_back(std::move(rhs));
    size_++;
  }
  template< class T >
  void Queue< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    list_.pop_front();
    size_--;
  }
  template< class T >
  void Queue< T >::pop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    list_.pop_back();
    size_--;
  }
  template< class T >
  const T& Queue< T >::top() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }
  template< class T >
  T& Queue< T >::top()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }
  template< class T >
  const T& Queue< T >::last() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.back();
  }
  template< class T >
  T& Queue< T >::last()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.back();
  }
  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }
  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return size_;
  }
  template< class U >
  std::ostream& operator<<(std::ostream& out, const Queue< U >& q)
  {
    out << "Queue[ size = " << q.size_ << " ]: < ";

    for (auto it = q.list_.begin(); it != q.list_.end(); ++it) {
      out << *it << " ";
    }
    out << ">" << '\n';
    return out;
  }
}

#endif
