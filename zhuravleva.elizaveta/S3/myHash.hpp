#ifndef MYHASH_HPP
#define MYHASH_HPP
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>
#include "list.hpp"
#include "myVector.hpp"

namespace zhuravleva
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HIter;

  template< class Key, class Value, class Hash, class Equal >
  class HCIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    public:
      using Iterator = HIter< Key, Value, Hash, Equal >;
      using ConstIterator = HCIter< Key, Value, Hash, Equal >;
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
      void add(const Key& key, Value&& value);
      bool has(const Key& key) const;
      bool drop(const Key& key);
      Value& get(const Key& key);
      const Value& get(const Key& key) const;
      Value& at(const Key& key);
      const Value& at(const Key& key) const;
      Value& operator[](const Key& key);
      void rehash(size_t newSize);
      bool empty() const noexcept;
      void clear();

    private:
      size_t size_;
      Hash hasher_;
      Equal equal_;
      myVector< List< std::pair< Key, Value > > > table_;
      size_t getIndex(const Key& key) const;
      friend class HIter< Key, Value, Hash, Equal >;
      friend class HCIter< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HIter
  {
    public:
      HIter();
      std::pair< Key, Value >& operator*();
      std::pair< Key, Value >* operator->();
      HIter& operator++();
      HIter operator++(int);
      bool operator==(const HIter& other) const noexcept;
      bool operator!=(const HIter& other) const noexcept;

    private:
      HashTable< Key, Value, Hash, Equal >* hashTable_;
      size_t bucketIndex_;
      LIter< std::pair< Key, Value > > listIter_;
      void skipEmpty();
      HIter(HashTable< Key, Value, Hash, Equal >* tablePtr,
          size_t index, LIter< std::pair< Key, Value > > iter);
      friend class HashTable< Key, Value, Hash, Equal >;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HCIter
  {
    public:
      HCIter();
      const std::pair< Key, Value >& operator*() const;
      const std::pair< Key, Value >* operator->() const;
      HCIter& operator++();
      HCIter operator++(int);
      bool operator==(const HCIter& other) const noexcept;
      bool operator!=(const HCIter& other) const noexcept;

    private:
      const HashTable< Key, Value, Hash, Equal >* hashTable_;
      size_t bucketIndex_;
      LCIter< std::pair< Key, Value > > listIter_;
      void skipEmpty();
      HCIter(const HashTable< Key, Value, Hash, Equal >* tablePtr,
          size_t index, LCIter< std::pair< Key, Value > > iter);
      friend class HashTable< Key, Value, Hash, Equal >;
  };
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >
    zhuravleva::HashTable< Key, Value, Hash, Equal >::begin()
{
  if (table_.empty())
  {
    return end();
  }
  return HIter(this, 0, table_[0].begin());
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >
    zhuravleva::HashTable< Key, Value, Hash, Equal >::end()
{
  return HIter(this, table_.size(), LIter< std::pair< Key, Value > >());
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >
    zhuravleva::HashTable< Key, Value, Hash, Equal >::begin() const
{
  if (table_.empty())
  {
    return end();
  }
  return HCIter(this, 0, table_[0].cbegin());
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >
    zhuravleva::HashTable< Key, Value, Hash, Equal >::end() const
{
  return HCIter(this, table_.size(), LCIter< std::pair< Key, Value > >());
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
    zhuravleva::HIter< Key, Value, Hash, Equal >::HIter():
  hashTable_(nullptr),
  bucketIndex_(0),
  listIter_()
{}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >::HIter(
    HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index,
    LIter< std::pair< Key, Value > > iter):
  hashTable_(tablePtr),
  bucketIndex_(index),
  listIter_(iter)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HIter< Key, Value, Hash, Equal >::skipEmpty()
{
  if (!hashTable_)
  {
    return;
  }

  while (bucketIndex_ < hashTable_->table_.size() &&
      listIter_ == hashTable_->table_[bucketIndex_].end())
  {
    bucketIndex_++;

    if (bucketIndex_ < hashTable_->table_.size())
    {
      listIter_ = hashTable_->table_[bucketIndex_].begin();
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >&
zhuravleva::HIter< Key, Value, Hash, Equal >::operator*()
{
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    throw std::runtime_error("invalid iterator");
  }
  return *listIter_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >*
zhuravleva::HIter< Key, Value, Hash, Equal >::operator->()
{
  return std::addressof(**this);
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >&
zhuravleva::HIter< Key, Value, Hash, Equal >::operator++()
{
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    return *this;
  }
  ++listIter_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >
zhuravleva::HIter< Key, Value, Hash, Equal >::operator++(int)
{
  HIter temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HIter< Key, Value, Hash, Equal >::operator==(
    const HIter& other) const noexcept
{
  if (hashTable_ != other.hashTable_)
  {
    return false;
  }
  if (bucketIndex_ != other.bucketIndex_)
  {
    return false;
  }
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    return true;
  }
  return listIter_ == other.listIter_;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HIter< Key, Value, Hash, Equal >::operator!=(
    const HIter& other) const noexcept
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >::HCIter():
  hashTable_(nullptr),
  bucketIndex_(0),
  listIter_()
{}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >::HCIter(
    const HashTable< Key, Value, Hash, Equal >* tablePtr, size_t index,
    LCIter< std::pair< Key, Value > > iter):
  hashTable_(tablePtr),
  bucketIndex_(index),
  listIter_(iter)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HCIter< Key, Value, Hash, Equal >::skipEmpty()
{
  if (!hashTable_)
  {
    return;
  }
  while (bucketIndex_ < hashTable_->table_.size() &&
      listIter_ == hashTable_->table_[bucketIndex_].cend())
  {
    bucketIndex_++;
    if (bucketIndex_ < hashTable_->table_.size())
    {
      listIter_ = hashTable_->table_[bucketIndex_].cbegin();
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >&
zhuravleva::HCIter< Key, Value, Hash, Equal >::operator*() const
{
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    throw std::runtime_error("invalid iterator");
  }
  return *listIter_;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >*
zhuravleva::HCIter< Key, Value, Hash, Equal >::operator->() const
{
  return std::addressof(**this);
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >&
zhuravleva::HCIter< Key, Value, Hash, Equal >::operator++()
{
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    return *this;
  }
  ++listIter_;
  skipEmpty();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >
zhuravleva::HCIter< Key, Value, Hash, Equal >::operator++(int)
{
  HCIter temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HCIter< Key, Value, Hash, Equal >::operator==(
    const HCIter& other) const noexcept
{
  if (hashTable_ != other.hashTable_)
  {
    return false;
  }
  if (bucketIndex_ != other.bucketIndex_)
  {
    return false;
  }
  if (!hashTable_ || bucketIndex_ >= hashTable_->table_.size())
  {
    return true;
  }
  return listIter_ == other.listIter_;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HCIter< Key, Value, Hash, Equal >::operator!=(
    const HCIter& other) const noexcept
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
zhuravleva::HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucket_count):
  size_(0),
  hasher_(),
  equal_(),
  table_(bucket_count)
{
  if (bucket_count == 0)
  {
    throw std::invalid_argument("bucket count is zero");
  }
}

template< class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
{
  return table_.size();
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  size_t idx = getIndex(key);
  List< std::pair< Key, Value > >& bucket = table_[idx];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (equal_(it->first, key))
    {
      it->second = value;
      return;
    }
  }
  bucket.pushBack(std::make_pair(key, value));
  size_++;
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::add(const Key& key, Value&& value)
{
  size_t idx = getIndex(key);
  List< std::pair< Key, Value > >& bucket = table_[idx];
  for (LIter< std::pair< Key, Value > > it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_(it->first, key))
    {
      it->second = std::move(value);
      return;
    }
  }
  bucket.pushBack(std::make_pair(key, std::move(value)));
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
{
  size_t idx = getIndex(key);
  const List< std::pair< Key, Value > >& bucket = table_[idx];
  for (auto it = bucket.cbegin(); it != bucket.cend(); it++)
  {
    if (equal_(it->first, key))
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t idx = getIndex(key);
  List< std::pair< Key, Value > >& bucket = table_[idx];
  for (auto it = bucket.begin(); it != bucket.end(); it++)
  {
    if (equal_(it->first, key))
    {
      bucket.erase(it);
      size_--;
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::get(const Key& key)
{
  return at(key);
}

template< class Key, class Value, class Hash, class Equal >
const Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::get(
    const Key& key) const
{
  return at(key);
}

template< class Key, class Value, class Hash, class Equal >
Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::at(const Key& key)
{
  size_t idx = getIndex(key);
  List< std::pair< Key, Value > >& bucket = table_[idx];

  for (LIter< std::pair< Key, Value > > it = bucket.begin();
      it != bucket.end();
      ++it)
  {
    if (equal_(it->first, key))
    {
      return it->second;
    }
  }

  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
const Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::at(
    const Key& key) const
{
  size_t idx = getIndex(key);
  const List< std::pair< Key, Value > >& bucket = table_[idx];
  for (LCIter< std::pair< Key, Value > > it = bucket.cbegin();
      it != bucket.cend();
      ++it)
  {
    if (equal_(it->first, key))
    {
      return it->second;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
Value& zhuravleva::HashTable< Key, Value, Hash, Equal >::operator[](
    const Key& key)
{
  if (!has(key))
  {
    add(key, Value());
  }
  return at(key);
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::rehash(size_t newSize)
{
  if (newSize == 0)
  {
    throw std::invalid_argument("new size is zero");
  }
  myVector< List< std::pair< Key, Value > > > newTable(newSize);
  for (size_t i = 0; i < table_.size(); i++)
  {
    List< std::pair< Key, Value > >& bucket = table_[i];
    for (auto it = bucket.begin(); it != bucket.end(); it++)
    {
      size_t newindex = hasher_(it->first) % newSize;
      newTable[newindex].pushBack(*it);
    }
  }

  table_.swap(newTable);
}

template< class Key, class Value, class Hash, class Equal >
size_t zhuravleva::HashTable< Key, Value, Hash, Equal >::getIndex(const Key& key) const
{
  if (table_.empty())
  {
    throw std::runtime_error("hash table_ is empty");
  }
  return hasher_(key) % table_.size();
}

template< class Key, class Value, class Hash, class Equal >
bool zhuravleva::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::clear()
{
  for (size_t i = 0; i < table_.size(); i++)
  {
    List< std::pair< Key, Value > >& bucket = table_[i];
    bucket.clear();
  }
  size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
void zhuravleva::HashTable< Key, Value, Hash, Equal >::swap(
    HashTable& other) noexcept
{
  table_.swap(other.table_);
  size_t tempSize = size_;
  size_ = other.size_;
  other.size_ = tempSize;

  Hash tempHasher = hasher_;
  hasher_ = other.hasher_;
  other.hasher_ = tempHasher;

  Equal tempEqual = equal_;
  equal_ = other.equal_;
  other.equal_ = tempEqual;
}

#endif
