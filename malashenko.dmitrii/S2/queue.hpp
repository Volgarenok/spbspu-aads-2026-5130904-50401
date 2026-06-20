#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list/list.hpp>

namespace malashenko
{
  template< class T >
  class Queue {
  public:
    T& front() noexcept;
    const T& front() const noexcept;


    bool empty() const noexcept;
    size_t size() const noexcept;


    void push(const T& value);
    void push(T&& value);

    void pop() noexcept;
    void swap(Queue< T >& other) noexcept;
    void clear() noexcept;

  private:
    List< T > list_;
  };


  template< class T >
  T& Queue< T >::front() noexcept
  {
    return list_.front();
  }

  template< class T >
  const T& Queue< T >::front() const noexcept
  {
    return list_.front();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  void Queue< T >::push(const T& value)
  {
    list_.push_back(value);
  }

  template< class T >
  void Queue< T >::push(T&& value)
  {
    list_.push_back(std::forward< T >(value));
  }

  template< class T >
  void Queue< T >::pop() noexcept
  {
    list_.pop_front();
  }

  template< class T >
  void Queue< T >::swap(Queue< T >& other) noexcept
  {
    list_.swap(other.list_);
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    list_.clear();
  }
}

#endif
