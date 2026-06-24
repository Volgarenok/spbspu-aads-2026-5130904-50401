#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include "../common/vector.hpp"
#include "../common/list.hpp"
#include "hash_iter.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace sedov
{
  template < class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using valPair = std::pair< Key, Value >;

  public:
    using HIter = HashIter< Key, Value, Hash, Equal >;
    using HCIter = HashConstIter< Key, Value, Hash, Equal >;

    explicit HashTable(size_t slots);
    ~HashTable();

    HashTable(const HashTable & other);
    HashTable(HashTable && other) noexcept;
    HashTable & operator=(const HashTable & other);
    HashTable & operator=(HashTable && other) noexcept;

    void add(Key k, Value v);
    void erase(Key k);
    Value & at(Key k);
    const Value & at(Key k) const;
    bool contains(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable & other) noexcept;

    HIter begin();
    HIter end();
    HCIter begin() const;
    HCIter end() const;
    HCIter cbegin() const;
    HCIter cend() const;

  private:
    sedov::Vector< sedov::List< valPair > > data_;
    size_t cap_;
    size_t size_;
    Hash hasher_;
    Equal comp_;
  };
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  cap_(slots),
  size_(0),
  hasher_(Hash{}),
  comp_(Equal{})
{
  for (size_t i = 0; i < slots; ++i)
  {
    data_.pushBack(sedov::List< valPair >());
  }
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & other):
  data_(),
  cap_(other.cap_),
  size_(0),
  hasher_(other.hasher_),
  comp_(other.comp_)
{
  for (size_t i = 0; i < cap_; ++i)
  {
    data_.pushBack(sedov::List< valPair >());
  }
  for (size_t i = 0; i < other.cap_; ++i)
  {
    const sedov::List< valPair > & src = other.data_[i];
    for (LCIter< valPair > it = src.cbegin(); it != src.cend(); ++it)
    {
      data_[i].pushBack(*it);
      ++size_;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && other) noexcept:
  data_(std::move(other.data_)),
  cap_(other.cap_),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  comp_(std::move(other.comp_))
{
  other.cap_ = 0;
  other.size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal > &
sedov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable & other)
{
  if (this == std::addressof(other))
  {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cpy(other);
  swap(cpy);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
sedov::HashTable< Key, Value, Hash, Equal > &
sedov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable && other) noexcept
{
  if (this == std::addressof(other))
  {
    return *this;
  }
  swap(other);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  size_t idx = hasher_(k) % cap_;
  for (LIter< valPair > it = data_[idx].begin(); it != data_[idx].end(); ++it)
  {
    if (comp_(it->first, k))
    {
      it->second = v;
      return;
    }
  }
  data_[idx].pushBack(valPair(k, v));
  ++size_;
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashTable< Key, Value, Hash, Equal >::erase(Key k)
{
  size_t idx = hasher_(k) % cap_;
  for (LIter< valPair > it = data_[idx].begin(); it != data_[idx].end(); ++it)
  {
    if (comp_(it->first, k))
    {
      data_[idx].erase(it);
      --size_;
      return;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
Value & sedov::HashTable< Key, Value, Hash, Equal >::at(Key k)
{
  size_t idx = hasher_(k) % cap_;
  for (LIter< valPair > it = data_[idx].begin(); it != data_[idx].end(); ++it)
  {
    if (comp_(it->first, k))
    {
      return it->second;
    }
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
const Value & sedov::HashTable< Key, Value, Hash, Equal >::at(Key k) const
{
  size_t idx = hasher_(k) % cap_;
  const sedov::List< valPair > &bucket = data_[idx];
  for (LCIter< valPair > it = bucket.cbegin(); it != bucket.cend(); ++it)
  {
    if (comp_(it->first, k))
    {
      return it->second;
    }
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashTable< Key, Value, Hash, Equal >::contains(Key k) const noexcept
{
  size_t idx = hasher_(k) % cap_;
  const sedov::List<valPair> &bucket = data_[idx];
  for (LCIter< valPair > it = bucket.cbegin(); it != bucket.cend(); ++it)
  {
    if (comp_(it->first, k))
    {
      return true;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots <= cap_)
  {
    return;
  }
  sedov::Vector< sedov::List< valPair > > new_data;
  for (size_t i = 0; i < slots; ++i)
  {
    new_data.pushBack(sedov::List< valPair >());
  }
  for (size_t i = 0; i < cap_; ++i)
  {
    for (LIter< valPair > it = data_[i].begin(); it != data_[i].end(); ++it)
    {
      size_t idx = hasher_(it->first) % slots;
      new_data[idx].pushBack(*it);
    }
  }
  data_ = std::move(new_data);
  cap_ = slots;
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < cap_; ++i)
  {
    data_[i].clear();
  }
  size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
size_t sedov::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
bool sedov::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template < class Key, class Value, class Hash, class Equal >
void sedov::HashTable< Key, Value, Hash, Equal >::swap(HashTable & other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(comp_, other.comp_);
  std::swap(cap_, other.cap_);
  std::swap(size_, other.size_);
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HIter sedov::HashTable< Key, Value, Hash, Equal >::begin()
{
  return HIter(&data_, cap_, 0);
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HIter sedov::HashTable< Key, Value, Hash, Equal >::end()
{
  return HIter();
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HCIter sedov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return HCIter(&data_, cap_, 0);
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HCIter sedov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return HCIter();
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HCIter
sedov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return HCIter(&data_, cap_, 0);
}

template < class Key, class Value, class Hash, class Equal >
typename sedov::HashTable< Key, Value, Hash, Equal >::HCIter sedov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HCIter();
}

#endif
