#ifndef MY_STACK
#define MY_STACK
#include "list.hpp"
#include <utility>  
#include <stdexcept>
namespace levkin {
  template < typename T > class Stack
  {

  private:
    List< T > list_;

  public:
    Stack() = default;
    void push(const T& v);
    void push(T&& v);
    T drop();
    size_t size() const;
    void pop();

    T& top();
    const T& top() const;
    bool empty() const;
  };

  template < class T > bool Stack< T >::empty() const
  {
    return size() == 0;
  }

  template < class T > void Stack< T >::push(T&& v)
  {
    list_.pushFront(std::move(v));
  }

  template < class T > T Stack< T >::drop()
  {
    if (empty()) {
      throw std::out_of_range("stack is empty!");
    }
    T val = std::move(top());
    list_.popFront();
    return val;
  }
  template < class T > void Stack< T >::pop()
  {
    if (!empty()) {
      list_.popFront();
    }
  }

  template < class T > const T& Stack< T >::top() const
  {
    if (empty()) {
      throw std::out_of_range("stack is empty!");
    }
    return *list_.cbegin();
  }

  template < class T > T& Stack< T >::top()
  {
    if (empty()) {
      throw std::out_of_range("stack is empty!");
    }
    return *list_.begin();
  }

  template < class T > size_t Stack< T >::size() const
  {
    return list_.size();
  }

  template < class T > void Stack< T >::push(const T& v)
  {
    list_.pushFront(v);
  }

}

#endif
