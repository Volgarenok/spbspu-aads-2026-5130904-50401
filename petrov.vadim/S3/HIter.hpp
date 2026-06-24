#ifndef HITER_HPP
#define HITER_HPP

#include <memory>
#include <cassert>
#include <utility>
#include "HashNode.hpp"

namespace petrov
{
  using namespace detail;
  template< class K, class V, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct HIter
  {
    template< class K, class V, class Hash, class Equal >
    friend struct HashTable;

    HIter();

    HIter< Key, Value >& operator++();
    HIter< Key, Value > operator++(int);
    bool operator==(const HIter< Key, Value >& rhs) const;
    bool operator!=(const HIter< Key, Value >& rhs) const;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    private:
      HashNode< Key, Value >* node_;
      HashNode< Key, Value >* end_;

      HIter(detail::HashNode< Key, Value >* node, detail::HashNode< Key, Value > * end);
      void skipInvalid();
  };

  template< class Key, class Value >
  HIter< Key, Value >::HIter():
    node_{nullptr},
    end_{nullptr}
  {}

  template< class Key, class Value >
  HIter< Key, Value >& HIter< Key, Value >::operator++()
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
  HIter< Key, Value > HIter< Key, Value >::operator++(int)
  {
    HIter< Key, Value > cpy = *this;
    ++(*this);
    return cpy;
  }

  template< class Key, class Value >
  bool HIter< Key, Value >::operator==(const HIter< Key, Value >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool HIter< Key, Value >::operator!=(const HIter< Key, Value >& rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >& HIter< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return *reinterpret_cast< std::pair< Key, Value >* >(node_);
  }

  template< class Key, class Value >
  std::pair< Key, Value >* HIter< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return reinterpret_cast< std::pair< Key, Value >* >(node_);
  }

  template< class Key, class Value >
  HIter< Key, Value >::HIter(detail::HashNode< Key, Value > * node, detail::HashNode< Key, Value > * end):
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void HIter< Key, Value >::skipInvalid()
  {
    while (node_ != end_ && node_->state != OCCUPIED)
    {
      ++node_;
    }
  }

}
#endif
