#ifndef CUCKOO_HASH_NODE_HPP
#define CUCKOO_HASH_NODE_HPP

#include <utility>

namespace kondrat
{
  namespace detail
  {
    template< class Key, class Value >
    struct CuckooHashNode
    {
      std::pair< Key, Value > data_;
      bool occupied_;

      CuckooHashNode();
      CuckooHashNode(const Key & key, const Value & value);
      CuckooHashNode(const Key & key, Value && value);
    };

    template< class Key, class Value >
    CuckooHashNode< Key, Value >::CuckooHashNode():
      data_(),
      occupied_(false)
    {}

    template< class Key, class Value >
    CuckooHashNode< Key, Value >::CuckooHashNode(const Key & key, const Value & value):
      data_(key, value),
      occupied_(true)
    {}

    template< class Key, class Value >
    CuckooHashNode< Key, Value >::CuckooHashNode(const Key & key, Value && value):
      data_(key, std::move(value)),
      occupied_(true)
    {}
  }
}

#endif
