#ifndef STACK
#define STACK
#include <list/list.hpp>
namespace malashenko
{
  template< class T >
  class Stack {
  public:
    Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) = default;

    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) = default;

    ~Stack() = default;

    T& top();
    const T& top() const;


    bool empty() const;
    size_t size() const;

    void push(const T& value);
    void push(T&& value);

    void pop();
    void swap(Stack& other);
    void clear();

  private:
    List< T > list_;
  };


  template< class T >
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }

  template< class T >
  size_t Stack< T >::size() const
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
    list_.push_back(value);
  }

  template< class T >
  void Stack< T >::pop()
  {
    list_.pop_back();
  }

  template< class T >
  const T& Stack< T >::top() const
  {
    return list_.back();
  }


  template< class T >
  T& Stack< T >::top()
  {
    return list_.back();
  }

  template< class T >
  void Stack< T >::swap(Stack< T >& other)
  {
    list_.swap(other.list_);
  }

  template< class T >
  void Stack< T >::clear()
  {
    list_.clear();
  }
}

#endif
