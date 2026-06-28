#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <utility>
#include "list.hpp"

namespace levkin {
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;

    void push(const T& v);
    void push(T&& v);
    void pop();

    T& front();
    const T& front() const;
    bool empty() const;
    size_t size() const;

  private:
    levkin::List< T > list_;
  };

  template < class T >
  void Queue< T >::push(const T& v)
  {
    list_.pushBack(v);
  }

  template < class T >
  void Queue< T >::push(T&& v)
  {
    list_.pushBack(std::move(v));
  }

  template < class T >
  void Queue< T >::pop()
  {
    if (!empty()) {
      list_.popFront();
    }
  }

  template < class T >
  T& Queue< T >::front()
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty!");
    }
    return *list_.begin();
  }

  template < class T >
  const T& Queue< T >::front() const
  {
    if (empty()) {
      throw std::out_of_range("Queue is empty!");
    }
    return *list_.cbegin();
  }

  template < class T >
  bool Queue< T >::empty() const
  {
    return list_.size() == 0;
  }

  template < class T >
  size_t Queue< T >::size() const
  {
    return list_.size();
  }
}

#endif
