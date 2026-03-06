#ifndef ITERATOR
#define ITERATOR
#include "list.hpp"
#include "node.hpp"
#include <memory>
#include <cassert>

namespace malashenko {
  template< class T > class List;
  template< class T > class Node;

  template< class T >
  class LIter {
  public:
    LIter();
    LIter< T >& operator++();
    LIter< T > operator++(int);
    LIter< T >& operator+(size_t s);


    LIter< T >& operator--();
    LIter< T > operator--(int);
    LIter< T >& operator-(size_t s);

    T& operator*();
    T* operator->();

    bool operator!=(const LIter< T >& other) const;
    bool operator==(const LIter< T >& other) const;
  private:
    friend class List< T >;
    LIter(Node< T >* other);
    Node< T >* node_;
  };

  template< class T >
  LIter< T >::LIter():
    node_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(Node< T >* other):
    node_(other)
  {}

  template< class T >
  LIter< T >& LIter< T >::operator++()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LIter< T > tmp{*this};
    ++(*this);
    return tmp;
  }

  template< class T >
  LIter< T >& LIter< T >::operator+(size_t s)
  {
    for (size_t i = 0; i < s; ++i)
    {
      ++node_;
    }
  }


  template< class T >
  LIter< T >& LIter< T >::operator--()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LIter< T > tmp{*this};
    --(*this);
    return tmp;
  }


  template< class T >
  LIter< T >& LIter< T >::operator-(size_t s)
  {
    for (size_t i = 0; i < s; ++i)
    {
      --node_;
    }
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& other) const
  {
    return node_ == other.node_;
  }
  
  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& other) const
  {
    return !(other == *this);
  }

  template< class T >
  T& LIter< T >::operator*()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return node_ ->value_;
  }

  template< class T >
  T* LIter< T >::operator->()
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return std::addressof(node_->value_);
  }
}

#endif
