#ifndef LCITER_HPP
#define LCITER_HPP

#include <cassert>
#include <memory>

namespace kondrat
{
  template< class T >
  class Node;

  template< class T >
  class List;

  template< class T >
  class LCIter
  {
    friend class List;
    public:
      LCIter();
      LCIter< T > & operator++();
      LCIter< T > operator++(int);
      LCIter< T > & operator--();
      LCIter< T > operator--(int);
      bool operator==(const LCIter< T > & rhs) const;
      bool operator!=(const LCIter< T > & rhs) const;
      T & operator*() const;
      T * operator->() const;
    private:
      LCIter(Node< T > * node) : node_(node) {}
      Node< T > * node_;
  };


  template< class T >
  LCIter< T >::LCIter() : node_(nullptr) {}

  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & rhs) const
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  T & LCIter< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  T * LCIter< T >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->val);
  }
}

#endif