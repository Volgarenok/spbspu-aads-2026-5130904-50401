#ifndef CHITER_HPP
#define CHITER_HPP

#include "HashNode.hpp"
#include <memory>
#include <cassert>

namespace petrov
{
  template< class K, class V, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct CHIter
  {
    template< class K, class V, class Hash, class Equal >
    friend struct HashTable;

    CHIter();

    CHIter< Key, Value >& operator++();
    CHIter< Key, Value > operator++(int);
    bool operator==(const CHIter< Key, Value >& rhs) const;
    bool operator!=(const CHIter< Key, Value >& rhs) const;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    private:
      const detail::HashNode< Key, Value >* node_;
      const detail::HashNode< Key, Value >* end_;

      CHIter(const detail::HashNode< Key, Value > * node, const detail::HashNode< Key, Value > * end);
      void skipInvalid();
  };

  template< class Key, class Value >
  CHIter< Key, Value >::CHIter():
    node_{nullptr},
    end_{nullptr}
  {}

  template< class Key, class Value >
  CHIter< Key, Value >& CHIter< Key, Value >::operator++()
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
  CHIter< Key, Value > CHIter< Key, Value >::operator++(int)
  {
    CHIter< Key, Value > cpy = *this;
    ++(*this);
    return cpy;
  }

  template< class Key, class Value >
  bool CHIter< Key, Value >::operator==(const CHIter< Key, Value >& rhs) const
  {
    return node_ == rhs.node_ && end_ == rhs.end_;
  }

  template< class Key, class Value >
  bool CHIter< Key, Value >::operator!=(const CHIter< Key, Value >& rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& CHIter< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return *reinterpret_cast< const std::pair< Key, Value >* >(node_);
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* CHIter< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return reinterpret_cast< const std::pair< Key, Value >* >(node_);
  }

  template< class Key, class Value >
  CHIter< Key, Value >::CHIter(const detail::HashNode< Key, Value > * node, const detail::HashNode< Key, Value > * end):
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void CHIter< Key, Value >::skipInvalid()
  {
    while (node_ != end_ && node_->state != detail::OCCUPIED)
    {
      ++node_;
    }
  }
}
#endif
