#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "queue.hpp"
#include <cstddef>

namespace petrov {
  template<class T>
  class Queue
  {
    public:
      Queue() = default;
      Queue(Queue&& a) = default;
      Queue(const Queue& a) = default;
      ~Queue() = default;

      void push(T rhs);
      void push(const T& a);
      void push(T&& a);

      void pop();
      T& front();
      T& back();
      bool empty();
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
      petrov::List<T> dat;
  };
}

template<class T>
void petrov::Queue<T>::push(T rhs)
{
  dat.push_back(rhs);
}

template<class T>
void petrov::Queue<T>::push(const T& a)
{
  dat.push_back(a);
}

template<class T>
void petrov::Queue<T>::push(T&& a)
{
  dat.push_back(std::move(a));
}

template<class T>
void petrov::Queue<T>::pop()
{
  dat.pop_front();
}

template<class T>
T& petrov::Queue<T>::front()
{
  if (dat.IsEmpty())
  {
    thorw std::logic_error("err");
  }
  return *dat.begin();
}

template<class T>
T& petrov::Queue<T>::back()
{
  if (dat.IsEmpty())
  {
    throw std::logic_error("err");
  }
  return *dat.end();
}

template<class T>
const T& petrov::Queue<T>::back() const
{
  if (dat.IsEmpty())
  {
    throw std::logic_error("err");
  }
  petrov::LIter<T> i = dat.end();
  return *i;
}

template<class T>
T petrov::Queue<T>::drop()
{
  T c = std::move(front());
  pop();
  return c;
}

template<class T>
bool petrov::Queue<T>::empty() const
{
  return dat.IsEmpty();
}

template<class T>
size_t petrov::Queue<T>::size() const
{
  return dat.getSize();
}

template<class T>
void petrov::Queue<T>::clear()
{
  dat.clear();
}

template<class T>
bool Queue<T>::operator==(const Queue& a) const
{
  if (size() != a.size())
  {
    return 0;
  }
  petrov::LIter<T> i1 = dat.begin();
  petrov::LIter<T> i2 = a.dat.begin();
  while (i1 != dat.end())
  {
    if (*i1 != *i2)
    {
      return 0;
    }
    ++i1; ++i2;
  }
  return 1;
}

template<class T>
bool Queue<T>::operator!=(const Queue& a) const
  return !(*this == a);
}

#endif
