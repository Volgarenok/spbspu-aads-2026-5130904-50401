#ifndef CONSTITERATOR
#define CONSTITERATOR
#include "list.hpp"
#include "node.hpp"
#include <memory>
#include <cassert>

namespace malashenko {
  template< class T > class List;
  template< class T > class Node;

  template< class T >
  class LCIter {
  public:
    LCIter();
    LCIter< T >& operator++();
    LCIter< T > operator++(int);
    LCIter< T >& LCIter< T >::operator+(size_t s);


    LCIter< T >& operator--();
    LCIter< T > operator--(int);
    LCIter< T >& LCIter< T >::operator-(size_t s);


    const T& operator*();
    const T* operator->();

    bool operator!=(const LCIter< T >& other) const;
    bool operator==(const LCIter< T >& other) const;
  private:
    Node< T >* node_;
    LCIter(Node< T >* other);
    friend class List< T >;
  };

  template< class T >
  LCIter< T >::LCIter():
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(Node< T >* other):
    node_(other)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LCIter< T > tmp{*this};
    ++(*this);
    return tmp;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator+(size_t s)
  {
    for (size_t i = 0; i < s; ++i)
    {
      ++node_;
    }
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator--()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LCIter< T > tmp{*this};
    --(*this);
    return tmp;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator-(size_t s)
  {
    for (size_t i = 0; i < s; ++i)
    {
      --node_;
    }
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& other) const
  {
    return node_ == other.node_;
  }
  
  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& other) const
  {
    return !(other == *this);
  }

  template< class T >
  const T& LCIter< T >::operator*()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return node_ ->value_;
  }

  template< class T >
  const T* LCIter< T >::operator->()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return std::addressof(node_->value_);
  }
}

#endif