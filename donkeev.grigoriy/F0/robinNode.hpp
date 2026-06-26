#ifndef ROBIN_NODE_HPP
#define ROBIN_NODE_HPP

#include <utility>

namespace donkeev
{
  template<class Key, class Value>
  struct RobinNode
  {
    Key key_;
    Value value_;
    bool isOccupied_;
    int psl_;

    RobinNode();

    RobinNode(const RobinNode&);
    RobinNode(RobinNode&&) noexcept;

    RobinNode(const Key&, const Value&, int);

    RobinNode& operator=(const RobinNode&);

    void swap(RobinNode&);
  };

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode():
    key_(),
    value_(),
    isOccupied_(false),
    psl_(-1)
  {}

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode(const RobinNode& other):
    key_(other.key_),
    value_(other.value_),
    isOccupied_(other.isOccupied_),
    psl_(other.psl_)
  {}

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode(RobinNode&& other) noexcept:
    key_(std::move(other.key_)),
    value_(std::move(other.value_)),
    isOccupied_(other.isOccupied_),
    psl_(other.psl_)
  {
    other.isOccupied_ = false;
    other.psl_ = -1;
  }

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode(const Key& key, const Value& value, int psl):
    key_(key),
    value_(value),
    isOccupied_(true),
    psl_(psl)
  {}

  template< class Key, class Value >
  RobinNode< Key, Value >& RobinNode< Key, Value >::operator=(const RobinNode< Key, Value >& other)
  {
    if (this != &other)
    {
      key_ = other.key_;
      value_ = other.value_;
      isOccupied_ = other.isOccupied_;
      psl_ = other.psl_;
    }

    return *this;
  }

  template< class Key, class Value >
  void RobinNode< Key, Value >::swap(RobinNode< Key, Value >& other)
  {
    std::swap(key_, other.key_);
    std::swap(value_, other.value_);
    std::swap(isOccupied_, other.isOccupied_);
    std::swap(psl_, other.psl_);
  }
}

#endif
