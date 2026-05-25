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
    public:
      LCIter() noexcept;
      LCIter< T >& operator++() noexcept;
      LCIter< T > operator++(int) noexcept;
      LCIter< T >& operator--() noexcept;
      LCIter< T > operator--(int) noexcept;

      bool operator==(const LCIter< T >& rhs) const noexcept;
      bool operator!=(const LCIter< T >& rhs) const noexcept;

      const T& operator*() const noexcept;
      const T* operator->() const noexcept;

    private:
      explicit LCIter(Node< T >* node) noexcept;
      Node< T >* node_;
      friend class List< T >;
  };

  template< class T >
  LCIter< T >::LCIter(Node< T >* node) noexcept:
        node_(node)
  {}

  template< class T >
  LCIter< T >::LCIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& rhs) const noexcept
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return &node_->val;
  }
}

#endif
