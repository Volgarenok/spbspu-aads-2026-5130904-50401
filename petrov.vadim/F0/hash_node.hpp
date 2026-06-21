#ifndef HASH_NODE_HPP
#define HASH_NODE_HPP

namespace petrov
{
  namespace detail
  {
    enum NodeState
    {
      EMPTY,
      OCCUPIED,
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
}


#endif
