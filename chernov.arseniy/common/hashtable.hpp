#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>

namespace chernov {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    HashTable();
    ~HashTable();
    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
  private:
    Value * _data;
    size_t * _bucket_sizes;
    size_t _num_buckets;
    size_t _bucket_cap;
    size_t _overflow_size;
    size_t _overflow_cap;
  };
}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::HashTable():
  _data(nullptr),
  _bucket_sizes(nullptr),
  _num_buckets(0),
  _bucket_cap(0),
  _overflow_size(0),
  _overflow_cap(0)
{}

template< class Key, class Value, class Hash, class Equal >
chernov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete [] _data;
  delete [] _bucket_sizes;
}

#endif
