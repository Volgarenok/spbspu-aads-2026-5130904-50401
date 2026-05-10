#include "Node.hpp"
#include "../common/top-it-vector.hpp"
#include "../common/vector-iterators.hpp"

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

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const size_t bucketCount, const size_t bucketSize):
    data_((bucketCount + 1) * bucketSize, Node< Key, Value >()),
    bucketCount_(bucketCount),
    bucketSize_(bucketSize),
    reserveBucketSize_(bucketSize),
    totalElements_(0),
    hashFunc_(),
    equalFunc_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    size_t hash = hashFunc_(key);
    size_t bucketId = hash % bucketCount_;
    size_t startId = bucketId * bucketSize_;
    size_t endId = startId + bucketSize_;

    topit::VIter< Node< Key, Value > > begin = data_.begin() + startId;
    topit::VIter< Node< Key, Value > > end = data_.end();
    while (begin != end)
    {
      if (begin->isEmpty())
      {
        *begin = Node< Key, Value >(key, value);
        ++totalElements_;
        return;
      }
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    size_t hash = hashFunc_(key);
    size_t bucketId = hash % bucketCount_;
    size_t startId = bucketId * bucketSize_;
    size_t endId = startId + bucketSize_;

    topit::VIter< Node< Key, Value > > begin = data_.begin() + startId;
    topit::VIter< Node< Key, Value > > end = begin + endId;
    while (begin != end)
    {
      if (equalFunc_(key, begin->key))
      {
        --totalElements_;
        return begin->dropNode();
      }
      ++begin;
    }

    begin = data_.begin() + bucketCount_ * bucketSize_;
    end = begin + bucketSize_;
    while (begin != end)
    {
      if (equalFunc_(key, begin->key))
      {
        --totalElements_;
        return begin->dropNode();
      }
      ++begin;
    }

    throw std::out_of_range("No such element");
  }
}
