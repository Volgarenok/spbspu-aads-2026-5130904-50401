#ifndef HASH_TABLE_NODE
#define HASH_TABLE_NODE

namespace malashenko
{
  namespace detail
  {
    template< class Key, class Value >
    struct HashTableNode {
      bool isOccupied = false;
      Key key;
      Value Value;
    };
  }
}

#endif