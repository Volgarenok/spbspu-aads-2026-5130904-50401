#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include "../common/list.hpp"
#include "../common/vector.hpp"
dgmrgjr
namespace lukashevich {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    public:
      struct Node
      {
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

      using OverflowIterator = LIter< Node >;
      using OverflowConstIterator = LCIter< Node >;

      class ConstIterator
      {
        public:
          ConstIterator():
            table_(nullptr),
            index_(0)
          {}

          const Node & operator*() const
          {
            return table_->nodeAt(index_);
          }

          const Node * operator->() const
          {
            return std::addressof(table_->nodeAt(index_));
          }

          ConstIterator & operator++()
          {
            ++index_;
            skipEmpty();
            return *this;
          }

          bool operator==(const ConstIterator & rhs) const noexcept
          {
            return (table_ == rhs.table_) && (index_ == rhs.index_);
          }

          bool operator!=(const ConstIterator & rhs) const noexcept
          {
            return !(*this == rhs);
          }

        private:
          ConstIterator(const HashTable * table, size_t index):
            table_(table),
            index_(index)
          {
            skipEmpty();
          }

          void skipEmpty()
          {
            while ((index_ < table_->totalNodes()) && !table_->nodeAt(index_).occupied_) {
              ++index_;
            }
          }

          const HashTable * table_;
          size_t index_;
          friend class HashTable< Key, Value, Hash, Equal >;
      };
    class Iterator
    {
      public:
        Iterator():
          table_(nullptr),
          index_(0)
        {}

        Node & operator*() const
        {
          return table_->nodeAt(index_);
        }

        Node * operator->() const
        {
          return std::addressof(table_->nodeAt(index_));
        }

        Iterator & operator++()
        {
          ++index_;
          skipEmpty();
          return *this;
        }

        bool operator==(const Iterator & rhs) const noexcept
        {
          return (table_ == rhs.table_) && (index_ == rhs.index_);
        }

        bool operator!=(const Iterator & rhs) const noexcept
        {
          return !(*this == rhs);
        }

      private:
        Iterator(HashTable * table, size_t index):
          table_(table),
          index_(index)
        {
          skipEmpty();
        }

        void skipEmpty()
        {
          while ((index_ < table_->totalNodes()) && !table_->nodeAt(index_).occupied_) {
            ++index_;
          }
        }

        HashTable * table_;
        size_t index_;
        friend class HashTable< Key, Value, Hash, Equal >;
    };

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

