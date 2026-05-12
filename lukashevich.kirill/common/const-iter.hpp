#ifndef CONST_ITER_HPP
#define CONST_ITER_HPP

#include <cassert>

namespace lukashevich
{
  template< class T >
  class Node;

  template< class T >
  class List;

  template< class T >
  class LCIter
  {
    friend class List< T >;
    public:
      LCIter();
      LCIter< T >& operator++();
      LCIter< T > operator++(int);
      LCIter< T >& operator--();
      LCIter< T > operator--(int);

      bool operator==(const LCIter< T >& rhs) const;
      bool operator!=(const LCIter< T >& rhs) const;

      const T& operator*() const;
      const T* operator->() const;

    private:
      explicit LCIter(Node< T >* node):
        node_(node)
      {}
      Node< T >* node_;
  };

  template< class T >
  LCIter< T >::LCIter():
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++()
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
  bool LCIter< T >::operator==(const LCIter< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& rhs) const
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    assert(node_ != nullptr);
    return &node_->val;
  }
}

#endif
