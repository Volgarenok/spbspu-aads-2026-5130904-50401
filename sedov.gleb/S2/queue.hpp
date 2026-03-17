#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <S1/list.hpp>

namespace sedov
{
  template< class T >
  class Queue
  {
    List< T > list_;
  public:
    Queue():
      list_()
    {}

    Queue(const Queue< T > & q):
      list_(q.list_)
    {}

    Queue(Queue< T > && q):
      list_(std::move(q.list_))
    {}

    ~Queue() = default;

    Queue & operator=(const Queue< T > & q)
    {
      Queue< T > temp(q);
      swap(temp);
      return *this;
    }

    Queue & operator=(Queue< T > && q)
    {
      if (this == &q)
      {
        return *this;
      }
      list_ = std::move(q.list_);
      return *this;
    }

    T & front() const
    {
      return list_.front();
    }

    T & back() const
    {
      return list_.back();
    }

    bool empty() const
    {
      return list_.size() == 0;
    }

    size_t size() const
    {
      return list_.size();
    }

    void push(const T& v)
    {
      list_.pushBack(v);
    }

    void pop()
    {
      list_.popFront();
    }

    void swap(Queue< T > & q)
    {
      list_.swap(q.list_);
    }

    void clear()
    {
      list_.clear();
    }
  };
}
#endif
