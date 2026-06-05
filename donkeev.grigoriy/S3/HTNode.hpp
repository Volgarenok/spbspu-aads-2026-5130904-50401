#ifndef HTNODE_HPP
#define HTNODE_HPP

#include <iostream>
#include <utility>

namespace donkeev
{
  template<class Key, class Value>
  struct HTNode
  {
    std::pair<Key, Value> data_;
    bool isOccupied_;

    HTNode();
    HTNode(const Key&, const Value&);
    HTNode(const Key&, Value&&);
    HTNode& operator=(const HTNode& other);
    HTNode& operator=(HTNode&&) noexcept;
    bool isEmpty() const;
    Value dropNode();
  };

  template<class Key, class Value>
  HTNode<Key, Value>::HTNode():
    data_(std::make_pair(Key(), Value())),
    isOccupied_(false)
  {}

  template<class Key, class Value>
  HTNode<Key, Value>::HTNode(const Key& key, const Value& value):
    data_(key, value),
    isOccupied_(true)
  {}

  template<class Key, class Value>
  HTNode<Key, Value>::HTNode(const Key& key, Value&& value):
  data_(key, Value{}),  
  isOccupied_(true)
  {
    data_.second = std::move(value);
  }

  template< class Key, class Value >
  HTNode< Key, Value >& HTNode< Key, Value >::operator=(const HTNode& other)
  {
    data_ = other.data_;
    isOccupied_ = other.isOccupied_;
    return *this;
  }

  template< class Key, class Value >
  HTNode< Key, Value >& HTNode< Key, Value >::operator=(HTNode&& other) noexcept
  {
    data_ = std::move(other.data_);
    isOccupied_ = other.isOccupied_;
    return *this;
  }

  template<class Key, class Value>
  bool HTNode<Key, Value>::isEmpty() const
  {
    return !isOccupied_;
  }

  template<class Key, class Value>
  Value HTNode<Key, Value>::dropNode()
  {
    Value tmp = data_.second;
    isOccupied_ = false;
    return tmp;
  }
}

#endif
