#ifndef HASH_TABLE
#define HASH_TABLE

#include <vector.hpp>

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <memory>

namespace studilova
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class Iterator
  {
    public:
      Iterator();

      std::pair< Key, Value >& operator*();
      std::pair< Key, Value >* operator->();

      Iterator& operator++();
      Iterator operator++(int);

      bool operator==(const Iterator& other) const;
      bool operator!=(const Iterator& other) const;

    private:
      HashTable< Key, Value, Hash, Equal >* hashTable_;
      size_t index_;

      Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t index);

      void skipEmpty();

      friend class HashTable< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class ConstIterator
  {
    public:
      ConstIterator();
      ConstIterator(const Iterator< Key, Value, Hash, Equal >& other);

      const std::pair< Key, Value >& operator*() const;
      const std::pair< Key, Value >* operator->() const;

      ConstIterator& operator++();
      ConstIterator operator++(int);

      bool operator==(const ConstIterator& other) const;
      bool operator!=(const ConstIterator& other) const;

    private:
      const HashTable< Key, Value, Hash, Equal >* hashTable_;
      size_t index_;

      ConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index);

      void skipEmpty();

      friend class HashTable< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    public:
      using It = Iterator< Key, Value, Hash, Equal >;
      using CIt = ConstIterator< Key, Value, Hash, Equal >;

      explicit HashTable(size_t capacity = 16, Hash hash = Hash{}, Equal equal = Equal{});

      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool empty() const noexcept;

      bool contains(const Key& key) const noexcept;

      It find(const Key& key) noexcept;
      CIt find(const Key& key) const noexcept;

      std::pair< It, bool > insert(const std::pair< Key, Value >& value);
      std::pair< It, bool > insert(std::pair< Key, Value >&& value);

      Value& at(const Key& key);
      const Value& at(const Key& key) const;

      size_t erase(const Key& key) noexcept;

      void swap(HashTable& other) noexcept;
      void rehash(size_t newCapacity);

      It begin();
      It end();

      CIt begin() const;
      CIt end() const;
      CIt cbegin() const;
      CIt cend() const;

    private:
      enum class State
      {
        EMPTY,
        OCCUPIED,
        TOMBSTONE
      };

      struct Entry
      {
        std::pair< Key, Value > data;
        State state;

        Entry();
      };

      studilova::Vector< Entry > table_;
      size_t size_;
      Hash hash_;
      Equal equal_;

      size_t probeIndex(const Key& key, size_t attempt) const;
      bool findEntry(const Key& key, size_t& outIndex) const;
      bool findPlace(const Key& key, size_t& outIndex) const;

      template< class T >
      std::pair< It, bool > insertImpl(T&& value);

      friend class Iterator< Key, Value, Hash, Equal >;
      friend class ConstIterator< Key, Value, Hash, Equal >;
  };
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::Entry::Entry():
  data(),
  state(State::EMPTY)
{}

template< class Key, class Value, class Hash, class Equal >
studilova::Iterator< Key, Value, Hash, Equal >::Iterator():
  hashTable_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
