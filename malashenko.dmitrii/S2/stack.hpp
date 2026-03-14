#ifndef STACK
#define STACK
#include <list/list.hpp>
namespace malashenko
{
  template< class T >
  class Stack {
  public:
    Stack();
    Stack(const Stack& other);
    Stack(Stack&& other);

    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other);
  
    T& top() const;

    bool empty() const;
    size_t size() const;

    void push(const T& value);
    void pop();
    void swap(Stack& other);
    void clear();

    void show() const;

    ~Stack();
  private:
    List< T > list_;
  };

  template< class T >
  Stack< T >::Stack():
    list_()
  {}

  template< class T >
  Stack< T >::Stack(const Stack< T >& other):
    list_(other.list_)
  {}

  template< class T >
  Stack< T >::Stack(Stack< T >&& other):
    list_(std::move(other.list_))
  {}

  template< class T >
  Stack< T >& Stack< T >::operator=(const Stack< T >& other)
  {
    Stack< T > temp(other);
    swap(temp);
    return *this;
  }

  template< class T >
  Stack< T >& Stack< T >::operator=(Stack< T >&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    list_ = std::move(other.list_);
    return *this;
  }

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
  void Stack< T >::pop()
  {
    list_.pop_back();
  }
  

  template< class T >
  void Stack< T >::show() const
  {
    for (LIter< T > it = list_.begin(); it != list_.end(); ++it)
    {
      std::cout << *it << ' ';
    }
    std::cout << '\n';
  }

  template< class T >
  T& Stack< T >::top() const
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

  template< class T >
  Stack< T >::~Stack()
  {
    list_.clear();
  }
}

#endif