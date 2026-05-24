#ifndef YALOVSKY_ITERATOR_HPP
#define YALOVSKY_ITERATOR_HPP

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>

#include "node.hpp"

namespace yalovsky
{
  template< class T >
  class List;

  template< class T >
  class LCIter;

  template< class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    LIter() noexcept;

    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
    LIter operator+(std::size_t step) const noexcept;

    LIter& operator--() noexcept;
    LIter operator--(int) noexcept;
    LIter operator-(std::size_t step) const noexcept;

    T& operator*() const;
    T* operator->() const;

    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;

  private:
    explicit LIter(NodeBase* node) noexcept;

    NodeBase* node_;
  };

  template< class T >
  LIter< T >::LIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(NodeBase* node) noexcept:
    node_(node)
  {}

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  LIter< T > LIter< T >::operator+(std::size_t step) const noexcept
  {
    LIter< T > result(*this);
    for (std::size_t i = 0; i < step; ++i)
    {
      ++result;
    }
    return result;
  }

  template< class T >
  LIter< T >& LIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    LIter< T > result(*this);
    --(*this);
    return result;
  }

  template< class T >
  LIter< T > LIter< T >::operator-(std::size_t step) const noexcept
  {
    LIter< T > result(*this);
    for (std::size_t i = 0; i < step; ++i)
    {
      --result;
    }
    return result;
  }

  template< class T >
  T& LIter< T >::operator*() const
  {
    assert(node_ != nullptr);
    return static_cast< Node< T >* >(node_)->value_;
  }

  template< class T >
  T* LIter< T >::operator->() const
  {
    return std::addressof(operator*());
  }

  template< class T >
  bool LIter< T >::operator==(const LIter& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter& other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
