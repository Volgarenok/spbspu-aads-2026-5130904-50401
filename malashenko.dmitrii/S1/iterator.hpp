#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <cassert>
#include <memory>
#include "list.hpp"
#include "node.hpp"

namespace malashenko
{
  template< class T > class List;
  template< class T > class Node;

  template< class T >
  class LIter {
  public:
    LIter() noexcept;
    LIter< T >& operator++() noexcept;
    LIter< T > operator++(int) noexcept;
    LIter< T > operator+(size_t s) noexcept;

    LIter< T >& operator--() noexcept;
    LIter< T > operator--(int) noexcept;
    LIter< T > operator-(size_t s) noexcept;

    T& operator*() noexcept;
    T* operator->() noexcept;

    bool operator!=(const LIter< T >& other) const noexcept;
    bool operator==(const LIter< T >& other) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T >* node_;
    LIter(detail::Node< T >* other) noexcept;
  };

  template< class T >
  LIter< T >::LIter() noexcept:
    node_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::Node< T >* other) noexcept:
    node_(other)
  {}

  template< class T >
  LIter< T >& LIter< T >::operator++() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LIter< T > tmp{*this};
    ++(*this);
    return tmp;
  }

  template< class T >
  LIter< T > LIter< T >::operator+(size_t s) noexcept
  {
    LIter< T > tmp = *this;
    for (size_t i = 0; i < s; ++i)
    {
      ++tmp;
    }
    return tmp;
  }


  template< class T >
  LIter< T >& LIter< T >::operator--() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator--(int) noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    LIter< T > tmp{*this};
    --(*this);
    return tmp;
  }

  template< class T >
  LIter< T > LIter< T >::operator-(size_t s) noexcept
  {
    LIter< T > tmp = *this;
    for (size_t i = 0; i < s; ++i)
    {
      --tmp;
    }
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& other) const noexcept
  {
    return !(other == *this);
  }

  template< class T >
  T& LIter< T >::operator*() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return node_ ->value_;
  }

  template< class T >
  T* LIter< T >::operator->() noexcept
  {
    assert(node_ != nullptr && "There is no node to gain access");
    return std::addressof(node_->value_);
  }
}

#endif
