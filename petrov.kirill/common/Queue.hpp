#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>
#include <stdexcept>
#include "list.hpp"

namespace petrov {
  template <class T>
  class Queue
  {
    public:
      Queue() = default;
      Queue(Queue&& a) = default;
      Queue(const Queue& a) = default;
      ~Queue() = default;

      void push(const T& a);
      void push(T&& a);

      void pop();
      T& front();
      T& back();
      size_t size();
      T drop();
      Queue& operator=(const Queue& a);
      Queue& operator=(Queue&& a);
      const T& front() const;
      const T& back() const;
      bool empty() const;
      size_t size() const;
      void clear();
      bool operator==(const Queue& a) const;
      bool operator!=(const Queue& a) const;
    private:
      petrov::List<T> dat_;
  };
}

template <class T>
void petrov::Queue<T>::push(const T& a)
{
  dat_.push_back(a);
}

template <class T>
void petrov::Queue<T>::push(T&& a)
{
  dat_.push_back(std::move(a));
}

template <class T>
void petrov::Queue<T>::pop()
{
  dat_.pop_front();
}

template <class T>
T& petrov::Queue<T>::front()
{
  if (dat_.IsEmpty())
  {
    throw std::logic_error("err");
  }
  return *dat_.begin();
}

template <class T>
T& petrov::Queue<T>::back()
{
  if (dat_.IsEmpty())
  {
    throw std::logic_error("err");
  }
  return *dat_.end();
}

template <class T>
const T& petrov::Queue<T>::back() const
{
  if (dat_.IsEmpty())
  {
    throw std::logic_error("err");
  }
  petrov::LIter<T> i = dat_.end();
  return *i;
}

template <class T>
T petrov::Queue<T>::drop()
{
  T c = std::move(front());
  pop();
  return c;
}

template <class T>
bool petrov::Queue<T>::empty() const
{
  return dat_.IsEmpty();
}

template <class T>
size_t petrov::Queue<T>::size() const
{
  return dat_.getSize();
}

template <class T>
void petrov::Queue<T>::clear()
{
  dat_.clear();
}

template <class T>
bool petrov::Queue<T>::operator==(const Queue& a) const
{
  if (size() != a.size())
  {
    return 0;
  }
  petrov::LCIter<T> i1 = dat_.begin();
  petrov::LCIter<T> i2 = a.dat_.begin();
  while (i1 != dat_.end())
  {
    if (*i1 != *i2)
    {
      return 0;
    }
    ++i1;
    ++i2;
  }
  return 1;
}

template <class T>
bool petrov::Queue<T>::operator!=(const Queue& a) const
{
  return !(*this == a);
}

#endif
