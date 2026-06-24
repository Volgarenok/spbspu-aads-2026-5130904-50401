#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <functional>
#include <limits>

#include "list/list.hpp"
#include "vector/vector.hpp"

namespace lukashevich {
  namespace detail {
    template< class Value >
    class HashTableArrow
    {
    public:
      explicit HashTableArrow(const Value & value);

      Value * operator->();
      const Value * operator->() const;

    private:
      Value value_;
    };
  }

  template< class Key, class Value, class Hash,
      class Equal = std::equal_to< Key > >
  class HashTable;

  template< class Key, class Value, class Hash,
    class Equal = std::equal_to< Key > >
  class HashTableIterator;

  template< class Key,class Value,class Hash,
    class Equal = std::equal_to< Key > >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    public:
      using value_type = std::pair< const Key, Value >;
      using iterator = HashTableIterator< Key, Value, Hash, Equal >;
      using const_iterator = HashTableConstIterator< Key, Value, Hash, Equal >;

      HashTable();
      HashTable(size_t bucketCount, size_t bucketSize, Hash hash = Hash(),
        Equal equal = Equal());

      Value & operator[](const Key & key);

      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;
      const_iterator cbegin() const;
      const_iterator cend() const;

      bool empty() const noexcept;
      size_t size() const noexcept;
      size_t bucketCount() const noexcept;
      size_t bucketSize() const noexcept;
      size_t overflowSize() const noexcept;
      bool has(const Key & key) const;

      Value & at(const Key & key);
      const Value & at(const Key & key) const;

      void add(const Key & key, const Value & value);
      void add(const Key & key, Value && value);
      bool erase(const Key & key);
      void clear() noexcept;
      void rehash(std::size_t bucketCount, std::size_t bucketSize);
      void swap(HashTable & rhs) noexcept;

    private:
      using StoredValue = std::pair< Key, Value >;

    struct Node
    {
      StoredValue value_;
      bool occupied_;

      Node();
      Node(const Key & key, const Value & value);
      Node(const Key & key, Value && value);
    };

  Vector< Node > buckets_;
  List< Node > overflow_;
  size_t bucketCount_;
  size_t bucketSize_;
  size_t size_;
  Hash hash_;
  Equal equal_;

  static size_t checkedMainSize(size_t bucketCount, size_t bucketSize);

  size_t totalNodes() const noexcept;
  size_t getHomeBucket(const Key & key) const;
  Node & nodeAt(size_t index);
  const Node & nodeAt(size_t index) const;
  Node * findNode(const Key & key);
  const Node * findNode(const Key & key) const;
  Node * findFreeInHomeBucket(const Key & key);

  template< class ValueArg >
  void insertImpl(const Key & key, ValueArg && value);

  friend class HashTableIterator< Key, Value, Hash, Equal >;
  friend class HashTableConstIterator< Key, Value, Hash, Equal >;
  };
}

template< class Value >
lukashevich::detail::HashTableArrow< Value >::HashTableArrow(
    const Value & value):
  value_(value)
{}

template< class Value >
Value * lukashevich::detail::HashTableArrow< Value >::operator->()
{
  return std::addressof(value_);
}

template< class Value >
const Value * lukashevich::detail::HashTableArrow< Value >::operator->() const
{
  return std::addressof(value_);
}

