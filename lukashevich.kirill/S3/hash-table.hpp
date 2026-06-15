#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include "../common/list.hpp"
#include "../common/vector.hpp"

namespace lukashevich {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    struct Node {
      Node():
        key_(),
        value_(),
        occupied_(false)
      {}

      Node(const Key & key, const Value & value):
        key_(key),
        value_(value),
        occupied_(true)
      {}

      Key key_;
      Value value_;
      bool occupied_;
    };

    class ConstIterator;
    class Iterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    HashTable();
    HashTable(size_t bucketCount,
        size_t bucketSize,
        Hash hash = Hash(),
        Equal equal = Equal());

    HashTable(const HashTable & rhs) = default;
    HashTable(HashTable && rhs) noexcept = default;
    HashTable & operator=(const HashTable & rhs) = default;
    HashTable & operator=(HashTable && rhs) noexcept = default;
    ~HashTable() = default;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t bucketCount() const noexcept;
    size_t bucketSize() const noexcept;
    size_t overflowSize() const noexcept;

  private:
    static size_t checkedMainSize(size_t bucketCount, size_t bucketSize);

    Vector< Node > buckets_;
    List< Node > overflow_;
    size_t bucketCount_;
    size_t bucketSize_;
    size_t size_;
    Hash hash_;
    Equal equal_;
  };
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(17, 4, Hash(), Equal())
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucketCount, size_t bucketSize, Hash hash, Equal equal):
  buckets_(checkedMainSize(bucketCount, bucketSize), Node()),
  overflow_(),
  bucketCount_(bucketCount),
  bucketSize_(bucketSize),
  size_(0),
  hash_(hash),
  equal_(equal)
{
  if ((bucketCount_ == 0) || (bucketSize_ == 0)) {
    throw std::invalid_argument("bad hash table param");
  }
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::checkedMainSize(size_t bucketCount, size_t bucketSize)
{
  if ((bucketCount != 0)
      && (bucketSize > static_cast< size_t >(-1) / bucketCount)) {
    throw std::overflow_error("hash table size overflow");
  }

  return bucketCount * bucketSize;
}

template< class Key, class Value, class Hash, class Equal >
bool lukashevich::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::bucketCount() const noexcept
{
  return bucketCount_;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::bucketSize() const noexcept
{
  return bucketSize_;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::overflowSize() const noexcept
{
  return overflow_.size();
}

#endif
