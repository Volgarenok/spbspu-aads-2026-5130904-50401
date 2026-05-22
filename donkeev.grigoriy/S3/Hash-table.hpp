#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "Node.hpp"
#include "../common/top-it-vector.hpp"
#include "../common/vector-iterators.hpp"
#include "hashTableIt.hpp"

namespace donkeev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using iterator = donkeev::HTIt< Key, Value >;
    using constIterator = donkeev::HTCIt< Key, Value >;

    HashTable() = delete;
    HashTable(const size_t, const size_t);

    iterator begin();
    constIterator begin() const;
    iterator end();
    constIterator end() const;

    void add(const Key&, const Value&);
    Value drop(const Key&);
    bool has(const Key&);
    void rehash(size_t);

  private:
    topit::Vector< Node< Key, Value > > data_;
    size_t bucketCount_;
    size_t bucketSize_;
    size_t reserveBucketSize_;
    size_t totalElements_;
    Hash hashFunc_;
    Equal equalFunc_;

    Node<Key, Value>* findNode(const Key& key);
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
  typename HashTable< Key, Value, Hash, Equal >::iterator donkeev::HashTable< Key, Value, Hash, Equal >::begin()
  {
    iterator it{&data_, 0};
    if (it != end() && data_[0].isEmpty())
    {
      ++it;
    }

    return it;
  }
  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::constIterator donkeev::HashTable< Key, Value, Hash, Equal >::begin() const
  {
    constIterator it{&data_, 0};
    if (it != end() && data_[0].isEmpty())
    {
      ++it;
    }

    return it;
  }
  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator donkeev::HashTable< Key, Value, Hash, Equal >::end()
  {
    return {&data_, data_.getSize()};
  }
  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::constIterator donkeev::HashTable< Key, Value, Hash, Equal >::end() const
  {
    return constIterator{&data_, data_.getSize()};
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    size_t hash = hashFunc_(key);
    size_t bucketId = hash % bucketCount_;
    size_t startId = bucketId * bucketSize_;
    size_t endId = startId + bucketSize_;

    topit::VIter< Node< Key, Value > > begin = data_.begin() + startId;
    topit::VIter< Node< Key, Value > > end = data_.begin() + endId;
    while (begin != end)
    {
      if (begin->isEmpty())
      {
        *begin = Node< Key, Value >(key, value);
        ++totalElements_;
        return;
      }

      ++begin;
    }

    begin = data_.begin() + bucketCount_ * bucketSize_;
    end = begin + bucketSize_;
    while (begin != end)
    {
      if (begin->isEmpty())
      {
        *begin = Node< Key, Value >(key, value);
        ++totalElements_;
        return;
      }

      ++begin;
    }

    throw std::out_of_range("Can't add becaause of overflow");
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    Node<Key, Value>* node = findNode(key);
    if (!node)
    {
      throw std::out_of_range("No such element");
    }

    --totalElements_;
    return node->dropNode();
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& key)
  {
    Node< Key, Value >* node = findNode(key);
    if (!node)
    {
      return false;
    }
    return true;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t newBucketCount)
  {
    topit::Vector< Node< Key, Value > > newData((newBucketCount + 1) * bucketSize_, Node< Key, Value >());

    topit::VIter< Node< Key, Value > > oldDataBegin = data_.begin();
    topit::VIter< Node< Key, Value > > oldDataEnd = data_.end();
    while (oldDataBegin != oldDataEnd)
    {
      if (oldDataBegin->isEmpty())
      {
        ++oldDataBegin;
        continue;
      }
      size_t hash = hashFunc_(oldDataBegin->key_);
      size_t bucketId = hash % newBucketCount;
      size_t startId = bucketId * bucketSize_;
      size_t endId = startId + bucketSize_;

      topit::VIter< Node< Key, Value > > newDataBegin = newData.begin() + startId;
      topit::VIter< Node< Key, Value > > newDataEnd = newData.begin() + endId;
      bool tookPlace = false;
      while (newDataBegin != newDataEnd)
      {
        if (newDataBegin->isEmpty())
        {
          *newDataBegin = *oldDataBegin;
          tookPlace = true;
          break;
        }

        ++newDataBegin;
      }

      if (!tookPlace)
      {
        newDataBegin = newData.begin() + newBucketCount * bucketSize_;
        newDataEnd = newDataBegin + bucketSize_;
        while (newDataBegin != newDataEnd)
        {
          if (newDataBegin->isEmpty())
          {
            *newDataBegin = *oldDataBegin;
            tookPlace = true;
            break;
          }

          ++newDataBegin;
        }
      }

      if (!tookPlace)
      {
        throw std::invalid_argument("New table too small for existing elements");
      }

      ++oldDataBegin;
    }

    data_ = std::move(newData);
    bucketCount_ = newBucketCount;
  }

  template< class Key, class Value, class Hash, class Equal >
  Node<Key, Value>* HashTable< Key, Value, Hash, Equal >::findNode(const Key& key)
  {
    size_t hash = hashFunc_(key);
    size_t bucketId = hash % bucketCount_;
    size_t startId = bucketId * bucketSize_;
    size_t endId = startId + bucketSize_;

    topit::VIter< Node< Key, Value > > begin = data_.begin() + startId;
    topit::VIter< Node< Key, Value > > end = data_.begin() + endId;
    while (begin != end)
    {
      if (equalFunc_(key, begin->key))
      {
        return &(*begin);
      }
      ++begin;
    }

    begin = data_.begin() + bucketCount_ * bucketSize_;
    end = begin + bucketSize_;
    while (begin != end)
    {
      if (equalFunc_(key, begin->key))
      {
        return &(*begin);
      }
      ++begin;
    }

    return nullptr;
  }
}

#endif
