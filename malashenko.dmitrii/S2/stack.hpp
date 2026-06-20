#ifndef STACK_HPP
#define STACK_HPP
#include <list/list.hpp>
namespace malashenko
{
  template< class T >
  class Stack {
  public:
    T& top() noexcept;
    const T& top() const noexcept;


    bool empty() const noexcept;
    size_t size() const noexcept;

    void push(const T& value);
    void push(T&& value);

    void pop() noexcept;
    void swap(Stack& other) noexcept;
    void clear() noexcept;

  private:
    List< T > list_;
  };


  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return list_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return list_.size();
  }

  template< class T >
  void Stack< T >::push(const T& value)
  {
    list_.push_back(value);
  }

  template< class T >
  void Stack< T >::push(T&& value)
  {
    list_.push_back(std::forward< T >(value));
  }

  template< class T >
  void Stack< T >::pop() noexcept
  {
    list_.pop_back();
  }

  template< class T >
  const T& Stack< T >::top() const noexcept
  {
    return list_.back();
  }


  template< class T >
  T& Stack< T >::top() noexcept
  {
    return list_.back();
  }

  template< class T >
  void Stack< T >::swap(Stack< T >& other) noexcept
  {
    list_.swap(other.list_);
  }

  template< class T >
  void Stack< T >::clear() noexcept
  {
    list_.clear();
  }
}

#endif
