#include "Node.hpp"
#include "common/top-it-vector.hpp"
#include "common/vector-iterators.hpp"

namespace donkeev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    topit::Vector< Node< Key, Value > > data_;
    size_t bucketCount_;
    size_t bucketSize_;
    size_t reserveBucketSize_;
    size_t totalElements_;
    Hash hashFunc_;
    Equal equalFunc_;

    HashTable() = delete;
    HashTable(const size_t, const size_t);

    void add(const Key&, const Value&);
    Value drop(const Key&);
    bool has(const Key key);
    void rehash(size_t slots);
  };
}
