#ifndef ITER_HPP
#define ITER_HPP

#include <cassert>
#include <memory>

namespace lukashevich
{
  namespace detail
  {
    template< class T >
    class Node;
  }

  template< class T >
  class List;

  template< class T >
  class LIter
  {
    public:
      LIter() noexcept;
      LIter< T >& operator++() noexcept;
      LIter< T > operator++(int) noexcept;
      LIter< T >& operator--() noexcept;
      LIter< T > operator--(int) noexcept;

      bool operator==(const LIter< T >& rhs) const noexcept;
      bool operator!=(const LIter< T >& rhs) const noexcept;

      T& operator*() noexcept;
      T* operator->() noexcept;

    private:
      detail::Node< T >* node_;
      explicit LIter(detail::Node< T >* node) noexcept;
      friend class List< T >;
  };

  template< class T >
  LIter< T >::LIter(detail::Node< T >* node) noexcept:
    node_(node)
  {}

  template< class T >
  LIter< T >::LIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  LIter< T >& LIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& rhs) const noexcept
  {
    return !(node_ == rhs.node_);
  }

  template< class T >
  T& LIter< T >::operator*() noexcept
  {
    assert(node_ != nullptr);
    return node_->val;
  }

  template< class T >
  T* LIter< T >::operator->() noexcept
  {
    assert(node_ != nullptr);
    return std::addressof(node_->val);
  }
}

#endif
