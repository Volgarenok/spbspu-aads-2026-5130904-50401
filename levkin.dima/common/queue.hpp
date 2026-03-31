#ifndef MY_QUEUE
#define MY_QUEUE
#include "list.hpp"
namespace levkin {
  template < typename T > class Queue
  {

  private:
    levkin::List< T > list_;

  public:
    Queue() : list_(List< T >{}) {}
    void push(const T& v);
    T drop();
    size_t size() const;
    void pop();

    T& front();
    const T& front() const;
    bool empty() const;
  };

  template < class T > bool levkin::Queue< T >::empty() const
  {
    return size() == 0;
  }

  template < class T > T levkin::Queue< T >::drop()
  {
    T val = std::move(front());
    list_.popFront();
    return val;
  }
  template < class T > void levkin::Queue< T >::pop() { list_.popFront(); }

  template < class T > const T& levkin::Queue< T >::front() const
  {
    return *list_.cbegin();
  }

  template < class T > T& levkin::Queue< T >::front() { return *list_.begin(); }


  template < class T > size_t levkin::Queue< T >::size() const
  {
    return list_.size();
  }

  template < class T > void levkin::Queue< T >::push(const T& v)
  {
    list_.pushBack(v);
  }

}

#endif
