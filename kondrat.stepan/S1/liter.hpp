#ifndef LITER_HPP
#define LITER_HPP

#include <cassert>
#include <memory>

namespace kondrat
{
  template< class T >
  class Node;

  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    public:
      LIter();
      LIter< T > & operator++();
      LIter< T > operator++(int);
      LIter< T > & operator--();
      LIter< T > operator--(int);
      bool operator==(const LIter< T > & rhs) const;
      bool operator!=(const LIter< T > & rhs) const;
      T & operator*() const;
      T * operator->() const;
    private:
      LIter(Node< T > * node) : node_(node) {}
      Node< T > * node_;
  };


  template< class T >
  LIter< T >::LIter() : node_(nullptr) {}

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LIter< T > & LIter< T >::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T > & rhs) const
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  T & LIter< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  T * LIter< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->val);
  }
}

#endif
