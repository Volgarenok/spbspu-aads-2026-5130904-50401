#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <list.hpp>

namespace burukov
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue &other) = default;
    Queue(Queue &&other) noexcept = default;
    Queue &operator=(const Queue &other) = default;
    Queue &operator=(Queue &&other) noexcept = default;

    T &front();
    const T &front() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &value);
    void push(T &&value);

    template< class... Args >
    void emplace(Args &&... args);

    void pop() noexcept;

    void swap(Queue &other) noexcept;

  private:
    List< T > list_;
  };
}

template< class T >
T &burukov::Queue< T >::front()
{
  return list_.front();
}

template< class T >
const T &burukov::Queue< T >::front() const
{
  return list_.front();
}

template< class T >
bool burukov::Queue< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t burukov::Queue< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void burukov::Queue< T >::push(const T &value)
{
  list_.pushBack(value);
}

template< class T >
void burukov::Queue< T >::push(T &&value)
{
  list_.pushBack(std::forward< T >(value));
}

template< class T >
template< class... Args >
void burukov::Queue< T >::emplace(Args &&... args)
{
  list_.emplaceBack(std::forward< Args >(args)...);
}

template< class T >
void burukov::Queue< T >::pop() noexcept
{
  list_.popFront();
}

template< class T >
void burukov::Queue< T >::swap(Queue &other) noexcept
{
  list_.swap(other.list_);
}

#endif
