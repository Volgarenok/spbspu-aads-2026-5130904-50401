#ifndef HASH_TABLE
#define HASH_TABLE

#include "../common/vector.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace studilova
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    enum class State
    {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };

    struct Entry
    {
      Key key;
      Value value;
      State state;

      Entry();
    };

    public:
      class Iterator
      {
        public:
          Iterator();

          std::pair< Key&, Value& > operator*() const;

          Iterator& operator++();

          bool operator==(const Iterator& other) const;
          bool operator!=(const Iterator& other) const;

        private:
          friend class HashTable;

          HashTable* hashTable_;
          size_t index_;

          void skipEmpty();
          Iterator(HashTable* table, size_t index);
      };

      explicit HashTable
      (
        size_t capacity = 16,
        Hash hash = Hash{},
        Equal equal = Equal{}
      );

      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool isEmpty() const noexcept;

      bool has(const Key& key) const;

      void add(const Key& key, const Value& value);

      Value& get(const Key& key);
      const Value& get(const Key& key) const;

      bool erase(const Key& key);
      Value drop(const Key& key);

      void swap(HashTable& other) noexcept;
      void rehash(size_t newCapacity);

      Iterator begin();
      Iterator end();

    private:
      topit::Vector< Entry > table_;
      size_t size_;
      Hash hash_;
      Equal equal_;

      size_t probeIndex(const Key& key, size_t attempt) const;
      bool findEntry(const Key& key, size_t& outIndex) const;
      bool findPlace(const Key& key, size_t& outIndex) const;
  };
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::Entry::Entry() :
  key(),
  value(),
  state(State::EMPTY)
{}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator():
  hashTable_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
void studilova::HashTable< Key, Value, Hash, Equal >::Iterator::skipEmpty()
{
  while (hashTable_ && index_ < hashTable_->table_.getSize() && hashTable_->table_[index_].state != State::OCCUPIED)
  {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator(HashTable* table, size_t index) :
  hashTable_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key&, Value& > studilova::HashTable< Key, Value, Hash, Equal >::Iterator::operator*() const
{
  Entry& entry = hashTable_->table_[index_];
  return std::pair< Key&, Value& >(entry.key, entry.value);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::Iterator&
studilova::HashTable< Key, Value, Hash, Equal >::Iterator::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::Iterator::operator==(const Iterator& other) const
{
  return hashTable_ == other.hashTable_ && index_ == other.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::HashTable(
  size_t capacity,
  Hash hash,
  Equal equal
):
  table_(capacity),
  size_(0),
  hash_(std::move(hash)),
  equal_(std::move(equal))
{
  if (capacity == 0)
  {
    throw std::invalid_argument("HashTable capacity must be positive");
  }
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::probeIndex(const Key& key, size_t attempt) const
{
  return (hash_(key) + attempt * attempt) % table_.getSize();
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::findEntry(const Key& key, size_t& outIndex) const
{
  for (size_t attempt = 0; attempt < table_.getSize(); ++attempt)
  {
    size_t index = probeIndex(key, attempt);
    if (table_[index].state == State::EMPTY)
    {
      return false;
    }
    if (table_[index].state == State::OCCUPIED && equal_(table_[index].key, key))
    {
      outIndex = index;
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::findPlace(const Key& key, size_t& outIndex) const
{
  bool hasTombstone = false;
  size_t tombstoneIndex = 0;

  for (size_t attempt = 0; attempt < table_.getSize(); ++attempt)
  {
    size_t index = probeIndex(key, attempt);
    if (table_[index].state == State::TOMBSTONE && !hasTombstone)
    {
      hasTombstone = true;
      tombstoneIndex = index;
    }

    if (table_[index].state == State::EMPTY)
    {
      outIndex = hasTombstone ? tombstoneIndex : index;
      return true;
    }
  }

  if (hasTombstone)
  {
    outIndex = tombstoneIndex;
    return true;
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return table_.getSize();
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
{
  size_t dummy = 0;
  return findEntry(key, dummy);
}

template< class Key, class Value, class Hash, class Equal >
void studilova::HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  size_t index = 0;
  if (findEntry(key, index))
  {
    table_[index].value = value;
    return;
  }

  if (!findPlace(key, index))
  {
    throw std::overflow_error("HashTable is full");
  }

  table_[index].key = key;
  table_[index].value = value;
  table_[index].state = State::OCCUPIED;
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
Value& studilova::HashTable< Key, Value, Hash, Equal >::get(const Key& key)
{
  size_t index = 0;
  if (!findEntry(key, index))
  {
    throw std::out_of_range("Key not found");
  }
  return table_[index].value;
}

template< class Key, class Value, class Hash, class Equal >
const Value& studilova::HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
{
  size_t index = 0;
  if (!findEntry(key, index))
  {
    throw std::out_of_range("Key not found");
  }
  return table_[index].value;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::erase(const Key& key)
{
  size_t index = 0;
  if (!findEntry(key, index))
  {
    return false;
  }

  table_[index].state = State::TOMBSTONE;
  --size_;
  return true;
}

template< class Key, class Value, class Hash, class Equal >
Value studilova::HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t index = 0;
  if (!findEntry(key, index))
  {
    throw std::out_of_range("Key not found");
  }

  Value result = table_[index].value;
  table_[index].state = State::TOMBSTONE;
  --size_;
  return result;
}

template< class Key, class Value, class Hash, class Equal >
void studilova::HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
{
  table_.swap(other.table_);
  std::swap(size_, other.size_);
  std::swap(hash_, other.hash_);
  std::swap(equal_, other.equal_);
}

template< class Key, class Value, class Hash, class Equal >
void studilova::HashTable< Key, Value, Hash, Equal >::rehash(size_t newCapacity)
{
  if (newCapacity == 0)
  {
    throw std::invalid_argument("HashTable capacity must be positive");
  }

  HashTable tmp(newCapacity, hash_, equal_);
  for (size_t i = 0; i < table_.getSize(); ++i)
  {
    if (table_[i].state == State::OCCUPIED)
    {
      tmp.add(table_[i].key, table_[i].value);
    }
  }
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::Iterator
studilova::HashTable< Key, Value, Hash, Equal >::begin()
{
  return Iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::Iterator
studilova::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(this, table_.getSize());
}

#endif
