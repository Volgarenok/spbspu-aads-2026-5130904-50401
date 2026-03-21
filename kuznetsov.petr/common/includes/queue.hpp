#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"
namespace kuznetsov {
  template< class T >
  class Queue {
  public:
    Queue():
      list_()
    {}

    Queue(Queue< T >&& q) :
      list_(std::move(q))
    {}

    Queue& operator=(const Queue< T >& q)
    {
      this->list_ = q.list_;
      return *this;
    }

    Queue& operator=(Queue< T >&& q)
    {
      if (this == &q) {
        return *this;
      }
      this->list_ = std::move(q);
      return *this;
    }

    T& front()
    {
      return list_.front();
    }

    T& back()
    {
      return list_.back();
    }

    void push(const T& rhs)
    {
      list_.insert(list_.end(), rhs);
    }

    void pop()
    {
      list_.popFront();
    }

    bool empty()
    {
      return list_.empty();
    }
    size_t size()
    {
      return list_.size();
    }

  private:
    List< T > list_;
  };
}

#endif

