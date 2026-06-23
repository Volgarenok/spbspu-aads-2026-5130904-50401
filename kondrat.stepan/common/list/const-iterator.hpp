#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP

#include <cassert>
#include <memory>

namespace kondrat
{
  namespace detail
  {
    template< class T >
    class Node;
  }

  template< class T >
  class List;

  template< class T >
  class LCIter
  {
    public:
      LCIter() noexcept;
      LCIter< T > & operator++() noexcept;
      LCIter< T > operator++(int) noexcept;
      LCIter< T > & operator--() noexcept;
      LCIter< T > operator--(int) noexcept;
      bool operator==(const LCIter< T > & rhs) const noexcept;
      bool operator!=(const LCIter< T > & rhs) const noexcept;
      const T & operator*() const noexcept;
      const T * operator->() const noexcept;
    private:
      detail::Node< T > * node_;
      LCIter(detail::Node< T > * node) noexcept;
      friend class List< T >;
  };

  template< class T >
  LCIter< T >::LCIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LCIter< T > & LCIter< T >::operator++() noexcept
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
  bool LCIter< T >::operator==(const LCIter< T > & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T > & rhs) const noexcept
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  const T & LCIter< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  const T * LCIter< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->val);
  }

  template< class T >
  LCIter< T >::LCIter(detail::Node< T > * node) noexcept:
    node_(node)
  {}
}

#endif
