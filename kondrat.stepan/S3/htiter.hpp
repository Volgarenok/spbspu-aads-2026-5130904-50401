#ifndef HTITER_HPP
#define HTITER_HPP

#include <cassert>
#include <memory>
#include <utility>
#include "hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct HTIter
  {
    using value_type = std::pair< Key, Value >;

    HTIter() noexcept;

    HTIter< Key, Value > & operator++() noexcept;
    HTIter< Key, Value > operator++(int) noexcept;
    bool operator==(const HTIter< Key, Value > & rhs) const noexcept;
    bool operator!=(const HTIter< Key, Value > & rhs) const noexcept;
    value_type & operator*() noexcept;
    value_type * operator->() noexcept;

  private:
    detail::HashNode< Key, Value > * node_;
    detail::HashNode< Key, Value > * end_;

    HTIter(detail::HashNode< Key, Value > * node, detail::HashNode< Key, Value > * end) noexcept;
    void skipInvalid() noexcept;

    template< class K, class V, class H, class E >
    friend struct HashTable;
  };

  template< class Key, class Value >
  HTIter< Key, Value >::HTIter() noexcept:
    node_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class Value >
  HTIter< Key, Value > & HTIter< Key, Value >::operator++() noexcept
  {
    assert(node_ != nullptr);
    if (node_ != end_)
    {
      ++node_;
      skipInvalid();
    }
    return *this;
  }

  template< class Key, class Value >
  HTIter< Key, Value > HTIter< Key, Value >::operator++(int) noexcept
  {
    HTIter< Key, Value > result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator==(const HTIter< Key, Value > & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator!=(const HTIter< Key, Value > & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value >
  typename HTIter< Key, Value >::value_type & HTIter< Key, Value >::operator*() noexcept
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return node_->data;
  }

  template< class Key, class Value >
  typename HTIter< Key, Value >::value_type * HTIter< Key, Value >::operator->() noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value >
  HTIter< Key, Value >::HTIter(detail::HashNode< Key, Value > * node, detail::HashNode< Key, Value > * end) noexcept:
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void HTIter< Key, Value >::skipInvalid() noexcept
  {
    while (node_ != end_ && node_->state != detail::OCCUPIED)
    {
      ++node_;
    }
  }
}

#endif
