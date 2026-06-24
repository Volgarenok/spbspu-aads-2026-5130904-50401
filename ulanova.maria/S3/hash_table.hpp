#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>

#include "top_it_vector.hpp"

namespace ulanova
{
template< class Key, class Value, class Hash, class Equal >
class HashTable;

template< class Key, class Value, class Hash, class Equal >
class HashTableIterator
{
public:
  HashTableIterator();

  HashTableIterator& operator++();
  HashTableIterator operator++(int);

  bool operator==(const HashTableIterator& rhs) const noexcept;
  bool operator!=(const HashTableIterator& rhs) const noexcept;

  std::pair< const Key&, Value& > operator*() const;
  std::pair< const Key*, Value* > operator->() const;

  const Key& key() const;

private:
  friend class HashTable< Key, Value, Hash, Equal >;

  using Table = HashTable< Key, Value, Hash, Equal >;

  Table* table_;
  size_t index_;

  HashTableIterator(Table* table, size_t index);
  void skipEmpty();
};

template< class Key, class Value, class Hash, class Equal >
class HashTableConstIterator
{
public:
  HashTableConstIterator();
  HashTableConstIterator(const HashTableIterator< Key, Value, Hash, Equal >& it);

  HashTableConstIterator& operator++();
  HashTableConstIterator operator++(int);

  bool operator==(const HashTableConstIterator& rhs) const noexcept;
  bool operator!=(const HashTableConstIterator& rhs) const noexcept;

  std::pair< const Key&, const Value& > operator*() const;
  std::pair< const Key*, const Value* > operator->() const;

  const Key& key() const;

private:
  friend class HashTable< Key, Value, Hash, Equal >;

  using Table = HashTable< Key, Value, Hash, Equal >;

  const Table* table_;
  size_t index_;

  HashTableConstIterator(const Table* table, size_t index);
  void skipEmpty();
};

template< class Key, class Value, class Hash, class Equal >
class HashTable
{
public:
  using iterator = HashTableIterator< Key, Value, Hash, Equal >;
  using const_iterator = HashTableConstIterator< Key, Value, Hash, Equal >;

  explicit HashTable(size_t slots = 8);
  HashTable(const HashTable& rhs);
  HashTable(HashTable&& rhs) noexcept;

  HashTable& operator=(const HashTable& rhs);
  HashTable& operator=(HashTable&& rhs) noexcept;

  void swap(HashTable& rhs) noexcept;

  bool empty() const noexcept;
  bool isEmpty() const noexcept;
  size_t size() const noexcept;
  size_t getsize() const noexcept;
  size_t capacity() const noexcept;
  size_t getcapacity() const noexcept;

  void add(const Key& key, const Value& value);
  void add(const Key& key, Value&& value);
  void erase(const Key& key);
  Value drop(const Key& key);
  bool contains(const Key& key) const;
  bool has(const Key& key) const;

  Value* find(const Key& key);
  const Value* find(const Key& key) const;

  iterator findIter(const Key& key);
  const_iterator findIter(const Key& key) const;

  void rehash(size_t slots);

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

private:
  friend class HashTableIterator< Key, Value, Hash, Equal >;
  friend class HashTableConstIterator< Key, Value, Hash, Equal >;

  enum class BucketState
  {
    Empty,
    Occupied,
    Deleted
  };

  struct Bucket
  {
    BucketState state;
    Key key;
    Value value;

    Bucket():
      state(BucketState::Empty),
      key(),
      value()
    {}
  };

  Vector< Bucket > buckets_;
  size_t size_;
  Hash hash_;
  Equal equal_;

  static constexpr size_t npos = static_cast< size_t >(-1);

