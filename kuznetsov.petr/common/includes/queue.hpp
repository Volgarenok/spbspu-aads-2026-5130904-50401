#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"
namespace kuznetsov {
  template< class T >
  class Queue {
  public:
    T& front()
    {
      return list_.front();
    }
    const T& front() const
    {
      return list_.front();
    }

    T& back()
    {
      return list_.back();
    }

    const T& back() const
    {
      return list_.back();
    }

    void push(const T& val)
    {
      list_.insert(list_.end(), val);
    }

    void pop()
    {
      list_.popFront();
    }

    bool empty() const
    {
      return list_.empty();
    }

    size_t size() const
    {
      return list_.size();
    }

    void clear() noexcept
    {
      list_.clear();
    }

  private:
    List< T > list_;
  };
}

#endif

