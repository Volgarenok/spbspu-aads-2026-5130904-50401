#ifndef MYHASH_HPP
#define MYHASH_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "../common/list.hpp"
#include "../common/myVector.hpp"

namespace zhuravleva
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    private:
      size_t size_;//кол-во пар (key, value)
      Hash hasher;
      Equal equal;
      myVector< List< std::pair<Key, Value> > > table;
      size_t getIndex(const Key& key) const;
    public:
      class Iterator;
      class ConstIterator;
      Iterator begin();
      Iterator end();
      ConstIterator begin() const;
      ConstIterator end() const;
      ConstIterator cbegin() const;
      ConstIterator cend() const;

      HashTable(size_t bucket_count = 8);
      size_t size() const noexcept;
      size_t bucket_count() const noexcept;
      void add(const Key& key, const Value& value);
      bool has(const Key& key) const;
      bool drop(const Key& key);
      Value& get(const Key& key);
      const Value& get(const Key& key) const;
      void rehash(size_t newSize);
      bool empty() const noexcept;
      void clear();
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::Iterator
  {
    private:
      HashTable< Key, Value, Hash, Equal >* hashTable;
      size_t bucketIndex;
      LIter< std::pair< Key, Value > > listIter;
      void skipEmpty();
    public:
      Iterator();
      Iterator(HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index, LIter< std::pair< Key, Value > > iter);
      std::pair< Key, Value >& operator*() const;
      std::pair< Key, Value >* operator->() const;
      Iterator& operator++();
      Iterator operator++(int);
      bool operator==(const Iterator& other) const noexcept;
      bool operator!=(const Iterator& other) const noexcept;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::ConstIterator
  {
    private:
      const HashTable< Key, Value, Hash, Equal >* hashTable;
      size_t bucketIndex;
      LCIter< std::pair< Key, Value > > listIter;
      void skipEmpty();
    public:
      ConstIterator();
      ConstIterator(const HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index, LCIter< std::pair< Key, Value > > iter);
      const std::pair< Key, Value >& operator*() const;
      const std::pair< Key, Value >* operator->() const;
      ConstIterator& operator++();
      ConstIterator operator++(int);
      bool operator==(const ConstIterator& other) const noexcept;
      bool operator!=(const ConstIterator& other) const noexcept;
  };
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::begin()
{
  if (table.empty())
  {
    return end();
  }
  return Iterator(this, 0, table[0].begin());
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(this, table.size(), LIter< std::pair< Key, Value > >());
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::begin() const
{
  if (table.empty())
  {
    return end();
  }
  return ConstIterator(this, 0, table[0].cbegin());
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::end() const
{
  return ConstIterator(this, table.size(), LCIter< std::pair< Key, Value > >());
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return begin();
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return end();
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator():
  hashTable(nullptr),
  bucketIndex(0),
  listIter()
{}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator(
    HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index,
    LIter< std::pair< Key, Value > > iter):
  hashTable(tablePtr),
  bucketIndex(index),
  listIter(iter)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::skipEmpty()
{
  if (!hashTable)
  {
    return;
  }

  while (bucketIndex < hashTable->table.size() &&
      listIter == hashTable->table[bucketIndex].end())
  {
    bucketIndex++;

    if (bucketIndex < hashTable->table.size())
    {
      listIter = hashTable->table[bucketIndex].begin();
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >&
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator*() const
{
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    throw std::runtime_error("invalid iterator");
  }
  return *listIter;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >*
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator->() const
{
  return &(**this);
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator&
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator++()
{
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    return *this;
  }
  ++listIter;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator++(int)
{
  Iterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator==(
    const Iterator& other) const noexcept
{
  if (hashTable != other.hashTable)
  {
    return false;
  }
  if (bucketIndex != other.bucketIndex)
  {
    return false;
  }
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    return true;
  }
  return listIter == other.listIter;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::Iterator::operator!=(
    const Iterator& other) const noexcept
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator():
  hashTable(nullptr),
  bucketIndex(0),
  listIter()
{}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator(
    const HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index,
    LCIter< std::pair< Key, Value > > iter):
  hashTable(tablePtr),
  bucketIndex(index),
  listIter(iter)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::skipEmpty()
{
  if (!hashTable)
  {
    return;
  }
  while (bucketIndex < hashTable->table.size() &&
      listIter == hashTable->table[bucketIndex].cend())
  {
    bucketIndex++;
    if (bucketIndex < hashTable->table.size())
    {
      listIter = hashTable->table[bucketIndex].cbegin();
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >&
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator*() const
{
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    throw std::runtime_error("invalid iterator");
  }
  return *listIter;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >*
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator->() const
{
  return &(**this);
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator&
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++()
{
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    return *this;
  }
  ++listIter;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator
zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++(int)
{
  ConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator==(
    const ConstIterator& other) const noexcept
{
  if (hashTable != other.hashTable)
  {
    return false;
  }
  if (bucketIndex != other.bucketIndex)
  {
    return false;
  }
  if (!hashTable || bucketIndex >= hashTable->table.size())
  {
    return true;
  }
  return listIter == other.listIter;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator!=(
    const ConstIterator& other) const noexcept
{
  return !(*this == other);
}

template <class Key, class Value, class Hash, class Equal>
zhuravleva::HashTable<Key, Value, Hash, Equal>::HashTable(size_t bucket_count):
size_(0),
hasher(),
equal(),
table(bucket_count)
{
  if (bucket_count == 0)
  {
    throw std::invalid_argument("bucket count is zero");
  }
}

template < class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
{
  return table.size();
}

template < class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  size_t idx = getIndex(key);
  auto& bucket = table[idx];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (equal(it->first, key))
    {
        it->second = value;
        return;
    }
  }
  bucket.addEnd(std::make_pair(key, value));
  size_++;
}

template < class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
{
  size_t idx = getIndex(key);
  const auto& bucket = table[idx];
  for (auto it = bucket.cbegin(); it != bucket.cend(); it++)
  {
    if (equal(it->first, key))
    {
        return true;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t idx = getIndex(key);
  auto& bucket = table[idx];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (equal(it -> first, key))
    {
      bucket.erase(it);
      size_--;
      return true;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::get(const Key& key)
{
  size_t idx = getIndex(key);
  auto& bucket = table[idx];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (equal(it -> first, key))
    {
      return it -> second;
    }
  }
  throw std::runtime_error("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
const Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
{
  size_t idx = getIndex(key);
  const auto& bucket = table[idx];
  for (auto it = bucket.cbegin(); it != bucket.cend(); it++)
  {
    if (equal(it -> first, key))
    {
      return it -> second;
    }
  }
  throw std::runtime_error("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::rehash(size_t newSize)
{
  if (newSize == 0)
  {
    throw std::invalid_argument("new size is zero");
  }
  myVector< List< std::pair<Key, Value> > > newTable(newSize);
  for (size_t i = 0; i < table.size(); i++)
  {
    List<std::pair<Key, Value>>& bucket = table[i];
    for (auto it = bucket.begin(); it != bucket.end(); it++)
    {
      size_t newindex = hasher(it->first) % newSize;
      newTable[newindex].addEnd(*it);
    }
  }

  table.swap(newTable);
}

template < class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::getIndex(const Key& key) const
{
  if (table.empty())
  {
    throw std::runtime_error("hash table is empty");
  }
  return hasher(key) % table.size();
}

template < class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template < class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::clear()
{
  for (size_t i = 0; i < table.size(); i++)
  {
    auto& bucket = table[i];
    bucket.clear();
  }
  size_ = 0;
}

#endif
