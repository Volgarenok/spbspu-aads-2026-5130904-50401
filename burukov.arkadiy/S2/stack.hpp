#ifndef STACK_HPP
#define STACK_HPP

#include <list.hpp>

namespace burukov
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack &other) = default;
    Stack(Stack &&other) noexcept = default;
    Stack &operator=(const Stack &other) = default;
    Stack &operator=(Stack &&other) noexcept = default;

    T &top();
    const T &top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &value);
    void push(T &&value);

    template< class... Args >
    void emplace(Args &&... args);

    void pop() noexcept;

    void swap(Stack &other) noexcept;

  private:
    List< T > list_;
  };
}

template< class T >
T &burukov::Stack< T >::top()
{
  return list_.front();
}

template< class T >
const T &burukov::Stack< T >::top() const
{
  return list_.front();
}

template< class T >
bool burukov::Stack< T >::empty() const noexcept
{
  return list_.empty();
}

template< class T >
size_t burukov::Stack< T >::size() const noexcept
{
  return list_.size();
}

template< class T >
void burukov::Stack< T >::push(const T &value)
{
  list_.pushFront(value);
}

template< class T >
void burukov::Stack< T >::push(T &&value)
{
  list_.pushFront(std::forward< T >(value));
}

template< class T >
template< class... Args >
void burukov::Stack< T >::emplace(Args &&... args)
{
  list_.emplaceFront(std::forward< Args >(args)...);
}

template< class T >
void burukov::Stack< T >::pop() noexcept
{
  list_.popFront();
}

template< class T >
void burukov::Stack< T >::swap(Stack &other) noexcept
{
  list_.swap(other.list_);
}

#endif