void studilova::Iterator< Key, Value, Hash, Equal >::skipEmpty()
{
  while (hashTable_ && index_ < hashTable_->table_.getSize() && hashTable_->table_[index_].state != HashTable< Key, Value, Hash, Equal >::State::OCCUPIED)
  {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
studilova::Iterator< Key, Value, Hash, Equal >::Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t index):
  hashTable_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >& studilova::Iterator< Key, Value, Hash, Equal >::operator*()
{
  typename HashTable< Key, Value, Hash, Equal >::Entry& entry = hashTable_->table_[index_];
  return entry.data;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >* studilova::Iterator< Key, Value, Hash, Equal >::operator->()
{
  return std::addressof(operator*());
}

template< class Key, class Value, class Hash, class Equal >
studilova::Iterator< Key, Value, Hash, Equal >& studilova::Iterator< Key, Value, Hash, Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
studilova::Iterator< Key, Value, Hash, Equal > studilova::Iterator< Key, Value, Hash, Equal >::operator++(int)
{
  Iterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::Iterator< Key, Value, Hash, Equal >::operator==(const Iterator& other) const
{
  return hashTable_ == other.hashTable_ && index_ == other.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::Iterator< Key, Value, Hash, Equal >::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
studilova::ConstIterator< Key, Value, Hash, Equal >::ConstIterator():
  hashTable_(nullptr),
  index_(0)
{}

template< class Key, class Value, class Hash, class Equal >
studilova::ConstIterator< Key, Value, Hash, Equal >::ConstIterator(const Iterator< Key, Value, Hash, Equal >& other):
  hashTable_(other.hashTable_),
  index_(other.index_)
{}

template< class Key, class Value, class Hash, class Equal >
void studilova::ConstIterator< Key, Value, Hash, Equal >::skipEmpty()
{
  while (hashTable_ && index_ < hashTable_->table_.getSize() && hashTable_->table_[index_].state != HashTable< Key, Value, Hash, Equal >::State::OCCUPIED)
  {
    ++index_;
  }
}

template< class Key, class Value, class Hash, class Equal >
studilova::ConstIterator< Key, Value, Hash, Equal >::ConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index):
  hashTable_(table),
  index_(index)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >& studilova::ConstIterator< Key, Value, Hash, Equal >::operator*() const
{
  const typename studilova::HashTable< Key, Value, Hash, Equal >::Entry& entry = hashTable_->table_[index_];
  return entry.data;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >* studilova::ConstIterator< Key, Value, Hash, Equal >::operator->() const
{
  return std::addressof(operator*());
}

template< class Key, class Value, class Hash, class Equal >
studilova::ConstIterator< Key, Value, Hash, Equal >& studilova::ConstIterator< Key, Value, Hash, Equal >::operator++()
{
  ++index_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
studilova::ConstIterator< Key, Value, Hash, Equal > studilova::ConstIterator< Key, Value, Hash, Equal >::operator++(int)
{
  ConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::ConstIterator< Key, Value, Hash, Equal >::operator==(const ConstIterator& other) const
{
  return hashTable_ == other.hashTable_ && index_ == other.index_;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::ConstIterator< Key, Value, Hash, Equal >::operator!=(const ConstIterator& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
studilova::HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity, Hash hash, Equal equal):
  table_(capacity, Entry{}),
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
    if (table_[index].state == State::OCCUPIED && equal_(table_[index].data.first, key))
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
bool studilova::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
bool studilova::HashTable< Key, Value, Hash, Equal >::contains(const Key& key) const noexcept
{
  return find(key) != cend();
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::It
  studilova::HashTable< Key, Value, Hash, Equal >::find(const Key& key) noexcept
{
  size_t index = 0;

  if (!findEntry(key, index))
  {
    return end();
  }

  return It(this, index);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::CIt
  studilova::HashTable< Key, Value, Hash, Equal >::find(const Key& key) const noexcept
{
  size_t index = 0;

  if (!findEntry(key, index))
  {
    return cend();
  }

  return CIt(this, index);
}

template< class Key, class Value, class Hash, class Equal >
template< class T >
std::pair< typename studilova::HashTable< Key, Value, Hash, Equal >::It, bool >
  studilova::HashTable< Key, Value, Hash, Equal >::insertImpl(T&& value)
{
  size_t index = 0;

  if (findEntry(value.first, index))
  {
    table_[index].data.second = std::forward< T >(value).second;
    return std::pair< It, bool >(It(this, index), false);
  }

  if (!findPlace(value.first, index))
  {
    throw std::overflow_error("HashTable is full");
  }

  table_[index].data = std::forward< T >(value);
  table_[index].state = State::OCCUPIED;
  ++size_;

  return std::pair< It, bool >(It(this, index), true);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< typename studilova::HashTable< Key, Value, Hash, Equal >::It, bool >
  studilova::HashTable< Key, Value, Hash, Equal >::insert(const std::pair< Key, Value >& value)
{
  return insertImpl(value);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< typename studilova::HashTable< Key, Value, Hash, Equal >::It, bool >
  studilova::HashTable< Key, Value, Hash, Equal >::insert(std::pair< Key, Value >&& value)
{
  return insertImpl(std::forward< std::pair< Key, Value > >(value));
}

template< class Key, class Value, class Hash, class Equal >
Value& studilova::HashTable< Key, Value, Hash, Equal >::at(const Key& key)
{
  It it = find(key);
  if (it == end())
  {
    throw std::out_of_range("Key not found");
  }
  return it->second;
}

template< class Key, class Value, class Hash, class Equal >
const Value& studilova::HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
{
  CIt it = find(key);
  if (it == cend())
  {
    throw std::out_of_range("Key not found");
  }
  return it->second;
}

template< class Key, class Value, class Hash, class Equal >
size_t studilova::HashTable< Key, Value, Hash, Equal >::erase(const Key& key) noexcept
{
  size_t index = 0;
  if (!findEntry(key, index))
  {
    return 0;
  }

  table_[index].state = State::TOMBSTONE;
  --size_;
  return 1;
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
      tmp.insert(table_[i].data);
    }
  }
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::It
  studilova::HashTable< Key, Value, Hash, Equal >::begin()
{
  return It(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::It
  studilova::HashTable< Key, Value, Hash, Equal >::end()
{
  return It(this, table_.getSize());
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::CIt
  studilova::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return CIt(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::CIt
  studilova::HashTable< Key, Value, Hash, Equal >::end() const
{
  return CIt(this, table_.getSize());
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::CIt
  studilova::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return begin();
}

template< class Key, class Value, class Hash, class Equal >
typename studilova::HashTable< Key, Value, Hash, Equal >::CIt
  studilova::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return end();
}

#endif
