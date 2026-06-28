#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../my_list/list.hpp"
#include <iostream>
#include <stdexcept>

namespace zubarev
{
  template< typename T >

  class Queue
  {

  public:
    void push(const T& rhs);
    void drop();
    const T& top() const;
    const T& last() const;
    bool empty() const;
    size_t size() const;

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
  void Queue< T >::drop()
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    list_.pop_front();
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
  const T& Queue< T >::last() const
  {
    if (list_.empty()) {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.back();
  }
  template< class T >
  bool Queue< T >::empty() const
  {
    return list_.empty();
  }
  template< class T >
  size_t Queue< T >::size() const
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
