#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace zharov
{
  template< class T >
  class Queue
  {
  public:
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    void push(const T& rhs);
    void push(T&& rhs);
    template< class... Args >
    void emplace(Args&&... args);
    void pop() noexcept;
    void swap(Queue& q) noexcept;

  private:
    List< T > list_;
  };
}

template< class T >
T& zharov::Queue< T >::front()
{
  return list_.front();
}

template< class T >
const T& zharov::Queue< T >::front() const
{
  return list_.front();
}

template< class T >
bool zharov::Queue< T >::empty() const noexcept
{
  return !list_.size();
}

template< class T >
size_t zharov::Queue< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void zharov::Queue< T >::clear() noexcept
{
  list_.clear();
}

template< class T >
void zharov::Queue< T >::push(const T& rhs)
{
  list_.pushBack(rhs);
}

template< class T >
void zharov::Queue< T >::push(T&& rhs)
{
  list_.pushBack(std::forward< T >(rhs));
}

template< class T >
template< class... Args >
void zharov::Queue< T >::emplace(Args&&... args)
{
  list_.emplaceBack(std::forward< Args >(args)...);
}

template< class T >
void zharov::Queue< T >::pop() noexcept
{
  list_.popFront();
}

template< class T >
void zharov::Queue< T >::swap(Queue& q) noexcept
{
  list_.swap(q.list_);
}

#endif