    bool has(const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    void add(const Key & key, const Value & value);
    Value & operator[](const Key & key);

    Value drop(const Key & key);
    void clear() noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    void rehash(size_t bucketCount, size_t bucketSize);
    void swap(HashTable & rhs) noexcept;

    private:
      static size_t checkedMainSize(size_t bucketCount, size_t bucketSize);
      size_t getHomeBucket(const Key & key) const;
      Node * findNode(const Key & key);
      const Node * findNode(const Key & key) const;
      Node * findFreeInHomeBucket(const Key & key);
      size_t totalNodes() const noexcept;
      Node & nodeAt(size_t index);
      const Node & nodeAt(size_t index) const;

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

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::add(
    const Key & key,
    const Value & value)
{
  if (has(key)) {
    throw std::logic_error("duplicate key in hash table");
  }

  Node * freeNode = findFreeInHomeBucket(key);

  if (freeNode != nullptr) {
    freeNode->key_ = key;
    freeNode->value_ = value;
    freeNode->occupied_ = true;
    ++size_;
    return;
  }

  if (overflow_.size() >= bucketSize_) {
    throw std::overflow_error("hash table overflow bucket is full");
  }

  overflow_.pushBack(Node(key, value));
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
Value & lukashevich::HashTable< Key, Value, Hash, Equal >::operator[](const Key & key)
{
  if (!has(key)) {
    add(key, Value());
  }

  return at(key);
}

template< class Key, class Value, class Hash, class Equal >
Value lukashevich::HashTable< Key, Value, Hash, Equal >::drop(const Key & key)
{
  const size_t bucket = getHomeBucket(key);
  const size_t begin = bucket * bucketSize_;
  const size_t finish = begin + bucketSize_;

  for (size_t index = begin; index < finish; ++index) {
    if (buckets_[index].occupied_ && equal_(buckets_[index].key_, key)) {
      Value value = buckets_[index].value_;
      buckets_[index].occupied_ = false;
      --size_;
      return value;
    }
  }

  List< Node > temp;
  bool found = false;
  Value value = Value();

  for (OverflowIterator it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (!found && equal_(it->key_, key)) {
      value = it->value_;
      found = true;
    } else {
      temp.pushBack(*it);
    }
  }

  if (found) {
    overflow_ = std::move(temp);
    --size_;
    return value;
  }

  throw std::out_of_range("hash table key not found");
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t index = 0; index < buckets_.getSize(); ++index) {
    buckets_[index].occupied_ = false;
  }

  overflow_.clear();
  size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::totalNodes() const noexcept
{
  return buckets_.getSize() + overflow_.size();
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::Node &
    lukashevich::HashTable< Key, Value, Hash, Equal >::nodeAt(size_t index)
{
  if (index < buckets_.getSize()) {
    return buckets_[index];
  }

  size_t overflowIndex = index - buckets_.getSize();
  OverflowIterator it = overflow_.begin();

  while (overflowIndex != 0) {
    --overflowIndex;
    ++it;
  }

  return *it;
}

template< class Key, class Value, class Hash, class Equal >
const typename lukashevich::HashTable< Key, Value, Hash, Equal >::Node &
    lukashevich::HashTable< Key, Value, Hash, Equal >::nodeAt(size_t index) const
{
  if (index < buckets_.getSize()) {
    return buckets_[index];
  }

  size_t overflowIndex = index - buckets_.getSize();
  OverflowConstIterator it = overflow_.begin();

  while (overflowIndex != 0) {
    --overflowIndex;
    ++it;
  }

  return *it;
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::begin()
{
  return Iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(this, totalNodes());
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::const_iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::const_iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::end() const
{
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::const_iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::const_iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return ConstIterator(this, totalNodes());
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::rehash(
    size_t bucketCount,
    size_t bucketSize)
{
  HashTable temp(bucketCount, bucketSize, hash_, equal_);

  for (const_iterator it = cbegin(); it != cend(); ++it) {
    temp.add(it->key_, it->value_);
  }

  swap(temp);
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::swap(HashTable & rhs) noexcept
{
  buckets_.swap(rhs.buckets_);
  overflow_.swap(rhs.overflow_);
  std::swap(bucketCount_, rhs.bucketCount_);
  std::swap(bucketSize_, rhs.bucketSize_);
  std::swap(size_, rhs.size_);
  std::swap(hash_, rhs.hash_);
  std::swap(equal_, rhs.equal_);
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
bool lukashevich::HashTable< Key, Value, Hash, Equal >::has(const Key & key) const
{
  return findNode(key) != nullptr;
}

template< class Key, class Value, class Hash, class Equal >
Value & lukashevich::HashTable< Key, Value, Hash, Equal >::at(const Key & key)
{
  Node * node = findNode(key);

  if (node == nullptr) {
    throw std::out_of_range("hash table key not found");
  }

  return node->value_;
}

template< class Key, class Value, class Hash, class Equal >
const Value & lukashevich::HashTable< Key, Value, Hash, Equal >::at(const Key & key) const
{
  const Node * node = findNode(key);

  if (node == nullptr) {
    throw std::out_of_range("hash table key not found");
  }

  return node->value_;
}

template< class Key, class Value, class Hash, class Equal >
size_t lukashevich::HashTable< Key, Value, Hash, Equal >::getHomeBucket(const Key & key) const
{
  return hash_(key) % bucketCount_;
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::Node *
    lukashevich::HashTable< Key, Value, Hash, Equal >::findNode(const Key & key)
{
  const size_t bucket = getHomeBucket(key);
  const size_t begin = bucket * bucketSize_;
  const size_t finish = begin + bucketSize_;

  for (size_t index = begin; index < finish; ++index) {
    if (buckets_[index].occupied_ && equal_(buckets_[index].key_, key)) {
      return std::addressof(buckets_[index]);
    }
  }

  for (OverflowIterator it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (equal_(it->key_, key)) {
      return std::addressof(*it);
    }
  }

  return nullptr;
}

template< class Key, class Value, class Hash, class Equal >
const typename lukashevich::HashTable< Key, Value, Hash, Equal >::Node *
    lukashevich::HashTable< Key, Value, Hash, Equal >::findNode(const Key & key) const
{
  const size_t bucket = getHomeBucket(key);
  const size_t begin = bucket * bucketSize_;
  const size_t finish = begin + bucketSize_;

  for (size_t index = begin; index < finish; ++index) {
    if (buckets_[index].occupied_ && equal_(buckets_[index].key_, key)) {
      return std::addressof(buckets_[index]);
    }
  }

  for (OverflowConstIterator it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (equal_(it->key_, key)) {
      return std::addressof(*it);
    }
  }

  return nullptr;
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::Node *
    lukashevich::HashTable< Key, Value, Hash, Equal >::findFreeInHomeBucket(
        const Key & key)
{
  const size_t bucket = getHomeBucket(key);
  const size_t begin = bucket * bucketSize_;
  const size_t finish = begin + bucketSize_;

  for (size_t index = begin; index < finish; ++index) {
    if (!buckets_[index].occupied_) {
      return std::addressof(buckets_[index]);
    }
  }

  return nullptr;
}

#endif
