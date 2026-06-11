#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include "myVector.hpp"

namespace zhuravleva
{
  template< class Key >
  struct SecondHash
  {
    size_t operator()(const Key& key) const
    {
      std::hash< Key > hash;
      size_t value = hash(key);
      return value ^ (value >> 16);
    }
  };

  template< class Key, class Value,
    class Hash1 = std::hash< Key >,
    class Hash2 = SecondHash< Key >,
    class Equal = std::equal_to< Key > >
  class CuckooHashTable
  {
  public:
    CuckooHashTable();

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    void clear() noexcept;
    void erase(const Key& key);
    void swap(CuckooHashTable& other) noexcept;
    bool contains(const Key& key) const;
    Value& get(const Key& key);
    const Value& get(const Key& key) const;

  private:
    struct Cell
    {
      bool occupied;
      std::pair< Key, Value > data;

      Cell():
        occupied(false),
        data()
      {}
    };

    myVector< Cell > table1_;
    myVector< Cell > table2_;
    size_t size_;
    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;
    size_t getIndex1(const Key& key) const;
    size_t getIndex2(const Key& key) const;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable():
    table1_(8), table2_(8),
    size_(0), hash1_(), hash2_(), equal_()
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::capacity() const noexcept
  {
    return table1_.size() + table2_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < table1_.size(); ++i)
    {
      table1_[i].occupied = false;
    }
    for (size_t i = 0; i < table2_.size(); ++i)
    {
      table2_[i].occupied = false;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::erase(const Key& key)
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      table1_[index1].occupied = false;
      --size_;
      return;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      table2_[index2].occupied = false;
      --size_;
      return;
    }
    throw std::out_of_range("key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHashTable& other) noexcept
  {
    table1_.swap(other.table1_);
    table2_.swap(other.table2_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::getIndex1(const Key& key) const
  {
    return hash1_(key) % table1_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::getIndex2(const Key& key) const
  {
    return hash2_(key) % table2_.size();
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::contains(const Key& key) const
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return true;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return true;
    }

    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key)
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return table1_[index1].data.second;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return table2_[index2].data.second;
    }
    throw std::out_of_range("key not found");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key) const
  {
    size_t index1 = getIndex1(key);
    if (table1_[index1].occupied && equal_(table1_[index1].data.first, key))
    {
      return table1_[index1].data.second;
    }
    size_t index2 = getIndex2(key);
    if (table2_[index2].occupied && equal_(table2_[index2].data.first, key))
    {
      return table2_[index2].data.second;
    }
    throw std::out_of_range("key not found");
  }
}

#endif