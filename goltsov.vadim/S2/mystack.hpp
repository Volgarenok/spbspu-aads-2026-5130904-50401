#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "../Common/mylist.hpp"

namespace goltsov
{
  template< class T >
  class Stack
  {
    List< T > dates_;
    size_t size_;
  public:
    Stack();
    ~Stack() = default;
    Stack(const Stack< T >& other);
    Stack(Stack< T >&& other);
    Stack< T >& operator=(const Stack< T >& other);
    Stack< T >& operator=(Stack< T >&& other);

    void push(const T& rhs);
    void push(T&& rhs);
    void drop();
    T& front();
    const T& front() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  };
}

namespace goltsov
{
  template< class T >
  Stack< T >::Stack():
    size_(0)
  {}
  template< class T >
  Stack< T >::Stack(const Stack< T >& other):
    dates_(other.dates_),
    size_(other.size_)
  {}
  template< class T >
  Stack< T >::Stack(Stack< T >&& other):
    dates_(std::move(other.dates_)),
    size_(other.size_)
  {}
  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    dates_ = other.dates_;
    size_ = other.size_;
    return * this;
  }
  template< class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other)
  {
    dates_ = std::move(other.dates_);
    size_ = other.size_;
    return * this;
  }

  template< class T >
  void Stack< T >::push(const T& rhs)
  {
    dates_.push_start(rhs);
    size_ += 1;
  }
  template< class T >
  void Stack< T >::push(T&& rhs)
  {
    dates_.push_start(std::move(rhs));
    size_ += 1;
  }
  template< class T >
  void Stack< T >::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    dates_.pop_start();
    size_--;
  }
  template< class T >
  T& Stack< T >::front()
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return (* dates_.begin());
  }
  template< class T >
  const T& Stack< T >::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Stack is empty");
    }
    return (* dates_.begin());
  }
  template< class T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< class T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }
  template< class T >
  void Stack< T >::clear()
  {
    dates_.clear();
    size_ = 0;
  }
}
#endif
