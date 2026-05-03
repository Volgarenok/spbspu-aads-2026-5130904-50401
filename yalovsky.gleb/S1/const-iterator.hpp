#ifndef YALOVSKY_CONST_ITERATOR_HPP
#define YALOVSKY_CONST_ITERATOR_HPP

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>

#include "node.hpp"
#include "iterator.hpp"

namespace yalovsky
{
  template< class T >
  class List;

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    LCIter() noexcept;
    LCIter(const LIter< T >& other) noexcept;

    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
    LCIter operator+(std::size_t step) const noexcept;

    LCIter& operator--() noexcept;
    LCIter operator--(int) noexcept;
    LCIter operator-(std::size_t step) const noexcept;

    const T& operator*() const;
    const T* operator->() const;

    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;

  private:
    explicit LCIter(NodeBase* node) noexcept;

    NodeBase* node_;
  };

  template< class T >
  LCIter< T >::LCIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T >& other) noexcept:
    node_(other.node_)
  {}

  template< class T >
  LCIter< T >::LCIter(NodeBase* node) noexcept:
    node_(node)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator+(std::size_t step) const noexcept
  {
    LCIter< T > result(*this);
    for (std::size_t i = 0; i < step; ++i)
    {
      ++result;
    }
    return result;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->prev_;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    LCIter< T > result(*this);
    --(*this);
    return result;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator-(std::size_t step) const noexcept
  {
    LCIter< T > result(*this);
    for (std::size_t i = 0; i < step; ++i)
    {
      --result;
    }
    return result;
  }

  template< class T >
  const T& LCIter< T >::operator*() const
  {
    assert(node_ != nullptr);
    return static_cast< const Node< T >* >(node_)->value_;
  }

  template< class T >
  const T* LCIter< T >::operator->() const
  {
    return std::addressof(operator*());
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter& other) const noexcept
  {
    return !(*this == other);
  }
}

#endif
