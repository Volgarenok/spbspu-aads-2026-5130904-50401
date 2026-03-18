#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "node.hpp"

namespace vasyakin
{
  template < class T > class List;
  template< class T >
  class LIter
  {
  private:
    friend class List< T >;
    Node< T >* ptr;
    explicit LIter(Node< T >* p);

  public:
    T& operator*() const;
    T* operator->() const;
    LIter& operator++();
    LIter operator++(int);
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
    Node< T >* get_ptr() const;
  };

  template< class T >
  class LCIter
  {
  private:
    friend class List< T >;
    const Node< T >* ptr;
    explicit LCIter(const Node< T >* p);
    explicit LCIter(const LIter< T >& it);

  public:
    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++();
    LCIter operator++(int);
    bool operator==(const LCIter& other) const;
    bool operator!=(const LCIter& other) const;
    const Node< T >* get_ptr() const;
  };

  template< class T >
  LIter< T >::LIter(Node< T >* p):
    ptr(p)
  {}

  template< class T >
  T& LIter< T >::operator*() const
  {
    return ptr->val;
  }

  template< class T >
  T* LIter< T >::operator->() const
  {
    return &(ptr->val);
  }

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const
  {
    return ptr != other.ptr;
  }

  template< class T >
  Node< T >* LIter< T >::get_ptr() const
  {
    return ptr;
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T >* p):
    ptr(p)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& it):
    ptr(it.ptr)
  {}

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    return ptr->val;
  }

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    return &(ptr->val);
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const
  {
    return ptr == other.ptr;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const
  {
    return ptr != other.ptr;
  }

  template< class T >
  const Node< T >* LCIter< T >::get_ptr() const
  {
    return ptr;
  }
}

#endif