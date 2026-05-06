#ifndef HASH_TABLE_ITER
#define HASH_TABLE_ITER
#include <utility>
#include <common/list/list.hpp>
#include <common/list/iterator.hpp>
#include <hash_table.hpp>
namespace malashenko
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIter {
  public:
    HashTableIter(HashTable< Key, Value, Hash, Equal >* hashTable, size_t ind, LIter< std::pair<Key, Value> > it);
    HashTableIter< Key, Value, Hash, Equal >& operator++();
    HashTableIter< Key, Value, Hash, Equal > operator++(int);

    HashTableIter< Key, Value, Hash, Equal >& operator--();
    HashTableIter< Key, Value, Hash, Equal > operator--(int);

    std::pair<Key, Value>& operator*();
    std::pair<Key, Value>* operator->();

    bool operator!=(const HashTableIter< Key, Value, Hash, Equal >& other) const;
    bool operator==(const HashTableIter< Key, Value, Hash, Equal >& other) const;
  private:
    HashTable< Key, Value, Hash, Equal >* hashTable_;
    size_t ind_;
    LIter< std::pair<Key, Value> > it_;
  };
}

#endif