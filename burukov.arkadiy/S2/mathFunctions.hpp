#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"

namespace burukov
{
  template< class T >
  class Queue
  {
  public:
    Queue():
      list_(),
      tail_()
    {}

    Queue(const Queue &other):
      list_(other.list_),
      tail_()
    {
      findTail();
    }

    Queue(Queue &&other):
      list_(std::move(other.list_)),
      tail_(other.tail_)
    {
      other.tail_ = LIter< T >();
    }

    Queue &operator=(const Queue &other)
    {
      if (this != &other)
      {
        Queue temp(other);
        swap(temp);
      }
      return *this;
    }

    Queue &operator=(Queue &&other)
    {
      if (this != &other)
      {
        Queue temp(std::move(other));
        swap(temp);
      }
      return *this;
    }

    T &front()
    {
      return list_.front();
    }

    const T &front() const
    {
      return list_.front();
    }

    bool empty() const
    {
      return list_.size() == 0;
    }

    size_t size() const
    {
      return list_.size();
    }

    void push(const T &value)
    {
      if (list_.empty())
      {
        list_.pushFront(value);
        tail_ = list_.begin();
      }
      else
      {
        tail_ = list_.insertAfter(tail_, value);
      }
    }

    void pop()
    {
      if (list_.size() == 1)
      {
        tail_ = LIter< T >();
      }
      list_.popFront();
    }

    void swap(Queue &other)
    {
      list_.swap(other.list_);
      const LIter< T > tmp = tail_;
      tail_ = other.tail_;
      other.tail_ = tmp;
    }

    void clear()
    {
      list_.clear();
      tail_ = LIter< T >();
    }

  private:
    List< T > list_;
    LIter< T > tail_;

    void findTail()
    {
      tail_ = LIter< T >();
      if (list_.empty())
      {
        return;
      }
      LIter< T > it = list_.begin();
      LIter< T > next = it;
      ++next;
      while (next != list_.end())
      {
        it = next;
        ++next;
      }
      tail_ = it;
    }
  };
}

#endif