namespace lukashevich {
  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator
  {
  public:
    using value_type = std::pair< const Key &, Value & >;
    using pointer = detail::HashTableArrow< value_type >;

    HashTableIterator();

    value_type operator*() const;
    pointer operator->() const;
    HashTableIterator & operator++();
    HashTableIterator operator++(int);
    bool operator==(const HashTableIterator & rhs) const noexcept;
    bool operator!=(const HashTableIterator & rhs) const noexcept;

  private:
    HashTableIterator(HashTable< Key, Value, Hash, Equal > * table,
        std::size_t index);

    void skipEmpty();

    HashTable< Key, Value, Hash, Equal > * table_;
    std::size_t index_;

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator
  {
  public:
    using value_type = std::pair< const Key &, const Value & >;
    using pointer = detail::HashTableArrow< value_type >;

    HashTableConstIterator();
    HashTableConstIterator(
        const HashTableIterator< Key, Value, Hash, Equal > & rhs);

    value_type operator*() const;
    pointer operator->() const;
    HashTableConstIterator & operator++();
    HashTableConstIterator operator++(int);
    bool operator==(const HashTableConstIterator & rhs) const noexcept;
    bool operator!=(const HashTableConstIterator & rhs) const noexcept;

  private:
    HashTableConstIterator(const HashTable< Key, Value, Hash, Equal > * table,
        std::size_t index);

    void skipEmpty();

    const HashTable< Key, Value, Hash, Equal > * table_;
    std::size_t index_;

    friend class HashTable< Key, Value, Hash, Equal >;
  };
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator():
  table_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(
    HashTable< Key, Value, Hash, Equal > * table,
    std::size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTableIterator< Key, Value, Hash, Equal >::value_type
    lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator*()
        const
{
  typename HashTable< Key, Value, Hash, Equal >::Node & node =
      table_->nodeAt(index_);
  return value_type(node.value_.first, node.value_.second);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTableIterator< Key, Value, Hash, Equal >::pointer
    lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator->()
        const
{
  return pointer(operator*());
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableIterator< Key, Value, Hash, Equal > &
    lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableIterator< Key, Value, Hash, Equal >
    lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator++(int)
{
  HashTableIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator==(
    const HashTableIterator & rhs) const noexcept
{
  return (table_ == rhs.table_) && (index_ == rhs.index_);
}

template< class Key, class Value, class Hash, class Equal >
bool lukashevich::HashTableIterator< Key, Value, Hash, Equal >::operator!=(
    const HashTableIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTableIterator< Key, Value, Hash, Equal >::skipEmpty()
{
  if (table_ == nullptr) {
    return;
  }

  while ((index_ < table_->totalNodes()) && !table_->nodeAt(index_).occupied_) {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::HashTableConstIterator():
  table_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::HashTableConstIterator(
        const HashTableIterator< Key, Value, Hash, Equal > & rhs):
  table_(rhs.table_),
  index_(rhs.index_)
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::HashTableConstIterator(
        const HashTable< Key, Value, Hash, Equal > * table,
        std::size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::value_type
    lukashevich::HashTableConstIterator< Key, Value, Hash,
        Equal >::operator*() const
{
  const typename HashTable< Key, Value, Hash, Equal >::Node & node =
      table_->nodeAt(index_);
  return value_type(node.value_.first, node.value_.second);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::pointer
    lukashevich::HashTableConstIterator< Key, Value, Hash,
        Equal >::operator->() const
{
  return pointer(operator*());
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableConstIterator< Key, Value, Hash, Equal > &
    lukashevich::HashTableConstIterator< Key, Value, Hash,
        Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTableConstIterator< Key, Value, Hash, Equal >
    lukashevich::HashTableConstIterator< Key, Value, Hash,
        Equal >::operator++(int)
{
  HashTableConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::operator==(const HashTableConstIterator & rhs) const noexcept
{
  return (table_ == rhs.table_) && (index_ == rhs.index_);
}

template< class Key, class Value, class Hash, class Equal >
bool lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::operator!=(const HashTableConstIterator & rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTableConstIterator< Key, Value, Hash,
    Equal >::skipEmpty()
{
  if (table_ == nullptr) {
    return;
  }

  while ((index_ < table_->totalNodes()) && !table_->nodeAt(index_).occupied_) {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::Node::Node():
  value_(),
  occupied_(false)
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::Node::Node(
    const Key & key,
    const Value & value):
  value_(key, value),
  occupied_(true)
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::Node::Node(
    const Key & key,
    Value && value):
  value_(key, std::move(value)),
  occupied_(true)
{}


template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(17, 4, Hash(), Equal())
{}

template< class Key, class Value, class Hash, class Equal >
lukashevich::HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucketCount,
    size_t bucketSize, Hash hash, Equal equal):
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
void lukashevich::HashTable< Key, Value, Hash, Equal >::add(const Key & key, const Value & value)
{
  insertImpl(key, value);
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::add(
    const Key & key,
    Value && value)
{
  insertImpl(key, std::move(value));
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
bool lukashevich::HashTable< Key, Value, Hash, Equal >::erase(
    const Key & key)
{
  const std::size_t bucket = getHomeBucket(key);
  const std::size_t beginIndex = bucket * bucketSize_;
  const std::size_t endIndex = beginIndex + bucketSize_;

  for (std::size_t index = beginIndex; index < endIndex; ++index) {
    if (buckets_[index].occupied_
        && equal_(buckets_[index].value_.first, key)) {
      buckets_[index].occupied_ = false;
      --size_;
      return true;
    }
  }

  List< Node > temp;
  bool removed = false;

  for (LIter< Node > it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (!removed && equal_(it->value_.first, key)) {
      removed = true;
    } else {
      temp.pushBack(*it);
    }
  }

  if (removed) {
    overflow_ = std::move(temp);
    --size_;
  }

  return removed;
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
  LIter< Node > it = overflow_.begin();

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
  LCIter< Node > it = overflow_.begin();

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
  return iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::end()
{
  return iterator(this, totalNodes());
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
  return const_iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename lukashevich::HashTable< Key, Value, Hash, Equal >::const_iterator
    lukashevich::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return const_iterator(this, totalNodes());
}

template< class Key, class Value, class Hash, class Equal >
void lukashevich::HashTable< Key, Value, Hash, Equal >::rehash(
    size_t bucketCount,
    size_t bucketSize)
{
  HashTable temp(bucketCount, bucketSize, hash_, equal_);

  for (const_iterator it = cbegin(); it != cend(); ++it) {
    temp.add(it->first, it->second);
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
  if ((bucketCount == 0) || (bucketSize == 0)) {
    throw std::invalid_argument("bad hash table bucket parameters");
  }

  if (bucketSize > std::numeric_limits< std::size_t >::max() / bucketCount) {
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

  return node->value_.second;
}

template< class Key, class Value, class Hash, class Equal >
const Value & lukashevich::HashTable< Key, Value, Hash, Equal >::at(const Key & key) const
{
  const Node * node = findNode(key);

  if (node == nullptr) {
    throw std::out_of_range("hash table key not found");
  }

  return node->value_.second;
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
    if (buckets_[index].occupied_ && equal_(buckets_[index].value_.first, key)) {
      return std::addressof(buckets_[index]);
    }
  }

  for (LIter< Node > it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (equal_(it->value_.first, key)) {
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
    if (buckets_[index].occupied_ && equal_(buckets_[index].value_.first, key)) {
      return std::addressof(buckets_[index]);
    }
  }

  for (LCIter< Node > it = overflow_.begin(); it != overflow_.end(); ++it) {
    if (equal_(it->value_.first, key)) {
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

template< class Key, class Value, class Hash, class Equal >
template< class ValueArg >
void lukashevich::HashTable< Key, Value, Hash, Equal >::insertImpl(
    const Key & key,
    ValueArg && value)
{
  if (has(key)) {
    throw std::logic_error("duplicate key in hash table");
  }

  Node * freeNode = findFreeInHomeBucket(key);

  if (freeNode != nullptr) {
    freeNode->value_.first = key;
    freeNode->value_.second = std::forward< ValueArg >(value);
    freeNode->occupied_ = true;
    ++size_;
    return;
  }

  if (overflow_.size() >= bucketSize_) {
    throw std::overflow_error("hash table overflow bucket is full");
  }

  overflow_.pushBack(Node(key, std::forward< ValueArg >(value)));
  ++size_;
}

#endif
