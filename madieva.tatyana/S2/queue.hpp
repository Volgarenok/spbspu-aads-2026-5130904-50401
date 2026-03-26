#ifndef STAC_HPP
#define STAC_HPP
#include "../common/list.hpp"
#include <stdexcept>
namespace madieva {
  template< class T >
  class Queue {
    List< T > data;
  public:
    void push(const T & val);
    T & front();
    const T & front() const;
    size_t size() const noexcept;
    void pop();
    void clear() noexcept;
    bool empty() const noexcept;
  };

  template< class T >
  void Queue< T >::push(const T & val)
  {
    data.push_back(val);
  }

  template< class T >
  T & Queue< T >::front()
  {
    if (!empty()) {
      LIter< T > it = data.begin();
      return (*it);
    } else {
      throw std::runtime_error("Queue is empty");
    }
  }

  template< class T >
  const T & Queue< T >::front() const
  {
    if (!empty()) {
      LCIter< T > it = data.begin();
      return (*it);
    } else {
      throw std::runtime_error("Queue is empty");
    }
  }

  template< class T >
  size_t Queue< T >::size() const noexcept
  {
    return data.size();
  }

  template< class T >
  void Queue< T >::pop()
  {
    data.pop_front();
  }

  template< class T >
  void Queue< T >::clear() noexcept
  {
    data.clear();
  }

  template< class T >
  bool Queue< T >::empty() const noexcept
  {
    return data.size() == 0;
  }
}

#endif