#ifndef HASHNODE_HPP
#define HASHNODE_HPP

namespace petrov
{
  namespace detail
  {
    enum NodeState
    {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };

    template< class Key, class Value >
    struct HashNode
    {
      Key key;
      Value value;
      NodeState state;

      HashNode();
      HashNode(const Key& key, const Value& value, NodeState state);
    };

    template< class Key, class Value >
    HashNode< Key, Value >::HashNode():
      state(NodeState::EMPTY)
    {}

    template< class Key, class Value >
    HashNode< Key, Value >::HashNode(const Key& k, const Value& v, NodeState s):
      key(k),
      value(v),
      state(s)
    {}
  }
}

#endif
