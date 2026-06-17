#ifndef CUCKOO_HASH_TABLE_NODE_HPP
#define CUCKOO_HASH_TABLE_NODE_HPP

namespace malashenko
{
  namespace detail
  {
    template< class Key, class Value >
    struct HashTableNode {
      bool isOccupied = false;
      Key key;
      Value value;
    };
  }
}

#endif