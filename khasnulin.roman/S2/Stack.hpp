#ifndef STACK_HPP
#define STACK_HPP

#include "List.hpp"
#include <cstddef>

namespace khasnulin
{
  template < class T > class Stack
  {
  public:
    Stack() = default;

    Stack(const Stack< T > &rhs) = default;
    Stack(Stack< T > &&rhs) noexcept = default;

    Stack< T > &operator=(const Stack< T > &rhs) = default;
    Stack< T > &operator=(Stack< T > &&rhs) noexcept = default;

    bool operator==(const Stack< T > &rhs) const noexcept;
    bool operator!=(const Stack< T > &rhs) const noexcept;

    ~Stack() = default;

    T &top();
    const T &top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T &elem);
    void push(T &&elem);

    template < class... Args > void emplace(Args &&...args);

    void pop();

    void swap(Stack< T > &rhs) noexcept;

  private:
    BiList< T > list_;
  };

  template < class T > bool Stack< T >::operator==(const Stack< T > &rhs) const noexcept
  {
    return list_ == rhs.list_;
  }
  template < class T > bool Stack< T >::operator!=(const Stack< T > &rhs) const noexcept
  {
    return !(list_ == rhs.list_);
  }

  template < class T > T &Stack< T >::top()
  {
    return list_.back();
  }

  template < class T > const T &Stack< T >::top() const
  {
    return static_cast< const BiList< T > & >(list_).back();
  }

  template < class T > bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template < class T > size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template < class T > void Stack< T >::push(const T &elem)
  {
    list_.push_back(elem);
  }

  template < class T > void Stack< T >::push(T &&elem)
  {
    list_.push_back(std::move(elem));
  }

  template < class T > void Stack< T >::pop()
  {
    list_.pop_back();
  }

  template < class T > template < class... Args > void Stack< T >::emplace(Args &&...args)
  {
    list_.emplace_back(std::forward< Args >(args)...);
  }

  template < class T > void Stack< T >::swap(Stack< T > &rhs) noexcept
  {
    list_.swap(rhs.list_);
  }
}

#endif
