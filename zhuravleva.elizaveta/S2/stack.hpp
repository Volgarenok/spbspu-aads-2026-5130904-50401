#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <list.hpp>

namespace zhuravleva
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
    void clear() noexcept;
    size_t size() const noexcept;

    template< class... Args >
    void emplace(Args&&... args);

  private:
    List< T > data_;
  };
}

template< class T >
void zhuravleva::Stack< T >::push(const T& value)
{
  data_.emplaceFront(value);
}

template< class T >
void zhuravleva::Stack< T >::push(T&& value)
{
  data_.emplaceFront(std::forward< T >(value));
}

template< class T >
void zhuravleva::Stack< T >::pop()
{
  if (data_.empty())
  {
    throw std::runtime_error("empty data error");
  }
  data_.popFront();
}

template< class T >
T& zhuravleva::Stack< T >::top()
{
  if (data_.empty())
  {
    throw std::runtime_error("empty data error");
  }
  return *data_.begin();
}

template< class T >
const T& zhuravleva::Stack< T >::top() const
{
  if (data_.empty())
  {
    throw std::runtime_error("empty stack");
  }
  return *data_.cbegin();
}


template< class T >
bool zhuravleva::Stack< T >::empty() const noexcept
{
  return data_.empty();
}

template< class T >
void zhuravleva::Stack< T >::clear() noexcept
{
  data_.clear();
}

template< class T >
size_t zhuravleva::Stack< T >::size() const noexcept
{
  return data_.size();
}

template< class T >
template< class... Args >
void zhuravleva::Stack< T >::emplace(Args&&... args)
{
  data_.emplaceFront(std::forward< Args >(args)...);
}

#endif
