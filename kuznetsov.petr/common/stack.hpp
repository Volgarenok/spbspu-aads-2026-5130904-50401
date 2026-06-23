#ifndef STACK_HPP
#define STACK_HPP
#include <list.hpp>

namespace kuznetsov {
  template< class T >
  class Stack {
  public:
    T& top();
    const T& top() const;

    bool empty() const noexcept;

    size_t size() const noexcept;

    void push(const T& val);
    void push(T&& val);

    void pop();

    void clear() noexcept;

  private:
    List< T > list_;
  };


}

template< class T >
T& kuznetsov::Stack< T >::top()
{
  return list_.front();
}

template< class T >
const T& kuznetsov::Stack< T >::top() const
{
  return list_.front();
}

template< class T >
bool kuznetsov::Stack< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t kuznetsov::Stack< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void kuznetsov::Stack< T >::push(const T& val)
{
  list_.insert(list_.cbegin(), val);
}

template< class T >
void kuznetsov::Stack< T >::push(T&& val)
{
  list_.insert(list_.cbegin(), std::move(val));
}

template< class T >
void kuznetsov::Stack< T >::pop()
{
  list_.popFront();
}

template< class T >
void kuznetsov::Stack< T >::clear() noexcept
{
  list_.clear();
}
#endif


