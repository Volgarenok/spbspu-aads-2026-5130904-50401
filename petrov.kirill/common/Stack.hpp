#ifndef STACK_HPP
#define STACK_HPP
#include "Queue.hpp"

namespace petrov
{
  template<class T>
  class Stack
  {
    public:
      Stack() = default;
      Stack(Stack&& a) = default;
      Stack(const Stack& a) = default;
      ~Stack();

      void push(T rhs);
      void push(T&& rhs);
      void pop();
      T drop();

      T top();
      const T& top() const;

      bool empty() const;
      size_t size() const;
      void clear();

      Stack& operator=(const Stack& a);
      Stack& operator=(Stack&& a);
      bool operator==(const Stack& a) const;
      bool operator!=(const Stack& a) const;

    private:
      petrov::List<T> dat;
  };

  template<class T>
  void Stack<T>::push(const T& val)
  {
    dat.push_front(val);
  }

  template<class T>
  void Stack<T>::push(T&& val)
  {
    dat.push_front(std::move(val));
  }

  template<class T>
  void Stack<T>::pop()
  {
    if (dat.IsEmpty())
    {
      throw std::logic_error("err");
    }
    dat.pop_front();
  }

  template<class T>
  T Stack<T>::drop()
  {
    T a = std::move(top());
    pop();
    return a;
  }

  template<class T>
  T& Stack<T>::top()
  {
    if (dat.IsEmpty())
    {
      throw std::logic_error("err");
    }
    return *dat.begin();
  }

  template<class T>
  const T& Stack<T>::top() const
  {
    if (dat.IsEmpty())
    {
      throw std::logic_error("err");
    }
    return *dat.begin();
  }

  template<class T>
  bool Stack<T>::empty() const
  {
    return dat.IsEmpty();
  }

  template<class T>
  size_t Stack<T>::size() const
  {
    size_t c = 0;
    for (petrov::LIter<T> i = dat.begin(); i != dat.end(); ++i)
    {
      c++;
    }
    return c;
  }

  template<class T>
  void Stack<T>::clear()
  {
    dat.clear();
  }

  template<class T>
  Stack<T>& Stack<T>::operator=(const Stack& a)
  {
    if (this != &a)
    {
      dat = a.dat;
    }
    return *this;
  }

  template<class T>
  Stack<T>& Stack<T>::operator=(Stack&& a)
  {
    if (this != &a)
    {
      dat = std::move(a.dat);
    }
    return *this;
  }

  template<class T>
  bool Stack<T>::operator==(const Stack& a) const
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
  bool Stack<T>::operator!=(const Stack& a) const
  {
    return !(*this == a);
  }
}

#endif
