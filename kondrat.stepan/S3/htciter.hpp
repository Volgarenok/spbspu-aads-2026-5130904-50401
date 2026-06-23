#ifndef HTCITER_HPP
#define HTCITER_HPP

#include <cassert>
#include <memory>
#include <utility>
#include "hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct HTCIter
  {
    using value_type = std::pair< Key, Value >;

    HTCIter() noexcept;

    HTCIter< Key, Value > & operator++() noexcept;
    HTCIter< Key, Value > operator++(int) noexcept;
    bool operator==(const HTCIter< Key, Value > & rhs) const noexcept;
    bool operator!=(const HTCIter< Key, Value > & rhs) const noexcept;
    const value_type & operator*() const noexcept;
    const value_type * operator->() const noexcept;

  private:
    const detail::HashNode< Key, Value > * node_;
    const detail::HashNode< Key, Value > * end_;

    HTCIter(const detail::HashNode< Key, Value > * node, const detail::HashNode< Key, Value > * end) noexcept;
    void skipInvalid() noexcept;

    template< class K, class V, class H, class E >
    friend struct HashTable;
  };

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter() noexcept:
    node_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class Value >
  HTCIter< Key, Value > & HTCIter< Key, Value >::operator++() noexcept
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
  HTCIter< Key, Value > HTCIter< Key, Value >::operator++(int) noexcept
  {
    HTCIter< Key, Value > result(*this);
    ++(*this);
    return result;
  }

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator==(const HTCIter< Key, Value > & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator!=(const HTCIter< Key, Value > & rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< class Key, class Value >
  const typename HTCIter< Key, Value >::value_type & HTCIter< Key, Value >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return node_->data;
  }

  template< class Key, class Value >
  const typename HTCIter< Key, Value >::value_type * HTCIter< Key, Value >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter(const detail::HashNode< Key, Value > * node, const detail::HashNode< Key, Value > * end) noexcept:
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void HTCIter< Key, Value >::skipInvalid() noexcept
  {
    while (node_ != end_ && node_->state != detail::OCCUPIED)
    {
      ++node_;
    }
  }
}

#endif