  size_t findIndex(const Key& key) const;
  size_t findPlace(const Key& key, bool& found) const;
  size_t nextIndex(size_t index) const noexcept;
};
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  buckets_(slots, Bucket()),
  size_(0),
  hash_(),
  equal_()
{}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& rhs):
  buckets_(rhs.buckets_),
  size_(rhs.size_),
  hash_(rhs.hash_),
  equal_(rhs.equal_)
{}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& rhs) noexcept:
  buckets_(std::move(rhs.buckets_)),
  size_(rhs.size_),
  hash_(std::move(rhs.hash_)),
  equal_(std::move(rhs.equal_))
{
  rhs.size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTable< Key, Value, Hash, Equal >&
ulanova::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& rhs)
{
  if (this != &rhs)
  {
    HashTable tmp(rhs);
    swap(tmp);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTable< Key, Value, Hash, Equal >&
ulanova::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& rhs) noexcept
{
  if (this != &rhs)
  {
    swap(rhs);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTable< Key, Value, Hash, Equal >::swap(HashTable& rhs) noexcept
{
  buckets_.swap(rhs.buckets_);
  std::swap(size_, rhs.size_);
  std::swap(hash_, rhs.hash_);
  std::swap(equal_, rhs.equal_);
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTable< Key, Value, Hash, Equal >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::getsize() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return buckets_.getsize();
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::getcapacity() const noexcept
{
  return buckets_.getsize();
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  if (size_ * 2 >= buckets_.getsize())
  {
    rehash(buckets_.getsize() * 2);
  }
  bool found = false;
  size_t index = findPlace(key, found);
  if (!found)
  {
    buckets_[index].state = BucketState::Occupied;
    buckets_[index].key = key;
    buckets_[index].value = value;
    ++size_;
  }
  else
  {
    buckets_[index].value = value;
  }
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTable< Key, Value, Hash, Equal >::add(const Key& key, Value&& value)
{
  if (size_ * 2 >= buckets_.getsize())
  {
    rehash(buckets_.getsize() * 2);
  }
  bool found = false;
  size_t index = findPlace(key, found);
  if (!found)
  {
    buckets_[index].state = BucketState::Occupied;
    buckets_[index].key = key;
    buckets_[index].value = std::move(value);
    ++size_;
  }
  else
  {
    buckets_[index].value = std::move(value);
  }
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    throw std::out_of_range("key not found");
  }
  buckets_[index].state = BucketState::Deleted;
  --size_;
}

template< class Key, class Value, class Hash, class Equal >
Value ulanova::HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    throw std::out_of_range("key not found");
  }
  Value result = std::move(buckets_[index].value);
  buckets_[index].state = BucketState::Deleted;
  --size_;
  return result;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTable< Key, Value, Hash, Equal >::contains(const Key& key) const
{
  return findIndex(key) != npos;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
{
  return findIndex(key) != npos;
}

template< class Key, class Value, class Hash, class Equal >
Value* ulanova::HashTable< Key, Value, Hash, Equal >::find(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return nullptr;
  }
  return &buckets_[index].value;
}

template< class Key, class Value, class Hash, class Equal >
const Value* ulanova::HashTable< Key, Value, Hash, Equal >::find(const Key& key) const
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return nullptr;
  }
  return &buckets_[index].value;
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::iterator
ulanova::HashTable< Key, Value, Hash, Equal >::findIter(const Key& key)
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return end();
  }
  return iterator(this, index);
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::const_iterator
ulanova::HashTable< Key, Value, Hash, Equal >::findIter(const Key& key) const
{
  size_t index = findIndex(key);
  if (index == npos)
  {
    return cend();
  }
  return const_iterator(this, index);
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots < size_ * 2)
  {
    throw std::invalid_argument("new capacity is too small");
  }
  HashTable tmp(slots);
  tmp.hash_ = hash_;
  tmp.equal_ = equal_;
  for (size_t i = 0; i < buckets_.getsize(); ++i)
  {
    if (buckets_[i].state == BucketState::Occupied)
    {
      tmp.add(buckets_[i].key, std::move(buckets_[i].value));
    }
  }
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::iterator
ulanova::HashTable< Key, Value, Hash, Equal >::begin() noexcept
{
  return iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::iterator
ulanova::HashTable< Key, Value, Hash, Equal >::end() noexcept
{
  return iterator(this, buckets_.getsize());
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::const_iterator
ulanova::HashTable< Key, Value, Hash, Equal >::begin() const noexcept
{
  return const_iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::const_iterator
ulanova::HashTable< Key, Value, Hash, Equal >::end() const noexcept
{
  return const_iterator(this, buckets_.getsize());
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::const_iterator
ulanova::HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return const_iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename ulanova::HashTable< Key, Value, Hash, Equal >::const_iterator
ulanova::HashTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return const_iterator(this, buckets_.getsize());
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::findIndex(const Key& key) const
{
  if (buckets_.getsize() == 0)
  {
    return npos;
  }
  size_t index = hash_(key) % buckets_.getsize();
  size_t start = index;
  do
  {
    if (buckets_[index].state == BucketState::Empty)
    {
      return npos;
    }
    if (buckets_[index].state == BucketState::Occupied && equal_(buckets_[index].key, key))
    {
      return index;
    }
    index = nextIndex(index);
  }
  while (index != start);
  return npos;
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::findPlace(const Key& key, bool& found) const
{
  size_t index = hash_(key) % buckets_.getsize();
  size_t start = index;
  size_t firstDeleted = npos;
  found = false;
  do
  {
    if (buckets_[index].state == BucketState::Empty)
    {
      return firstDeleted != npos ? firstDeleted : index;
    }
    if (buckets_[index].state == BucketState::Deleted && firstDeleted == npos)
    {
      firstDeleted = index;
    }
    if (buckets_[index].state == BucketState::Occupied && equal_(buckets_[index].key, key))
    {
      found = true;
      return index;
    }
    index = nextIndex(index);
  }
  while (index != start);
  return firstDeleted != npos ? firstDeleted : npos;
}

template< class Key, class Value, class Hash, class Equal >
size_t ulanova::HashTable< Key, Value, Hash, Equal >::nextIndex(size_t index) const noexcept
{
  return (index + 1) % buckets_.getsize();
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator():
  table_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(Table* table, size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableIterator< Key, Value, Hash, Equal >&
ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableIterator< Key, Value, Hash, Equal >
ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator++(int)
{
  HashTableIterator tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator==(
  const HashTableIterator& rhs) const noexcept
{
  return table_ == rhs.table_ && index_ == rhs.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator!=(
  const HashTableIterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key&, Value& >
ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator*() const
{
  auto& bucket = table_->buckets_[index_];
  return { bucket.key, bucket.value };
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key*, Value* >
ulanova::HashTableIterator< Key, Value, Hash, Equal >::operator->() const
{
  auto& bucket = table_->buckets_[index_];
  return { &bucket.key, &bucket.value };
}

template< class Key, class Value, class Hash, class Equal >
const Key& ulanova::HashTableIterator< Key, Value, Hash, Equal >::key() const
{
  return table_->buckets_[index_].key;
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTableIterator< Key, Value, Hash, Equal >::skipEmpty()
{
  using BucketState = typename Table::BucketState;
  while (index_ < table_->buckets_.getsize() &&
    table_->buckets_[index_].state != BucketState::Occupied)
  {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator():
  table_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
  const HashTableIterator< Key, Value, Hash, Equal >& it):
  table_(it.table_),
  index_(it.index_)
{}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
  const Table* table, size_t index):
  table_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >&
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator++(int)
{
  HashTableConstIterator tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator==(
  const HashTableConstIterator& rhs) const noexcept
{
  return table_ == rhs.table_ && index_ == rhs.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator!=(
  const HashTableConstIterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key&, const Value& >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator*() const
{
  const auto& bucket = table_->buckets_[index_];
  return { bucket.key, bucket.value };
}

template< class Key, class Value, class Hash, class Equal >
std::pair< const Key*, const Value* >
ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::operator->() const
{
  const auto& bucket = table_->buckets_[index_];
  return { &bucket.key, &bucket.value };
}

template< class Key, class Value, class Hash, class Equal >
const Key& ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::key() const
{
  return table_->buckets_[index_].key;
}

template< class Key, class Value, class Hash, class Equal >
void ulanova::HashTableConstIterator< Key, Value, Hash, Equal >::skipEmpty()
{
  using BucketState = typename Table::BucketState;
  while (index_ < table_->buckets_.getsize() &&
    table_->buckets_[index_].state != BucketState::Occupied)
  {
    ++index_;
  }
}

#endif
