#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"

namespace sedov
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack & s) = default;
    Stack(Stack< T > && s) = default;
    ~Stack() = default;

    Stack & operator=(const Stack< T > & s) = default;
    Stack & operator=(Stack< T > && s) = default;

    T & top();
    const T & top() const;

    bool empty() const;
    size_t size() const;

    void push(const T & v);
    void push(T && v);

    template< class... Args >
    LIter< T > emplace(Args&&... args);

    void pop() noexcept;
    void swap(Stack & s) noexcept;
    void clear() noexcept;
  private:
    List< T > list_;
  };

  template< class T >
  T & Stack< T >::top()
  {
    return list_.back();
  }

  template< class T >
  const T & Stack< T >::top() const
  {
    return list_.back();
  }

  template< class T >
  bool Stack< T >::empty() const
  {
    return list_.size() == 0;
  }

  template< class T >
  size_t Stack< T >::size() const
  {
    return list_.size();
  }

  template< class T >
  void Stack< T >::push(const T & v)
  {
    list_.pushBack(v);
  }

  template< class T >
  void Stack< T >::push(T && v)
  {
    list_.pushBack(std::move(v));
  }

  template< class T >
  template< class... Args >
  LIter< T > Stack< T >::emplace(Args&&... args)
  {
    return list_.emplaceBack(std::forward< Args >(args)...);
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    list_.popBack();
  }

  template< class T >
  void Stack< T >::swap(Stack & s) noexcept
  {
    list_.swap(s.list_);
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    list_.clear();
  }
}

#endif
