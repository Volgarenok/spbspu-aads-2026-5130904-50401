#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include <utility>
#include "mylist.hpp"

namespace goltsov
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack< T >& other) = default;
    Stack(Stack< T >&& other) = default;
    ~Stack() = default;
    Stack< T >& operator=(const Stack< T >& other) = default;
    Stack< T >& operator=(Stack< T >&& other) = default;

    template< class ValRef >
    void push(ValRef&&);

    void pop();
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
  private:
    List< T > dates_;
  };
}

template< class T >
template< class ValRef >
void goltsov::Stack< T >::push(ValRef&& rhs)
{
  dates_.push_start(std::forward< ValRef >(rhs));
}
template< class T >
void goltsov::Stack< T >::pop()
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  dates_.pop_start();
}
template< class T >
T& goltsov::Stack< T >::top()
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  return (*dates_.begin());
}
template< class T >
const T& goltsov::Stack< T >::top() const
{
  if (empty())
  {
    throw std::runtime_error("Stack is empty");
  }
  return (*dates_.begin());
}
template< class T >
bool goltsov::Stack< T >::empty() const noexcept
{
  return dates_.empty();
}
template< class T >
size_t goltsov::Stack< T >::size() const noexcept
{
  return dates_.size();
}
template< class T >
void goltsov::Stack< T >::clear() noexcept
{
  dates_.clear();
}

#endif
