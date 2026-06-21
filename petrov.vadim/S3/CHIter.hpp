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

    Value& operator*() const;
    const HashNode< Key, Value >* operator->() const;

    private:
      const HashNode< Key, Value >* node_;
      const HashNode< Key, Value >* end_;

      CHIter(const HashNode< Key, Value > * node, const HashNode< Key, Value > * end);
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
  Value& CHIter< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return node_->value_;
  }

  template< class Key, class Value >
  const HashNode< Key, Value >* CHIter< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return std::addressof(*node_);
  }

  template< class Key, class Value >
  CHIter< Key, Value >::CHIter(const HashNode< Key, Value > * node, const HashNode< Key, Value > * end):
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void CHIter< Key, Value >::skipInvalid()
  {
    while (node_ != end_ && node_->state_ != OCCUPIED)
    {
      ++node_;
    }
  }
}
#endif
