#ifndef HASHNODE_HPP
#define HASHNODE_HPP

namespace petrov
{
  enum NodeState
  {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template < class Key, class Value >
  struct HashNode
  {
    Key key_;
    Value value_;
    NodeState state_;

    HashNode();
    HashNode(const Key& key, const Value& value, NodeState state);
  };

  template < class Key, class Value >
  HashNode<Key, Value>::HashNode():
    state_(NodeState::EMPTY)
  {}

  template < class Key, class Value >
  HashNode<Key, Value>::HashNode(const Key& key, const Value& value, NodeState state):
    key_(key),
    value_(value),
    state_(state)
  {}
}

#endif
