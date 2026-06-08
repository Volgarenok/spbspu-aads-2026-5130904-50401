#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <cctype>
#include <stdexcept>
#include "list.hpp"

namespace vasyakin
{
  template< class T >
  class Queue
  {
  public:
    void push(const T& value);
    void push(T&& value);

    void pop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;

    template< class... Args >
    void emplace(Args&&... args);
  private:
    vasyakin::List< T > list_;
  };

  template< class T >
  void Queue< T >::push(const T& value)
  {
    list_.pushBack(value);
  }

  template< class T >
  void Queue< T >::push(T&& value)
  {
    list_.emplace_back(std::move(value));
  }

  template< class T >
  void Queue< T >::pop()
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error("Queue is empty");
    }
    list_.erase(list_.end());
  }

  template< class T >
  T& Queue< T >::front()
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }

  template< class T >
  const T& Queue< T >::front() const
  {
    if (list_.size() == 0)
    {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.size() == 0;
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  template< class... Args >
  void Queue< T >::emplace(Args&&... args)
  {
    list_.emplace_back(std::forward< Args >(args)...);
  }
}

#endif
