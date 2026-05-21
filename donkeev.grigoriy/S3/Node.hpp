#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <utility>

namespace donkeev
{
  template<class Key, class Value>
  struct Node
  {
    std::pair<Key, Value> data_;
    bool isOccupied_;

    Node();
    Node(const Key&, const Value&);
    bool isEmpty() const;
    Value dropNode();
  };

  template<class Key, class Value>
  Node<Key, Value>::Node():
    data_(),
    isOccupied_(false)
  {}

  template<class Key, class Value>
  Node<Key, Value>::Node(const Key& key, const Value& value):
    data_(key, value),
    isOccupied_(true)
  {}

  template<class Key, class Value>
  bool Node<Key, Value>::isEmpty() const
  {
    return !isOccupied_;
  }

  template<class Key, class Value>
  Value Node<Key, Value>::dropNode()
  {
    Value tmp = data_.second;
    isOccupied_ = false;
    return tmp;
  }
}

#endif
