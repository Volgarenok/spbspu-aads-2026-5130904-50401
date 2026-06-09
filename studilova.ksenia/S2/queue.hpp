#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

#include <list.hpp>

namespace studilova
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
      void clear() noexcept;

      template< class... Args >
      void emplace(Args&&... args);

    private:
      List< T > data_;
  };
}

template< class T >
void studilova::Queue< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< class T >
void studilova::Queue< T >::push(T&& value)
{
  data_.pushBack(std::forward< T >(value));
}

template< class T >
void studilova::Queue< T >::pop()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  data_.popFront();
}

template< class T >
T& studilova::Queue< T >::front()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return data_.front();
}

template< class T >
const T& studilova::Queue< T >::front() const
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return data_.front();
}

template< class T >
bool studilova::Queue< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
size_t studilova::Queue< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
void studilova::Queue< T >::clear() noexcept
{
  data_.clear();
}

template< class T >
template< class... Args >
void studilova::Queue< T >::emplace(Args&&... args)
{
  data_.emplaceBack(std::forward< Args >(args)...);
}

#endif
