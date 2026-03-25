#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "common/STRUCTS.hpp"
#include <stdexcept>
#include <cstddef>

namespace petrov
{
  template<typename T>
  class Queue {
  public:
    Queue();
    Queue(const Queue<T>& q);
    Queue(Queue<T>&& q);

    Queue<T>& operator=(const Queue<T>& q);
    Queue<T>& operator=(Queue<T>&& q);

    void push(const T& rhs);
    T drop();

    const T& front() const;
    void pop();
    bool empty() const;
    size_t size() const;
    void clear();

    ~Queue();
  private:
    List<T> list_;
  };

  template<typename T>
  Queue<T>::Queue():
    list_()
  {}

  template<typename T>
  Queue<T>::Queue(const Queue<T>& q):
    list_(q.list_)
  {}

  template<typename T>
  Queue<T>::Queue(Queue<T>&& q):
    list_(std::move(q.list_))
  {}
  
  template<typename T>
  Queue<T>& Queue<T>::operator=(const Queue<T>& q)
  {
    if (this != &q)
    {
      list_ = q.list_;
    }
    return *this;
  }

  template<typename T>
  Queue<T>& Queue<T>::operator=(Queue<T>&& q)
  {
    if (this != &q)
    {
      list_ = std::move(q.list_);
    }
    return *this;
  }

  template<typename T>
  void Queue<T>::push(const T& rhs)
  {
    list_.insert(list_.getLast(), rhs);
  }

  template<typename T>
  T Queue<T>::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    T res = front();
    pop();
    return res;
  }

  template<typename T>
  const T& Queue<T>::front() const
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    return *list_.begin();
  }

  template<typename T>
  void Queue<T>::pop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    list_.popStart();
  }

  template<typename T>
  bool Queue<T>::empty() const
  {
    return list_.size() == 0;
  }

  template<typename T>
  size_t Queue<T>::size() const
  {
    return list_.size();
  }

  template<typename T>
  void Queue<T>::clear()
  {
    list_.clear();
  }

  template<typename T>
  Queue<T>::~Queue()
  {
    clear();
  }
}
#endif
