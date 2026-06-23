#ifndef HASH_NODE_HPP
#define HASH_NODE_HPP

#include <utility>

namespace kondrat
{
  namespace detail
  {
    enum HashNodeState
    {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };

    template< class Key, class Value >
    struct HashNode
    {
      using value_type = std::pair< Key, Value >;

      value_type data;
      HashNodeState state;

      HashNode();
    };

    template< class Key, class Value >
    HashNode< Key, Value >::HashNode():
      data(),
      state(EMPTY)
    {}
  }
}

#endif
