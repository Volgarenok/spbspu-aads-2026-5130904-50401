#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <list.hpp>
namespace kuznetsov {
  template< class T >
  class Queue {
  public:
    T& front();
    const T& front() const;

    void push(const T& val);
    void push(T&& val);

    void pop();

    bool empty() const noexcept;

    size_t size() const noexcept;

    void clear() noexcept;
  private:
    List< T > list_;
  };
}

template< class T >
T& kuznetsov::Queue< T >::front()
{
  return list_.front();
}

template< class T >
const T& kuznetsov::Queue< T >::front() const
{
  return list_.front();
}

template< class T >
void kuznetsov::Queue< T >::push(const T& val)
{
  list_.insert(list_.cend(), val);
}

template< class T >
void kuznetsov::Queue< T >::push(T&& val)
{
  list_.insert(list_.cend(),std::move(val));
}

template< class T >
void kuznetsov::Queue< T >::pop()
{
  list_.popFront();
}

template< class T >
bool kuznetsov::Queue< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t kuznetsov::Queue< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void kuznetsov::Queue< T >::clear() noexcept
{
  list_.clear();
}
#endif

