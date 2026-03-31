#ifndef MY_STACK
#define MY_STACK
#include "list.hpp"
namespace levkin {
  template < typename T > class Stack
  {

  private:
    levkin::List< T > list_;

  public:
    Stack() : list_(List< T >{}) {}
    void push(const T& v);
    T drop();
    size_t size() const;
    void pop();

    T& front();
    const T& front() const;
    bool empty() const;
  };

  template < class T > bool levkin::Stack< T >::empty() const
  {
    return size() == 0;
  }

  template < class T > T levkin::Stack< T >::drop()
  {
    T val = std::move(front());
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Stack< T >::pop() { list_.popFront(); }

  template < class T > const T& levkin::Stack< T >::front() const
  {
    return *list_.cbegin();
  }

  template < class T > T& levkin::Stack< T >::front() { return *list_.begin(); }

  template < class T > size_t levkin::Stack< T >::size() const
  {
    return list_.size();
  }

  template < class T > void levkin::Stack< T >::push(const T& v)
  {
    list_.pushFront(v);
  }

}

#endif
