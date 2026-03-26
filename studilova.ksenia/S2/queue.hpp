#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>

namespace studilova
{
  template< class T >
  class Queue
  {
    public:
      void push(const T& value);
      void pop();
      T& front();

      bool empty() const;
      size_t size() const;

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
bool studilova::Queue< T >::empty() const
{
  return data_.empty();
}

template< class T >
size_t studilova::Queue< T >::size() const
{
  return data_.size();
}

#endif
