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
      Key key = it->first;
      Value value = it->second;
      size_t newindex = hasher(key) % newSize; 
      newTable[newindex].addEnd(std::make_pair(key, value));
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
