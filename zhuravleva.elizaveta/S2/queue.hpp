#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <list.hpp>

namespace zhuravleva
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
    void clear() noexcept;
    size_t size() const noexcept;

    template< class... Args >
    void emplace(Args&&... args);

  private:
    List< T > data_;
  };
}

template< class T >
void zhuravleva::Queue< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< class T >
void zhuravleva::Queue< T >::push(T&& value)
{
  data_.pushBack(std::move(value));
}

template< class T >
void zhuravleva::Queue< T >::pop()
{
  if (data_.empty())
  {
    throw std::runtime_error("empty data error");
  }
  data_.popFront();
}

template< class T >
T& zhuravleva::Queue< T >::front()
{
  if (data_.empty())
  {
    throw std::runtime_error("error");
  }
  return (*data_.begin());
}

template< class T >
const T& zhuravleva::Queue< T >::front() const
{
  if (data_.empty())
  {
    throw std::runtime_error("empty queue");
  }
  return *data_.cbegin();
}

template< class T >
bool zhuravleva::Queue< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
void zhuravleva::Queue< T >::clear() noexcept
{
  data_.clear();
}

template< class T >
size_t zhuravleva::Queue< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
template< class... Args >
void zhuravleva::Queue< T >::emplace(Args&&... args)
{
  data_.emplaceBack(std::forward< Args >(args)...);
}

#endif
