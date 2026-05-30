#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace studilova
{
  template< class T >
  class Stack
  {
    public:
      void push(const T& value);
      void push(T&& value);

      void pop();

      T& top();
      const T& top() const;

      bool empty() const noexcept;
      size_t size() const noexcept;
      void clear() noexcept;

    private:
      List< T > data_;
  };
}

template< class T >
void studilova::Stack< T >::push(const T& value)
{
  data_.pushBack(value);
}

template< class T >
void studilova::Stack< T >::push(T&& value)
{
  data_.pushBack(std::move(value));
}

template< class T >
void studilova::Stack< T >::pop()
{
  if(empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  data_.popBack();
}

template< class T >
T& studilova::Stack< T >::top()
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  return data_.back();
}

template< class T >
const T& studilova::Stack< T >::top() const
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  return data_.back();
}

template< class T >
bool studilova::Stack< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
size_t studilova::Stack< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
void studilova::Stack< T >::clear() noexcept
{
  data_.clear();
}

#endif
