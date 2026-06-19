#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <cassert>
#include <memory>
#include "list.hpp"
#include "node.hpp"

namespace malashenko
{
  template< class T > class List;
  template< class T > class Node;

  template< class T >
  class LCIter {
  public:
    LCIter() noexcept;
    LCIter< T >& operator++() noexcept;
    LCIter< T > operator++(int) noexcept;
    LCIter< T > operator+(size_t s) noexcept;

    LCIter< T >& operator--() noexcept;
    LCIter< T > operator--(int) noexcept;
    LCIter< T > operator-(size_t s) noexcept;

    const T& operator*() const noexcept;
    const T* operator->() const noexcept;

    bool operator!=(const LCIter< T >& other) const noexcept;
    bool operator==(const LCIter< T >& other) const noexcept;
  private:
    detail::Node< T >* node_;
    LCIter(detail::Node< T >* other) noexcept;
    friend class List< T >;
  };

  template< class T >
  LCIter< T >::LCIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(detail::Node< T >* other) noexcept:
    node_(other)
  {}

  template< class T >
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LCIter< T > tmp{*this};
    ++(*this);
    return tmp;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator+(size_t s) noexcept
  {
    LCIter< T > tmp = *this;
    for (size_t i = 0; i < s; ++i)
    {
      ++tmp;
    }
    return tmp;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LCIter< T > tmp{*this};
    --(*this);
    return tmp;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator-(size_t s) noexcept
  {
    LCIter< T > tmp = *this;
    for (size_t i = 0; i < s; ++i)
    {
      --tmp;
    }
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& other) const noexcept
  {
    return !(other == *this);
  }

  template< class T >
  const T& LCIter< T >::operator*() const noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return node_ ->value_;
  }

  template< class T >
  const T* LCIter< T >::operator->() const noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return std::addressof(node_->value_);
  }
}

#endif
