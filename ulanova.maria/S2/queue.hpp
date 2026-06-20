#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace ulanova
{
  template <class T>
  class Queue
  {
  private:
    List<T> list;
  public:
    Queue() = default;

    void push(const T& value);
    void clear();
    T drop();
    bool empty() const;
  };

  template <class T>
  void Queue<T>::push(const T& value)
  {
    list.push_back(value);
  }
  template <class T>
  void Queue<T>:: clear()
  {
    while (!empty())
    {
      drop();
    }
  }
  template <class T>
  T Queue<T>::drop()
  {
    if (empty())
    {
      throw std::runtime_error("Queue is empty");
    }
    T value = list.front();
    list.pop_front();
    return value;
  }
  template <class T>
  bool Queue<T>::empty() const
  {
    return list.empty();
  }
}
#endif
