#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list.hpp>
#include <stdexcept>
#include <utility>

namespace ulanova
{
  template < class T >
  class Queue
  {
  public:
    void push(const T& value);
    void push(T&& value);
    void clear();
    T& front();
    const T& front() const;
    void pop();
    bool empty() const;
  private:
    List< T > list_;
  };

  template < class T >
  void Queue< T >::push(const T& value)
  {
    list_.push_back(value);
  }
  template < class T >
  void Queue< T >::push(T&& value)
  {
    list_.push_back(std::forward< T >(value));
  }
  template < class T >
  void Queue< T >:: clear()
  {
    list_.clear();
  }
  template< class T >
  T& Queue< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return list_.front();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return list_.front();
  }
  template < class T >
  void Queue< T >::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    list_.pop_front();
  }
  template < class T >
  bool Queue< T >::empty() const
  {
    return list_.empty();
  }
}
#endif
