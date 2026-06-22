#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace zharov
{
  template< class T >
  class Stack
  {
  public:
    T& top();
    const T& top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear() noexcept;
    void push(const T& rhs);
    void push(T&& rhs);
    template< class... Args >
    void emplace(Args&&... args);
    void pop() noexcept;
    void swap(Stack& s) noexcept;

  private:
    List< T > list_;
  };
}

template< class T >
T& zharov::Stack< T >::top()
{
  return list_.back();
}

template< class T >
const T& zharov::Stack< T >::top() const
{
  return list_.back();
}

template< class T >
bool zharov::Stack< T >::empty() const noexcept
{
  return !list_.size();
}

template< class T >
size_t zharov::Stack< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void zharov::Stack< T >::clear() noexcept
{
  list_.clear();
}

template< class T >
void zharov::Stack< T >::push(const T& rhs)
{
  list_.pushBack(rhs);
}

template< class T >
void zharov::Stack< T >::push(T&& rhs)
{
  list_.pushBack(std::forward< T >(rhs));
}

template< class T >
template< class... Args >
void zharov::Stack< T >::emplace(Args&&... args)
{
  list_.emplaceBack(std::forward< Args >(args)...);
}

template< class T >
void zharov::Stack< T >::pop() noexcept
{
  list_.popBack();
}

template< class T >
void zharov::Stack< T >::swap(Stack& s) noexcept
{
  list_.swap(s.list_);
}

#endif
